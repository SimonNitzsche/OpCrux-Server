#ifndef __SERVER__MASTERSERVER_HPP__
#define __SERVER__MASTERSERVER_HPP__

#include <thread>
#include <vector>
#include <string>
#include <atomic>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/Types.h>
#include "DataTypes/LWOOBJID.hpp"

enum class SERVERMODE : uint8_t;

struct Machine;

struct MachineProcess {
public:
	Machine * machine = nullptr;
	uint16_t port;
	SERVERMODE server_mode;
	int processID;
};

enum ClientSessionMRState {
	IN_TRANSFER,
	IN_WORLD_LOADING,
	IN_WORLD
};

// Client Session Master Reference (server client session as reference for master)
struct ClientSessionMR {
public:
	unsigned long accountID;
	DataTypes::LWOOBJID objectID;
	SystemAddress systemAddress;
	MachineProcess * process;
	ClientSessionMRState sessionState;
};

struct Machine {
public:
	std::string machineName;
	std::string machineOS;
	std::string dottedIP;

	std::vector<MachineProcess> processes;
};

class MasterServer {
private:
	RakPeerInterface * rakServer = nullptr;
	std::thread listenThread;
	std::atomic_int32_t nextInstanceID = 0;
	int reserveInstanceID();
public:
	std::vector<Machine> connected_machines;
	std::vector<ClientSessionMR> connected_clients;
private:
	inline MachineProcess * GetMachineProcess(Packet * packet) {
		for (auto & connected_machine : connected_machines) {
			Machine* m = &connected_machine;
			if (m->dottedIP == packet->systemAddress.ToString(false)) {
				for (int j = 0; j < m->processes.size(); ++j) {
					MachineProcess* p = &(m->processes[j]);
					if (p->port == packet->systemAddress.port) {
						if (p->machine == nullptr) p->machine = m;
						return &(connected_machine.processes[j]);
					}
				}
			}
		}

		Logger::log("MasterServer", "GetMachineProcess(): Couldn't find MachineProcess.", LogType::UNEXPECTED);
		return nullptr;
	}

	inline ClientSessionMR* GetClientSessionMR(std::uint32_t accountID) {
		for (ClientSessionMR client : connected_clients) {
			if (client.accountID == accountID) {
				return &client;
			}
		}
		return nullptr;
	}
public:
	bool isDone = false;
	MasterServer();
	void Listen();
	~MasterServer();
};

/*
	Flow:
		Connect
		Auth
			MachineName
			ServerType
		OnAuth:
			
		OnWorld:
			UpdateInstances
1*/

#endif // !__SERVER__MASTERSERVER_HPP__
