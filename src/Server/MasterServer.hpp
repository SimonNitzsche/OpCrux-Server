#ifndef __SERVER__MASTERSERVER_HPP__
#define __SERVER__MASTERSERVER_HPP__

#include <thread>
#include <vector>
#include <string>
#include <atomic>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/Types.h>
#include "DataTypes/LWOOBJID.hpp"
#include "DataTypes/LDF.hpp"
#include <queue>

enum class SERVERMODE : uint8_t;

struct Machine;
struct RemoteWorldInstance;

struct MachineProcess {
public:
	Machine * machine = nullptr;
	uint16_t port;
	SERVERMODE server_mode;
	int processID;
	SystemAddress systemAddress;
	std::vector<RemoteWorldInstance*> instances;
};

enum class ClientSessionMRState {
	IN_TRANSFER_QUEUE,
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
	RemoteWorldInstance* currentInstance = nullptr;
	std::uint16_t connectedServerPort = 0;
	ClientSessionMRState sessionState;
	LDFCollection metadata;
	std::string sessionKey;

	template<typename T>
	void SetVar(std::u16string key, T data) {
		auto it = metadata.find(key);
		if (it != metadata.end()) {
			it->second = LDFEntry(key, data);
		}
		else {
			metadata.insert({ key, LDFEntry(key, data) });
		}
	}

	template<typename T = LDFEntry>
	LDFEntry GetVarEntry(std::u16string key) {
		if (metadata.find(key) != metadata.end())
			return metadata.at(key);
		else
			return LDFEntry(key, T());
	}

	template<typename T = LDFEntry>
	T GetVar(std::u16string key) {
		return static_cast<T>(static_cast<LDFEntry>(GetVarEntry<T>(key)));
	}
};

struct Machine {
public:
	std::string machineName;
	std::string machineOS;
	std::string dottedIP;

	std::vector<MachineProcess*> processes;
	std::queue<std::uint16_t> availablePorts;
};

struct RemoteWorldInstance {
	MachineProcess* process;
	std::uint16_t instanceID;
	std::uint16_t zoneID;
	std::uint32_t cloneID;
	std::uint16_t port;
};

class ClientSession;

class MasterServer {
private:
	RakPeerInterface * rakServer = nullptr;
	std::thread listenThread;
public:
	std::vector<Machine*> connected_machines;
	std::vector<ClientSessionMR*> connected_clients;
	std::vector<RemoteWorldInstance*> available_instances;
	std::vector<RemoteWorldInstance*> pending_instances;
	std::unordered_map<std::uint16_t, std::uint16_t> instanceID_counter;
private:
	inline MachineProcess * GetMachineProcess(Packet * packet) {
		for (int i = 0; i < connected_machines.size(); ++i) {
			Machine* m = (connected_machines[i]);
			if (m->dottedIP == packet->systemAddress.ToString(false)) {
				for (int j = 0; j < m->processes.size(); ++j) {
					MachineProcess* p = (m->processes[j]);
					if (p->port == packet->systemAddress.port) {
						if (p->machine == nullptr) p->machine = m;
						return (connected_machines[i]->processes[j]);
					}
				}
			}
		}

		Logger::log("MasterServer", "GetMachineProcess(): Couldn't find MachineProcess.", LogType::UNEXPECTED);
		return nullptr;
	}

	inline ClientSessionMR* GetClientSessionMR(std::uint32_t accountID) {
		for (auto it = connected_clients.begin(); it != connected_clients.end(); ++it) {
			if ((*it)->accountID == accountID) {
				return *it;
			}
		}
		return nullptr;
	}
public:
	bool isDone = false;
	MasterServer();
	void Listen();
	void CheckTransferQueue(std::uint16_t zoneID, std::uint16_t instanceID, std::uint32_t cloneID);
	std::uint32_t GetPlayerCountOfInstance(RemoteWorldInstance* instance);
	RemoteWorldInstance* SelectInstanceToJoin(std::uint16_t zoneID, std::uint32_t cloneID = 0, bool ignoreSoftCap = false);
	void RequestNewZoneInstance(std::uint16_t zoneID, std::uint32_t cloneID);
	ClientSessionMR* GetClientByObjectID(DataTypes::LWOOBJID objectID);
	ClientSessionMR* GetClientByAccountID(std::uint32_t accountID);
	RemoteWorldInstance * GetInstanceByMachineProcessAndPort(MachineProcess * machineProcess, std::uint16_t port);
	ClientSession ConvertSessionToCl(ClientSessionMR* sessionMR);
	void ClientWorldTransferFinishedResponse(ClientSessionMR* sessionMR);
	~MasterServer();


	RemoteWorldInstance* GetHubCharServer();

    void MovePlayerFromAuthToSession(ClientSessionMR * playerSession, RemoteWorldInstance * instance);
	void MovePlayerSessionToNewInstance(ClientSessionMR * playerSession, RemoteWorldInstance * instance);
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
