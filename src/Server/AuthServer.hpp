#ifndef __SERVER__AUTHSERVER_HPP__
#define __SERVER__AUTHSERVER_HPP__

#include "Interfaces/ILUServer.hpp"

class AuthServer : public ILUServer {
private:
	RakPeerInterface* rakServer;
public:
	AuthServer();
	~AuthServer();
private:
	void RequestMasterUserAuthConfirmation(SystemAddress systemAddress, std::uint64_t accountID);
    std::string MakeAccountAPICall(std::string scope, std::unordered_map<std::string, std::string> args);
    void handlePacket(RakPeerInterface * rakServer, LUPacket * packet);
public:
	void MasterClientAuthResponse(SystemAddress systemAddress, int accountID, int reason);
};

#endif // !__SERVER__AUTHSERVER_HPP__
