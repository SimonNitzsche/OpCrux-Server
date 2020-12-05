#ifndef __SERVER__AUTHSERVER_HPP__
#define __SERVER__AUTHSERVER_HPP__

#include "Interfaces/ILUServer.hpp"
#include "odbc-cpp-wrapper/src/odbc/Connection.h"

class AuthServer : public ILUServer {
private:
	RakPeerInterface* rakServer;
public:
	AuthServer();
	AuthServer(AuthServer** as);
	~AuthServer();
private:
	void RequestMasterUserAuthConfirmation(SystemAddress systemAddress, std::uint64_t accountID);
    std::string MakeAccountAPICall(std::string scope, std::unordered_map<std::string, std::string> args);
    void handlePacket(RakPeerInterface * rakServer, LUPacket * packet);
	odbc::ConnectionRef dbConnection;
public:
	odbc::ConnectionRef GetDBConnection() {
		return dbConnection;
	}
	void MasterClientAuthResponse(SystemAddress systemAddress, int accountID, int reason);
    void DoPlayerLoginSuccess(SystemAddress systemAddress, SystemAddress destination, std::string sessionKey);
};

#endif // !__SERVER__AUTHSERVER_HPP__
