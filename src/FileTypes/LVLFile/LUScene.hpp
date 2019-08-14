#ifndef __FILETYPES__LVLFILE__LUSCENE_HPP__
#define __FILETYPES__LVLFILE__LUSCENE_HPP__

#include <string>

#include "Utils/FileUtils.hpp"
#include<memory>
#include "FileTypes/LVLFile/LVLInfoChunk.hpp"
#include "FileTypes/LVLFile/LVLObjectsChunk.hpp"
namespace FileTypes::LUZ {
	class LUZone;
}

class LUScene {
	std::shared_ptr<unsigned char[]> filePtr; // We need to store it here, so we don't loose it.

	LVLInfoChunk infoChunk;
	void * skyChunk;
	LVLObjectsChunk objectsChunk;
	void * envConfigChunk;
public:
	LUScene() {}
	LUScene(FileTypes::LUZ::LUZone * zone, std::string file);
};
#endif