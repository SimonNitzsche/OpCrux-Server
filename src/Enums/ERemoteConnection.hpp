#pragma once
#ifndef __ENUMS__EREMOTECONNECTION_HPP__
#define __ENUMS__EREMOTECONNECTION_HPP__
#include <memory>

enum class ERemoteConnection : uint16_t {
	GENERAL = 0,
	AUTH,
	CHAT,
	MASTER,
	SERVER,
	CLIENT
};
#endif