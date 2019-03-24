#ifndef __COMMON__CROSSPLATFORM_HPP__
#define __COMMON__CROSSPLATFORM_HPP__

#include "Platformident.hpp"

#ifdef LUR_PLATFORM_UNIX
#include <sys/socket.h>
#endif
#ifdef LUR_PLATFORM_WIN32
#include <WinSock2.h>
#endif

#if defined(LUR_PLATFORM_UNIX) || defined(LUR_PLATFORM_MACOS)
#include <unistd.h>
//Wintypes
#include <cstdint>
typedef std::uint8_t byte;
typedef std::uint8_t BYTE;
typedef std::uint16_t WORD;
typedef std::uint32_t DWORD;
typedef std::uint64_t QWORD;
#endif
#ifdef LUR_PLATFORM_WIN32
#include <ws2tcpip.h>
#include <windows.h>
#endif

#endif // !__COMMON__CROSSPLATFORM_HPP__
