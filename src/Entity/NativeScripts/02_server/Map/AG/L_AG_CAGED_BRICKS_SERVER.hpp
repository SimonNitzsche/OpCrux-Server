#ifndef __ENTITY__NATIVESCRIPTS__02_SERVER__MAP__AG__L_AG_CAGED_BRICKS_SERVER_HPP__
#define __ENTITY__NATIVESCRIPTS__02_SERVER__MAP__AG__L_AG_CAGED_BRICKS_SERVER_HPP__

#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScript.hpp"
#include "Entity/Components/CharacterComponent.hpp";
#include "Entity/Components/InventoryComponent.hpp";
#include <memory>

class NATIVESCRIPT__02_SERVER__MAP__AG__L_AG_CAGED_BRICKS_SERVER : public NativeScript {

	const std::u16string spiderGroup = u"cagedSpider";
	const std::u16string preconID = u"188;189";
	const std::int32_t flagID = 74;
	const std::int32_t invenItem = 14553;

public:

	void onUse(Entity::GameObject* self, GM::RequestUse msg) {
		auto spiderObj = self->GetZoneInstance()->objectsManager->GetObjectsInGroup(spiderGroup, self->GetObjectID(), true);

		for (auto obj : spiderObj) {
			if (obj != nullptr) {
				// tell the spider to show up
				{ GM::FireEventClientSide nmsg; nmsg.args = u"toggle"; nmsg.senderID = msg.user->GetObjectID(); GameMessages::Send(msg.user, obj->GetObjectID(), nmsg); }
				// set the mission player flag
				{GM::SetFlag nmsg; nmsg.iFlagID = flagID; nmsg.bFlag = true; msg.user->CallMessage(nmsg); }
				// Player has completed the mission, remove necessary items
				msg.user->GetComponent<InventoryComponent>()->RemoveItem2(invenItem, 1);
			}
		}

	}

};
#endif 