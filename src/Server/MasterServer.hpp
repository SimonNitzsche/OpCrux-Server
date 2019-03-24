#ifndef __SERVER__MASTERSERVER_HPP__
#define __SERVER__MASTERSERVER_HPP__

#include <thread>
#include <vector>
#include <string>
#include <atomic>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/Types.h>

enum class SERVERMODE : uint8_t;

struct MachineProcess {
public:
	uint16_t port;
	SERVERMODE server_mode;
	int processID;
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
	const int reserveInstanceID();
public:
	std::vector<Machine> connected_machines;
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
