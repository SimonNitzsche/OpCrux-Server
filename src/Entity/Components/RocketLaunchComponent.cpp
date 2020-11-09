#include "RocketLaunchComponent.hpp"
#include <Utils\LDFUtils.hpp>
#include "Entity/GameMessages.hpp"

void RocketLaunchComponent::OnRequestUse(Entity::GameObject* sender, GM::RequestUse& msg) {

	GM::FireEventClientSide fecs;
	fecs.args = u"RocketEquipped";
	fecs.senderID = sender->GetObjectID();
	fecs.object = 1152921504606847016;

	GameMessages::Broadcast(owner, fecs);


	GM::TransferToZone ttz;
	ttz.bCheckTransferAllowed = false;
	ttz.spawnPoint = targetScene;
	ttz.zoneID = targetZone;

	// GameMessages::Send(sender, sender->GetObjectID(), ttz);

	GM::TransferToZoneCheckedIM ttzcim;
	ttzcim.bIsThereaQueue = false;
	ttzcim.spawnPoint = targetScene;
	ttzcim.zoneID = targetZone;

	// GameMessages::Send(sender, sender->GetObjectID(), ttzcim);

}

void RocketLaunchComponent::OnFireEventServerSide(Entity::GameObject* sender, GM::FireEventServerSide& msg) {
	if (msg.args == u"ZonePlayer") {
		Entity::GameObject* senderObject = sender->GetZoneInstance()->objectsManager->GetObjectByID(msg.senderID);

		if (senderObject == nullptr) return;

		ClientSession* clSession = sender->GetZoneInstance()->sessionManager.GetSession(sender->GetObjectID());

		if (clSession == nullptr) return;

		DataTypes::ZoneInfo zi;
		zi.zoneID = msg.param3;
		masterServerBridge->ClientRequestMapChange(*clSession, zi);
	}
}

void RocketLaunchComponent::PopulateFromLDF(LDFCollection* collection) {
	LDF_GET_VAL_FROM_COLLECTION(targetScene, collection, u"targetScene", u"")
	LDF_GET_VAL_FROM_COLLECTION(targetZone, collection, u"targetZone", 1100)
	LDF_GET_VAL_FROM_COLLECTION(transferZoneID, collection, u"transferZoneID", u"")
}