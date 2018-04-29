#pragma once

enum class ERemoteConnection : unsigned char {
	GENERAL = 0,
	AUTH,
	CHAT,
	MASTER,
	SERVER,
	CLIENT
};