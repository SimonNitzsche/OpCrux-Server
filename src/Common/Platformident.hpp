#ifndef __COMMON__PLATFORMIDENT_HPP__
#define __COMMON__PLATFORMIDENT_HPP__

//
// uniform platform identification macros
//

#if defined(_WIN32)
	#define OPCRUX_PLATFORM_WIN32
#elif defined(__APPLE__) && defined(__MACH__)
	#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
		#define OPCRUX_PLATFORM_IOS
	#elif TARGET_OS_MAC
		#define OPCRUX_PLATFORM_MACOS
	#else
		#error unknown Apple operating system
	#endif
#elif defined(__unix__)
	#define OPCRUX_PLATFORM_UNIX
	#if defined(__ANDROID__)
		#define OPCRUX_PLATFORM_ANDROID
	#elif defined(__linux__)
		#define OPCRUX_PLATFORM_LINUX
	#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
		#define OPCRUX_PLATFORM_FREEBSD
	#elif defined(__CYGWIN__)
		#define OPCRUX_PLATFORM_CYGWIN
	#else
		#error unknown unix operating system
	#endif
#else
	#error unknown operating system
#endif

#if defined OPCRUX_PLATFORM_WIN32
	#define OPCRUX_UNICODE_UTF16
#else
	#define OPCRUX_UNICODE_UTF8
#endif

#endif // !__COMMON__PLATFORMIDENT_HPP__
