#ifndef __SESSIONS__SESSIONMANAGER_HPP__
#define __SESSIONS__SESSIONMANAGER_HPP__
/// NOTE:
// This is attached to the master server.

#include "ClientSession.hpp"
#include <vector>
#include <stdlib.h>

class SessionManager {
	std::vector<std::shared_ptr<ClientSession>> clients;
};

#endif // !__SESSIONS__SESSIONMANAGER_HPP__
