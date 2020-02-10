#pragma once
#ifndef __GEOMETRYFILE_HPP__
#define __GEOMETRYFILE_HPP__

#include <memory>
#include <stdlib.h>

#include "DataTypes/Vector3.hpp"

struct UV;

class GeometryFile
{
private:
	std::unique_ptr<unsigned char[]> fileData;

public:
	std::uint32_t * magic;
	std::uint32_t * vertexCount;
	std::uint32_t * indexCount;
	std::uint32_t * options;

public:
	DataTypes::Vector3 * vertexCoordinates;
public:
	DataTypes::Vector3 * vertexNormals;
public:
	UV * textureCoordinates;
public:
	std::uint32_t * triangles;

public:
	std::uint32_t fileSize;
	GeometryFile(const char* filepath);
	~GeometryFile();
};

struct UV{
public:
	float u;
	float v;
};

#endif // !__GEOMETRYFILE_HPP__