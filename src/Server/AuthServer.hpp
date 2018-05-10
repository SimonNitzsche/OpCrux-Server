#pragma once
#include "Interfaces/ILUServer.hpp"

class AuthServer : public ILUServer {
public:
	AuthServer(std::string masterServerIP);
	void Listen();
	~AuthServer();
};

