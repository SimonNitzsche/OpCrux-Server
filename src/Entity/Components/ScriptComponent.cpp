#include "ScriptComponent.hpp"
#include "Entity/GameObject.hpp"

#include "Entity/NativeScripts/02_server/Equipment/L_MAESTROM_EXTRACTICATOR_SERVER.hpp"
#include "Entity/NativeScripts/02_server/Map/AG/L_AG_CAGED_BRICKS_SERVER.hpp"
#include "Entity/NativeScripts/02_server/Map/AG/L_AG_MONUMENT_BIRDS.hpp"
#include "Entity/NativeScripts/02_server/Map/General/L_TOUCH_MISSION_UPDATE_SERVER.hpp"
#include "Entity/NativeScripts/02_server/Map/Property/AG_Small/L_ZONE_AG_PROPERTY.hpp"
#include "Entity/NativeScripts/ai/AG/L_AG_BUS_DOOR.hpp"
#include "Entity/NativeScripts/ai/AG/L_AG_DARKLING_MECH.hpp"
#include "Entity/NativeScripts/ai/AG/L_AG_FANS.hpp"
#include "Entity/NativeScripts/ai/AG/L_AG_IMAG_SMASHABLE.hpp"
#include "Entity/NativeScripts/ai/AG/L_AG_SHIP_PLAYER_SHOCK_SERVER.hpp"
#include "Entity/NativeScripts/ai/AG/L_AG_SHIP_SHAKE.hpp"
#include "Entity/NativeScripts/ai/AG/L_AG_SPACE_STUFF.hpp"
#include "Entity/NativeScripts/ai/AG/L_AG_SHIP_PLAYER_DEATH_TRIGGER.hpp"
#include "Entity/NativeScripts/ai/FV/L_ACT_CANDLE.hpp"
#include "Entity/NativeScripts/ai/NS/L_NS_CONCERT_INSTRUMENT_QB.hpp"
#include "Entity/NativeScripts/ai/NS/L_NS_GET_FACTION_MISSION_SERVER.hpp"
#include "Entity/NativeScripts/ai/NP/L_NPC_NP_SPACEMAN_BOB.hpp"
#include "Entity/NativeScripts/ai/RACING/TRACK_GF/GF_RACE_SERVER.hpp"
#include "Entity/NativeScripts/ai/WILD/L_ALL_CRATE_CHICKEN.hpp"
#include "Entity/NativeScripts/zone/AG/L_ZONE_AG_SURVIVAL.hpp"
#include "Entity/NativeScripts/ScriptComponent_0952_script_name__removed.hpp"
#include "Entity/NativeScripts/ScriptComponent_1021_script_name__removed.hpp"

const std::unordered_map<std::string, script_factory> ScriptComponent::factories = {
		{"TestScript", []() -> script_ptr {return std::make_shared<TestScript>(); }},
		{"ScriptComponent_1582_script_name__removed", []()->script_ptr {return std::make_shared<NATIVESCRIPT__02SERVER__EQUIPMENT__L_MAESTROM_EXTRACTICATOR_SERVER>(); }},
		{"ScriptComponent_1586_script_name__removed", []()->script_ptr {return std::make_shared<NATIVESCRIPT__02_SERVER__MAP__AG__L_AG_CAGED_BRICKS_SERVER>(); }},
		{"ScriptComponent_1584_script_name__removed", []()->script_ptr {return std::make_shared<NATIVESCRIPT__02_SERVER__MAP__AG__L_AG_MONUMENT_BIRDS>(); }},
		{"scripts\\02_server\\Map\\General\\L_TOUCH_MISSION_UPDATE_SERVER.lua", []()->script_ptr {return std::make_shared<NATIVESCRIPT__02SERVER__MAP__GENERAL__L_TOUCH_MISSION_UPDATE_SERVER>(); }},
		{"ScriptComponent_1136_script_name__removed", []()->script_ptr {return std::make_shared<NATIVESCRIPT__02SERVER__MAP__PROPERTY__AG_SMALL__L_ZONE_AG_PROPERTY>(); }},
		{"scripts\\ai\\AG\\L_AG_BUS_DOOR.lua", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__AG__L_AG_BUS_DOOR>(); }},
		{"scripts\\ai\\AG\\L_AG_FANS.lua", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__AG__L_AG_FANS>(); }},
		{"scripts\\ai\\AG\\L_AG_IMAG_SMASHABLE.lua", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__AG__L_AG_IMAG_SMASHABLE>(); }},
		{"scripts\\ai\\AG\\L_AG_SHIP_PLAYER_SHOCK_SERVER.lua", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__AG__L_AG_SHIP_PLAYER_SHOCK_SERVER>(); }},
		{"scripts\\ai\\AG\\L_AG_SHIP_SHAKE.lua", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__AG__L_AG_SHIP_SHAKE>(); }},
		{"scripts\\ai\\AG\\L_AG_SPACE_STUFF.lua", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__AG__L_AG_SPACE_STUFF>();  }},
		{"scripts\\ai\\AG\\L_AG_SHIP_PLAYER_DEATH_TRIGGER.lua", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__AG__L_AG_SHIP_PLAYER_DEATH_TRIGGER>();  }},
		{"ScriptComponent_1094_script_name__removed", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__FV__L_ACT_CANDLE>(); }},
		{"scripts\\ai\\NP\\L_NPC_NP_SPACEMAN_BOB.lua", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__NP__L_NPC_SPACEMAN_BOB>();  }},
		{"scripts\\ai\\NS\\L_NS_CONCERT_INSTRUMENT_QB.lua", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__NS__L_NS_CONCERT_INSTRUMENT_QB>(); }},
		{"scripts\\ai\\NS\\L_NS_GET_FACTION_MISSION_SERVER.lua", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__NS__L_NS_GET_FACTION_MISSION_SERVER>(); }},
		{"scripts\\ai\\RACING\\TRACK_GF\\GF_RACE_SERVER.lua", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__RACING__TRACK_GF__GF_RACE_SERVER>(); }},
		{"scripts\\ai\\WILD\\L_ALL_CRATE_CHICKEN.lua", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__WILD__L_ALL_CRATE_CHICKEN>(); }},
		{"scripts\\zone\\AG\\L_ZONE_AG_SURVIVAL.lua", []()->script_ptr {return std::make_shared < NATIVESCRIPT__ZONE__AG__L_ZONE_AG_SURVIVAL>(); }},
		{"ScriptComponent_952_script_name__removed", []()->script_ptr {return std::make_shared<NATIVESCRIPT__ScriptComponent_0952_script_name__removed>(); }},
		{"ScriptComponent_1021_script_name__removed", []()->script_ptr {return std::make_shared<NATIVESCRIPT__ScriptComponent_1021_script_name__removed>(); }},
		{"ScriptComponent_815_script_name__removed", []()->script_ptr {return std::make_shared<NATIVESCRIPT__AI__AG__L_AG_DARKLING_MECH>(); }}
};