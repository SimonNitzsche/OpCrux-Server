#pragma once

//
// uniform platform identification macros
//

#if defined(_WIN32)
	#define LUR_PLATFORM_WIN32
#elif defined(__APPLE__) && defined(__MACH__)
	#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
		#define LUR_PLATFORM_IOS
	#elif TARGET_OS_MAC
		#define LUR_PLATFORM_MACOS
	#else
		#error unknown Apple operating system
	#endif
#elif defined(__unix__)
	#define LUR_PLATFORM_UNIX
	#if defined(__ANDROID__)
		#define LUR_PLATFORM_ANDROID
	#elif defined(__linux__)
		#define LUR_PLATFORM_LINUX
	#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
		#define LUR_PLATFORM_FREEBSD
	#else
		#error unknown unix operating system
	#endif
#else
	#error unknown operating system
#endif

#if defined LUR_PLATFORM_WIN32
	#define LUR_UNICODE_UTF16
#else
	#define LUR_UNICODE_UTF8
#endif
