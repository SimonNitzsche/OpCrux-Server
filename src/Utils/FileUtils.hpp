#ifndef __UTILS_FILEUTILS_HPP_
#define __UTILS_FILEUTILS_HPP_

#include <cstdio>
#include <string>
#include <memory>

namespace FileUtils {
	inline unsigned char* ReadFileCompletely(std::string filename, uint32_t fsize = 0) {
		FILE * file = fopen(filename.c_str(), "r+");
		if (file == nullptr) return nullptr;
		fseek(file, 0, SEEK_END);
		long int size = ftell(file);
		fclose(file);
		// Reading data to array of unsigned chars
		file = fopen(filename.c_str(), "r+");
		unsigned char * data;
		data = (unsigned char *)malloc(size);
		int bytes_read = fread(data, sizeof(unsigned char), size, file);
		fclose(file);
		fsize = bytes_read;
		return data;
	}
}

#endif // !__UTILS_FILEUTILS_HPP_