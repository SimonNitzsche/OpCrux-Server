#pragma once
#ifndef _MASTERSERVER_HPP
#define _MASTERSERVER_HPP

#include <thread>
#include <vector>
#include <string>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/Types.h>

extern enum class SERVERMODE : uint8_t;

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

#endif