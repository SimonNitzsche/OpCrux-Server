#ifndef __UTILS__FILEUTILS_HPP__
#define __UTILS__FILEUTILS_HPP__

#include <RakNet/BitStream.h>

#include "Utils/Logger.hpp"
#include "Common/CrossPlatform.hpp"

#include "Utils/ServerInfo.hpp"

#include <algorithm>
#include <cstdio>
#include <string>
#include <memory>
#include <fstream>

#ifdef OPCRUX_PLATFORM_WIN32

#include <filesystem>
/* Make sure filesystem gets loaded correctly. */
#if __cplusplus < 201703L && __cplusplus < 180000 // If the version of C++ is less than 17
// It was still in the experimental:: namespace
namespace fs = std::experimental::filesystem;
#else
namespace fs = std::filesystem;
#endif
#endif


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
	
	inline bool FileExists(std::string &file) {
		struct stat buf;
		return (stat(file.c_str(), &buf) == 0);
	}

	inline std::string ReadTextFile(std::string& filename) {
		// Open
		FILE* file;
		fopen_s(&file, filename.c_str(), "r");

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
		return std::string(data.get(), data.get() + size);
	}

	inline void SaveTextFile(std::string &file, std::string &data) {
		std::ofstream stream;
		stream.open(file);
		stream << data;
		stream.close();
	}

	/* Used by SavePacket(...), to know the current packet index for saving. */
	static std::uint32_t savedPacketIndex = 0;
	static std::string savedPacketFolder = "";

	inline void SavePacket(RakNet::BitStream * packetToSave, std::uint16_t portFrom, std::uint16_t portTo) {
#ifdef OPCRUX_PLATFORM_WIN32
		// Copy packet
		unsigned char * _data = packetToSave->GetData();

		// make folder (timestamp) for organization
		if(savedPacketFolder == "")
		savedPacketFolder = "log/packets/" + std::to_string(ServerInfo::startupStamp());
		if (!fs::exists(savedPacketFolder))
			fs::create_directories(savedPacketFolder);
		// Get packet info (filename)
		std::string saveFileName = savedPacketFolder + "/"
			+ std::to_string(savedPacketIndex++) + "_"
			+ std::to_string(portFrom) + "_"
			+ std::to_string(portTo) + "_";

		if (*_data == 0x53) {
			saveFileName += "[" + std::string(StringUtils::CharToHex((*_data + 0)))
				+ "-" + StringUtils::CharToHex((*_data + 1))
				+ "-" + StringUtils::CharToHex((*_data + 2))
				+ "-" + StringUtils::CharToHex((*_data + 3)) + "]";

			// Game-MSG
			if (*reinterpret_cast<std::uint32_t*>(*_data) == 0x05000453 || *reinterpret_cast<std::uint32_t*>(*_data) == 0x0c000553) {
				saveFileName += "_[" + std::string(StringUtils::CharToHex((*_data + 8)))
					+ "-" + StringUtils::CharToHex((*_data + 9)) + "]";
			}
		}
		else {
			saveFileName += "[" + std::string(StringUtils::CharToHex((*_data + 0))) + "]";
		}

		saveFileName += ".bin";

		// save
		std::ofstream file (saveFileName.c_str());
		file.write((char*)_data, packetToSave->GetNumberOfBytesUsed());

#endif
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
