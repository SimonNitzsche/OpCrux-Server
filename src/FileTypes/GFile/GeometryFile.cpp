#include "FileTypes/GFile/GeometryFile.hpp"
#include <fstream>
#include <vector>
#include <iostream>
#include "Utils/FileUtils.hpp"

GeometryFile::GeometryFile(const char * filepath) {
	if (filepath == nullptr) filepath = "";

	this->fileData = FileUtils::ReadFileCompletely(std::string(filepath), &this->fileSize);

	unsigned char* filePtr = fileData.get();

	this->magic = reinterpret_cast<std::uint32_t*>(filePtr + 0);
	this->vertexCount = reinterpret_cast<std::uint32_t*>(filePtr + 4);
	this->indexCount = reinterpret_cast<std::uint32_t*>(filePtr + 8);
	this->options = reinterpret_cast<std::uint32_t*>(filePtr + 12);

	this->vertexCoordinates = reinterpret_cast<DataTypes::Vector3*>(filePtr + 16);

	std::uint8_t * filePosition = reinterpret_cast<unsigned char*>(this->vertexCoordinates + *this->vertexCount);
	
	if ((*this->options & 0x02) != 0) {
		this->vertexNormals = reinterpret_cast<DataTypes::Vector3*>(filePosition);
		filePosition = reinterpret_cast<unsigned char*>(this->vertexNormals + *this->vertexCount);
	}

	if ((*this->options & 0x01) != 0) {
		this->textureCoordinates = reinterpret_cast<UV*>(filePosition);
		filePosition = reinterpret_cast<unsigned char*>(this->textureCoordinates + *this->vertexCount);
	}

	this->triangles = reinterpret_cast<std::uint32_t *>(filePosition);
}


GeometryFile::~GeometryFile()
{
}
