#pragma once
#ifndef __CROSSPLATFORM_HPP
#define __CROSSPLATFORM_HPP

#ifdef __unix__
#include <sys/socket.h>
#endif
#ifdef WIN32
#include <WinSock2.h>
#endif

#if defined(__unix__) || defined(MAC)
#include <unistd.h>
//Wintypes
#include <cstdint.h>
typedef std::uint8_t byte;
typedef std::uint8_t BYTE;
typedef std::uint16_t WORD;
typedef std::uint32_t DWORD;
typedef std::uint64_t QWORD;
#endif
#ifdef WIN32
#include <ws2tcpip.h>
#include <windows.h>
#endif

#endif
