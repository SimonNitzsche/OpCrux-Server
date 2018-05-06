#pragma once
#ifndef _MASTERSERVER_HPP
#define _MASTERSERVER_HPP

#include <thread>
#include <RakNet\RakPeerInterface.h>

class MasterServer {
private:
	RakPeerInterface * rakServer = nullptr;
	std::thread listenThread;
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