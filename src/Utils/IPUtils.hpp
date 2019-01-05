#ifndef _IPUTILS__
#define _IPUTILS__

namespace IPUtils {
	bool isIPLocalHost(unsigned int ip) {
		return ip == 16777343;
	}

	bool isIPIntern(unsigned int ip) {
		return !isIPLocalHost(ip)
			&& ((ip & 0x0000ffff) == 43200
				|| (ip & 0x0000ffff == 4268)
				|| (ip & 0x000000ff == 10));
	}

	bool isIPPublic(unsigned int ip) {
		return !(isIPIntern(ip) || isIPLocalHost(ip));
	}

}

#endif