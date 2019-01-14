#ifndef __ENUMS__EREMOTECONNECTION_HPP__
#define __ENUMS__EREMOTECONNECTION_HPP__

#include <memory>

namespace Enums {

	/*
		What kind of remote connection is this?
	*/
	enum class ERemoteConnection : uint16_t {
		GENERAL = 0,
		AUTH,
		CHAT,
		MASTER,
		SERVER,
		CLIENT
	};
}

#endif // !__ENUMS__EREMOTECONNECTION_HPP__
