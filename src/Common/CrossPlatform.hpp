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
typedef unsigned char byte;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
#define GetCurrentProcessId() getpid()
#endif
#ifdef WIN32
#include <ws2tcpip.h>
#include <windows.h>
#endif

#endif