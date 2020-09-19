#ifndef __FILETYPES__LVLFILE__TRIGGERFILE_HPP__
#define __FILETYPES__LVLFILE__TRIGGERFILE_HPP__

#include <string>
#include <vector>
#include <fstream>
#include <rapidxml/rapidxml.hpp>

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


using namespace rapidxml;

class ZoneTriggerFile {
public:
	std::vector<ZoneTrigger> triggers {};
public:
	bool Load(const char * filepath ) {
		xml_document<> doc;
		xml_node<> * root_node;

		Logger::log("WRLD", "Loading Scene Triggers: " + std::string(filepath));

		// Open file
		std::ifstream fstream(filepath);


		// File open
		if (fstream.is_open()) {
			// Copy file content to string
			std::string fcontent ((std::istreambuf_iterator<char>(fstream)),
				std::istreambuf_iterator<char>());

			// Parse xml from string
			doc.parse<0>(const_cast<char*>(fcontent.c_str()));
			
			// Select first node
			root_node = doc.first_node("triggers");

			// Fill Data
			for (xml_node<> * trigger_node = root_node->first_node("trigger"); trigger_node; trigger_node = trigger_node->next_sibling()) {
				int trigger_id = std::stoi(trigger_node->first_attribute("id")->value());
				bool trigger_enabled = std::stoi(trigger_node->first_attribute("enabled")->value());

				ZoneTrigger zTrigger(trigger_id, trigger_enabled);
				
				for (xml_node<> * event_node = trigger_node->first_node("event"); event_node; event_node = event_node->next_sibling()) {
					std::string event_id = event_node->first_attribute("id")->value();

					ZoneTriggerEvent zEvent(event_id);

					for (xml_node<> * command_node = event_node->first_node("command"); command_node; command_node = command_node->next_sibling()) {
						std::string command_id = command_node->first_attribute("id")->value();
						std::string command_target = command_node->first_attribute("target")->value();
						std::string command_targetName;
						if (command_target == "objGroup") {
							command_targetName = command_node->first_attribute("targetName")->value();
						}

						auto attr_args = command_node->first_attribute("args");
						std::string command_args = attr_args ? attr_args->value() : "";
					
						ZoneTriggerCommand zCommand(command_id, command_target, command_args, command_targetName);

						zEvent.commands.push_back(zCommand);
					}

					zTrigger.events.push_back(zEvent);
				}

				triggers.push_back(zTrigger);
			}
			return true;
		}
		return false;
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