#ifndef __FILETYPES__LVLFILE__TRIGGERFILE_HPP__
#define __FILETYPES__LVLFILE__TRIGGERFILE_HPP__

#include <string>
#include <vector>
#include <fstream>
#include <rapidxml/rapidxml.hpp>
#include <TinyXML2/tinyxml2.h>

#include "Utils/StringUtils.hpp"

struct ZoneTriggerCommand {
	std::string id;
	std::string target;
	std::string targetName;
	std::vector<std::string> args {};

	ZoneTriggerCommand(std::string _id, std::string _target, std::string _args, std::string _targetName = "")
		: id(_id), target(_target), args(StringUtils::splitString(_args, ',')), targetName(_targetName) {}
};

struct ZoneTriggerEvent {
	std::string id;
	std::vector<ZoneTriggerCommand> commands {};

	ZoneTriggerEvent(std::string _id)
		: id(_id) {}

	ZoneTriggerEvent()
		: id("NULL"), commands({}) {}
};

struct ZoneTrigger {
	int id;
	bool enabled;
	std::vector<ZoneTriggerEvent> events {};
	
	ZoneTrigger() : id(-1), enabled(false), events({}) {}

	ZoneTrigger(int _id, bool _enabled)
		: id(_id), enabled(_enabled), events({}) {}
};

class ZoneTriggerFile {
public:
	std::vector<ZoneTrigger> triggers {};
public:
	bool Load(const char * filepath ) {
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLElement* root_node;

		Logger::log("WRLD", "Loading Scene Triggers: " + std::string(filepath));

		std::ifstream fstream = std::ifstream();
		fstream.open(filepath, std::ios::in);
		if (fstream.is_open()) {
			std::string fcontent((std::istreambuf_iterator<char>(fstream)),
				std::istreambuf_iterator<char>());

			doc.Parse(fcontent.c_str());
		}
		else {
			Logger::log("WRLD", "Failed to load scene trigger: " + std::string(filepath));
			return false;
		}

		root_node = doc.FirstChildElement("triggers");

		for (tinyxml2::XMLElement* trigger_node = root_node->FirstChildElement("trigger");
			trigger_node != NULL;
			trigger_node = trigger_node->NextSiblingElement("trigger")
		) {
			int trigger_id = std::stoi(trigger_node->FindAttribute("id")->Value());
			bool trigger_enabled = std::stoi(trigger_node->FindAttribute("enabled")->Value());

			ZoneTrigger zTrigger(trigger_id, trigger_enabled);
				
			for (tinyxml2::XMLElement* event_node = trigger_node->FirstChildElement("event"); event_node; event_node = event_node->NextSiblingElement("event")) {
				std::string event_id = event_node->FindAttribute("id")->Value();

				ZoneTriggerEvent zEvent(event_id);

				for (tinyxml2::XMLElement* command_node = event_node->FirstChildElement("command"); command_node; command_node = command_node->NextSiblingElement("command")) {
					std::string command_id = command_node->FindAttribute("id")->Value();
					std::string command_target = command_node->FindAttribute("target")->Value();
					std::string command_targetName = "";
					if (command_target == "objGroup") {
						command_targetName = command_node->FindAttribute("targetName")->Value();
					}

					auto attr_args = command_node->FindAttribute("args");
					std::string command_args = attr_args ? attr_args->Value() : "";
					
					ZoneTriggerCommand zCommand(command_id, command_target, command_args, command_targetName);

					zEvent.commands.push_back(zCommand);
				}

				zTrigger.events.push_back(zEvent);
			}

			triggers.push_back(zTrigger);
		}
		return true;
	}


#define TF_EVENT_FUN(evname, evfunc) void evname() evfunc

	TF_EVENT_FUN(OnDestroy, {
		
	});
	TF_EVENT_FUN(OnCustomEvent, {

	});
	TF_EVENT_FUN(OnEnter, {

	});
	TF_EVENT_FUN(OnExit, {

	});
	TF_EVENT_FUN(OnCreate, {

	});
	TF_EVENT_FUN(OnHit, {

	});
	TF_EVENT_FUN(OnTimerDone, {

	});
	TF_EVENT_FUN(OnRebuildComplete, {

	});
	TF_EVENT_FUN(OnActivated, {

	});
	TF_EVENT_FUN(OnDeactivated, {

	});
	TF_EVENT_FUN(OnArrived, {

	});
	TF_EVENT_FUN(OnArrivedAtEndOfPath, {

	});
	TF_EVENT_FUN(OnZoneSummaryDismissed, {

	});
	TF_EVENT_FUN(OnArrivedAtDesiredWaypoint, {

	});
	TF_EVENT_FUN(OnPetOnSwitch, {

	});
	TF_EVENT_FUN(OnPetOffSwitch, {

	});
	TF_EVENT_FUN(OnInteract, {

	});
};

#endif