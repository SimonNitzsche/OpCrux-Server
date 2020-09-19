#ifndef __SESSIONS__SESSIONMANAGER_HPP__
#define __SESSIONS__SESSIONMANAGER_HPP__
/// NOTE:
// This is attached to the client server

#include "ClientSession.hpp"
#include <vector>
#include <stdlib.h>
#include "DataTypes/LWOOBJID.hpp"

class SessionManager {
	std::vector<ClientSession> clients;
public:

	std::vector<ClientSession> GetClients() {
		return clients;
	}

	ClientSession * GetSession(SystemAddress systemAddress) {
		for (auto & client : clients) {
			ClientSession * session = &client;
			if ((session->systemAddress) != systemAddress) continue;
			return session;
		}
		return nullptr;
	}

	ClientSession* GetSession(DataTypes::LWOOBJID object) {
		for (auto & client : clients) {
			ClientSession* session = &client;
			if ((session->actorID) != object) continue;
			return session;
		}
		return nullptr;
	}

	void AddSession(const ClientSession& csCopy) {
		clients.push_back(csCopy);
	}
	void RemoveSession(ClientSession * session) {
		for (int i = 0; i < clients.size(); ++i) {
			if (&clients[i] == session) {
				clients.erase(clients.begin() + i);
				return;
			}
		}
	}
};

#endif // !__SESSIONS__SESSIONMANAGER_HPP__
