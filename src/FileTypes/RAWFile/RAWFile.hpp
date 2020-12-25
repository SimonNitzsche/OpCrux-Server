#ifndef	__FILETYPES__RAWFILE__RAWFILE_HPP__
#define __FILETYPES__RAWFILE__RAWFILE_HPP__

#include <memory>
#include <string>
#include "RAWHeader.hpp"
#include "RAWChunk.hpp"

namespace RAW {
	class RAWFile {
		std::shared_ptr<unsigned char[]> filePtr;
		unsigned char* data;

		RAWHeader header;

		std::list<RAWChunk> chunks;
		
	public:
		void Open(const std::string& filePath);
		void Read(std::uint8_t** fileDataPtr);
		bool _isFileLoaded();
	};
};
#endif