#include "HKXCacheManager.hpp"

#include "hkxFile.hpp"

#include "Utils/Logger.hpp"
#include "Utils/StringUtils.hpp"

std::map<std::size_t, HKX::HKXFile*> HKXCacheManager::fileCache = {};

HKX::HKXFile* HKXCacheManager::GetHKXFile(std::string filename) {
	StringUtils::replace(filename, "\\", "/");

	// Hash the filename as lookup
	std::size_t fnameHash = std::hash<std::string>()(filename);

	// Check if we have entries
	if (fileCache.size() != 0) {

		// Check if we already have it
		auto cacheIt = fileCache.find(fnameHash);
		if (cacheIt != fileCache.end())
			// And return if so
			return cacheIt->second;
	}

	// At this point we don't have it already and need to load it
	Logger::log("WRLD", "Loading hkxFile: " + filename);
	HKX::HKXFile* hkxFile = new HKX::HKXFile();
	if (!hkxFile->Load(filename)) {
		// Load returned false, couldn't load file, return nullptr
		delete hkxFile;
		hkxFile = nullptr;
	}

	// after loading store it in the cache, for later
	fileCache.insert({ fnameHash, hkxFile });

	// and return it
	return hkxFile;
}