#ifndef __ENTITY__NATIVESCRIPTS__EQUIPMENTTRIGGERS__ASSEMBLY1_HPP__
#define __ENTITY__NATIVESCRIPTS__EQUIPMENTTRIGGERS__ASSEMBLY1_HPP__
#include "SKILLSETTRIGGERTEMPLATE.hpp"

class NATIVESCRIPT__EQUIPMENTTRIGGERS__ASSEMBLY1 : public NATIVESCRIPT__EQUIPMENTTRIGGERS__SKILLSETTRIGGERTEMPLATE {
	void onStartup(Entity::GameObject * self) {
		// --conditions for firing the trigger
		// self:SetVar("trigger", {Name="Low Imagination", Stat="IMAGINATION", Operator="LESS", Value=1} )
		// --skill to fire
		// self:SetVar("skillID", 394)
		// --how many items from a given set they must have equipped before the skill fires.
		// self:SetVar("itemsRequired", 4)
		// --ID of the skill set they have to have equipped
		// self:SetVar("skillSet", 2)
	}
};

#endif