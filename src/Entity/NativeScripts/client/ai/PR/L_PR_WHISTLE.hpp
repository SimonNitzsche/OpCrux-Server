#ifndef __ENTITY__NATIVESCRIPTS__CLIENT_AI_PR_L_PR_WHISTLE__
#define __ENTITY__NATIVESCRIPTS__CLIENT_AI_PR_L_PR_WHISTLE__

#include <string>
#include "Entity/NativeScript.hpp"
#include "Entity/GameObject.hpp"

class NATIVESCRIPT__CLIENT_AI_PR_L_PR_WHISTLE : public NativeScript {

	void onStartup(Entity::GameObject* self) {
		
	}

	void onFireEventServerSide(Entity::GameObject* self, GM::FireEventServerSide msg) {
		Logger::log("RECIEVED", StringUtils::to_string(msg.args));
		if (msg.args == u"unlockEmote") {
			Entity::GameObject* player = self->GetZoneInstance()->objectsManager->GetObjectByID(msg.senderID);
			GM::SetEmoteLockState setEmoteLockStateGM;
			setEmoteLockStateGM.bLock = false;
			setEmoteLockStateGM.emoteID = 115;
			GameMessages::Send(player, player->GetObjectID(), setEmoteLockStateGM);

			Database::UnlockEmote(player->GetZoneInstance()->GetDBConnection(), player->GetObjectID(), 115);
		}
	}
};
#endif 