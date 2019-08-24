#ifndef __COMMON__CROSSPLATFORM_HPP__
#define __COMMON__CROSSPLATFORM_HPP__

#include "Platformident.hpp"

/* Windows Only Stuff */
#ifdef OPCRUX_PLATFORM_WIN32
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <direct.h>
#endif
/* Unix Only Stuff */
#ifdef OPCRUX_PLATFORM_UNIX
#include <sys/socket.h>
#include <unistd.h>
#include <cstdint>
#include <libgen.h>
typedef std::uint8_t byte;
typedef std::uint8_t BYTE;
typedef std::uint16_t WORD;
typedef std::uint32_t DWORD;
typedef std::uint64_t QWORD;
#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),(mode)))==NULL
#endif

#endif // !__COMMON__CROSSPLATFORM_HPP__
