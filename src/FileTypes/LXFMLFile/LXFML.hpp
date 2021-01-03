#ifndef FILETYPES_LXFMLFILE
#define FILETYPES_LXFMLFILE

#include <vector>
#include <string>

#include "Utils/StringUtils.hpp"

#include <TinyXML2/tinyxml2.h>

struct Part {
	int32_t Material;
};

struct Brick {
	Part DesignPart;
	uint32_t RefID;
	uint32_t DesignID;
};

class LXFML {
public:

	bool Loaded = true;

	std::vector<Brick> Bricks;

	LXFML(std::string filename);
};


#endif // !FILETYPES_LXFMLFILE