#pragma once
#include "Interfaces/ILUServer.hpp"

class AuthServer : public ILUServer {
public:
	AuthServer();
	void Listen();
	~AuthServer();
};

