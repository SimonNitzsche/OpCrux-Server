#ifndef __ENTITY__NATIVESCRIPTS__ScriptComponent_1021_script_name__removed_HPP__
#define __ENTITY__NATIVESCRIPTS__ScriptComponent_1021_script_name__removed_HPP__
#include "Entity/NativeScript.hpp"

class NATIVESCRIPT__ScriptComponent_1021_script_name__removed : public NativeScript {
	void onUse(Entity::GameObject* self, GM::RequestUse msg) {
		
		// Construct flagID, which is zoneID + var["number"]
		std::string zoneID = std::to_string(self->GetZoneInstance()->GetZoneID());
		std::u16string number = self->GetVar<std::u16string>(u"number");
		if (number == u"") number = u"00";
		std::string flagID = zoneID.substr(0,2) + std::string(number.begin(), number.end());

		// Trigger SetFlag
		GM::SetFlag msgSetFlag;
		msgSetFlag.bFlag = true;
		msgSetFlag.iFlagID = std::stoi(flagID);
		msg.user->OnSetFlag(self, &msgSetFlag);

		// Also notify client to remove effect
		GM::FireEventClientSide msgFireEvent;
		msgFireEvent.object = self->GetObjectID();
		msgFireEvent.args = u"achieve";
		GameMessages::Send(msg.user, self->GetObjectID(), msgFireEvent);
	};
};

#endif