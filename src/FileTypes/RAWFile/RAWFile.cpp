#include "RAWFile.hpp"
#include <Utils/FileUtils.hpp>

void RAW::RAWFile::Open(const std::string & filePath) {
	filePtr = FileUtils::ReadFileCompletely(filePath);
	data = filePtr.get();

	if (_isFileLoaded()) {
		Read(&data);
	}

	// std::string pth = "res/mg/___test";
	// SaveMesh(pth);
}

void RAW::RAWFile::Read(std::uint8_t** fileDataPtr) {
	header.Read(fileDataPtr);
	if (header.version < 0x20) throw "Unsupported terrain format version.";
	
	chunks.resize(header.chunkCount);
	for (auto & chunk : chunks) {
		chunk.Read(fileDataPtr);
		int c = 3;
	}
}

bool RAW::RAWFile::_isFileLoaded() {
	return data != nullptr;
}
