#ifndef __FILETYPES__HKXFILE__HKXCACHEMANAGER_HPP__
#define __FILETYPES__HKXFILE__HKXCACHEMANAGER_HPP__

#include <memory>
#include <string>
#include <map>

namespace HKX {
	class HKXFile;
}

class HKXCacheManager {
private:
	static std::map<std::size_t, HKX::HKXFile*> fileCache;
public:
	static HKX::HKXFile* GetHKXFile(std::string filename);
};

#endif