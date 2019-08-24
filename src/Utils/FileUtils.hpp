#ifndef __UTILS__FILEUTILS_HPP__
#define __UTILS__FILEUTILS_HPP__

#include "Utils/Logger.hpp"
#include "Common/CrossPlatform.hpp"

#include <algorithm>
#include <cstdio>
#include <string>
#include <memory>

#include "Utils/StringUtils.hpp"

namespace FileUtils {
	inline std::unique_ptr<unsigned char[]> ReadFileCompletely(std::string filename, uint32_t * fsize = 0) {
		// Open
		FILE * file;
		StringUtils::ToLowerSelf(filename);
		fopen_s(&file, filename.c_str(), "rb");
		
		// Check
		if (file == nullptr) { 
			Logger::log("FileUtils::ReadFileCompletely", "Couldn't load file \"" + filename + "\"");
			return nullptr;
		}

		// Get file size
		fseek(file, 0, SEEK_END);
		long int size = ftell(file);

		// Reading data to array of unsigned chars
		std::unique_ptr<unsigned char[]> data = std::make_unique<unsigned char[]>(size);
		fseek(file, 0, SEEK_SET);
		int bytes_read = fread(data.get(), sizeof(unsigned char), size, file);

		// Cleanup
		fclose(file);
		
		// return
		if(fsize != nullptr)
			*fsize = bytes_read;
		return data;
	}

	inline std::string GetFileDir(std::string file) {
		return file.substr(0, file.find_last_of("\\/"));
	}
	
	inline void ChangeDirectory() {
		// Change directory to directory of the executable, since we're using relative paths
#ifdef OPCRUX_PLATFORM_WIN32
		// TODO: Windows Solution
		//_chdir(path)
#endif
#ifdef OPCRUX_PLATFORM_UNIX
		char * result;
		ssize_t count = readlink("/proc/self/exe", result, 1024);
		if (count != -1) {
			chdir(dirname(result));
		}
#endif
	}
	
};

#endif // !__UTILS__FILEUTILS_HPP__
