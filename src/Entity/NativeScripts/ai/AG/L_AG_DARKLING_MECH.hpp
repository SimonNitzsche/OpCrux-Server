#ifndef __ENTITY__NATIVESCRIPTS__AI__AG__L_AG_DARKLING_MECH_HPP__
#define __ENTITY__NATIVESCRIPTS__AI__AG__L_AG_DARKLING_MECH_HPP__

#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScript.hpp"

class NATIVESCRIPT__AI__AG__L_AG_DARKLING_MECH : public NativeScript {
	const std::int32_t QuickbuildTurretLOT = 6254;
public:

	void onDie(Entity::GameObject* self, GM::Die msg) {
		Entity::GameObject* turret = new Entity::GameObject(self->GetZoneInstance(), QuickbuildTurretLOT);
		turret->SetObjectID(DataTypes::LWOOBJID((1ULL << 58) + 104120439353844ULL + self->GetZoneInstance()->spawnedObjectIDCounter++));

		auto mypos = self->GetPosition();
		auto posString = StringUtils::to_u16string(std::to_string(mypos.x)) + char16_t(0x001f) +
						 StringUtils::to_u16string(std::to_string(mypos.y)) + char16_t(0x001f) +
						 StringUtils::to_u16string(std::to_string(mypos.z));
		auto myrot = self->GetRotation();

		turret->SetPosition(mypos);
		turret->SetRotation(myrot);

		LDFCollection config = {
			LDF_COLLECTION_INIT_ENTRY(u"rebuild_activators", posString),
			LDF_COLLECTION_INIT_ENTRY(u"respawn", 100000.f),
			LDF_COLLECTION_INIT_ENTRY(u"rebuild_reset_time", -1.f),
			LDF_COLLECTION_INIT_ENTRY(u"no_timed_spawn", true),
			LDF_COLLECTION_INIT_ENTRY(u"currentTime", 0),
			LDF_COLLECTION_INIT_ENTRY(u"CheckPrecondition", u"21")
		};
		turret->PopulateFromLDF(&config);

		turret->Finish();
		self->GetZoneInstance()->objectsManager->RegisterObject(turret);
		self->GetZoneInstance()->objectsManager->Construct(turret);
	}
};

#endif 