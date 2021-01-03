#include "LXFML.hpp"


LXFML::LXFML(std::string FileName) : Bricks(std::vector<Brick>()) {
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError err = doc.LoadFile(FileName.c_str());
	if (err == tinyxml2::XMLError::XML_ERROR_FILE_NOT_FOUND) {
		this->Loaded = false;
		return;
	}

	tinyxml2::XMLElement* RootElement = doc.RootElement();
	tinyxml2::XMLElement* BrickElement = RootElement->FirstChildElement("Bricks");

	for (tinyxml2::XMLElement* child = BrickElement->FirstChildElement("Brick"); child != NULL; child = child->NextSiblingElement("Brick")) {
		Brick current;

		current.RefID = std::stoi(child->FindAttribute("refID")->Value());
		current.DesignID = std::stoi(child->FindAttribute("designID")->Value());

		Part currentPart;
		auto materials = StringUtils::splitString(child->FirstChildElement("Part")->FindAttribute("materials")->Value(), ',');

		currentPart.Material = std::stoi(materials[0]);

		current.DesignPart = currentPart;

		Bricks.push_back(current);
	}

}
