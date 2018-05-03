#pragma once

enum class ERemoteConnection : WORD {
	GENERAL = 0,
	AUTH,
	CHAT,
	MASTER,
	SERVER,
	CLIENT
};