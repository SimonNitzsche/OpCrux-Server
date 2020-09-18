#ifndef __ENTITY__GMUTILS_HPP__ 
#define __ENTITY__GMUTILS_HPP__


#define GM_DESERIALIZE_SWITCH_CASE(name)\
	case name::GetID(): {name msg = name(); msg.Deserialize(bs); msg.TriggerEvent(senderObject, targetObject); targetObject->On##name(senderObject, msg); break;}

#define GM_VAR_DESERIALIZE_STRING(bs, parameter)  {parameter = StringUtils::readStringFromBitStream<std::uint32_t>(bs);}
#define GM_VAR_DESERIALIZE_WSTRING(bs, parameter) {parameter = StringUtils::readWStringFromBitStream<std::uint32_t>(bs);}
#define GM_VAR_DESERIALIZE(bs, parameter) {bs->Read(parameter);}
#define GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, parameter, defaultVal) {\
	/*Check if bool, because those don't need extra bit*/\
	if(typeid(parameter) == typeid(bool)){\
		GM_VAR_DESERIALIZE(bs,parameter);\
	}\
	else {\
		bool notDefault; bs->Read(notDefault);\
		if(!notDefault) parameter = defaultVal;\
		else GM_VAR_DESERIALIZE(bs, parameter);\
	}\
}

#define GM_VAR_SERIALIZE_STRING(bs, parameter) {bs->Write<std::uint32_t>(parameter.size()); bs->Write(reinterpret_cast<const char*>(parameter.c_str()), parameter.size());}
#define GM_VAR_SERIALIZE_WSTRING(bs, parameter) {bs->Write<std::uint32_t>(parameter.size()); bs->Write(reinterpret_cast<const char*>(parameter.c_str()), parameter.size() * 2);}
#define GM_VAR_SERIALIZE(bs, parameter) {bs->Write(parameter);}
#define GM_VAR_SERIALIZE_LDF(bs, parameter) { auto u16stringval = LDFUtils::PackCollectionToWString(parameter); GM_VAR_SERIALIZE_STRING(bs, u16stringval); if(u16stringval.size() != 0) {bs->Write<std::uint16_t>(0);}}
#define GM_VAR_SERIALIZE_WITH_DEFAULT(bs, parameter, defaultVal) {\
	/*Check if bool, because those don't need extra bit*/\
	if(typeid(parameter) == typeid(bool)){\
		GM_VAR_SERIALIZE(bs,parameter);\
	}\
	else {\
		bool notDefault = !(parameter == defaultVal); bs->Write(notDefault);\
		if(notDefault)\
			GM_VAR_SERIALIZE(bs, parameter);\
	}\
}

#define GM_MAKE_GAMEOBJECT_DECLARE(name) void On##name(Entity::GameObject * sender, GM::name & msg)
#define GM_MAKE_GAMEOBJECT_DEFINE(name) void Entity::GameObject::On##name(Entity::GameObject * sender, GM::name & msg){ Logger::log("WRLD", "Event "+std::string(#name)+" on object LOT #" + std::to_string(LOT)); for (auto i : components) i.second->On##name(sender, msg);}
#define GM_MAKE_COMPONENT_DECLARE(name) virtual void On##name(Entity::GameObject * sender, GM::name & msg) {}

#define GM_MAKE_LIST_CLIENT(mfun) \
	mfun(BuildExitConfirmation);\
	mfun(BuildModeSet);\
	mfun(FireEventServerSide);\
	mfun(HasBeenCollected);\
	mfun(MissionDialogueOK);\
	mfun(ModuleAssemblyQueryData);\
	mfun(NotifyClientFlagChange);\
	mfun(PickupCurrency);\
	mfun(PickupItem);\
	mfun(PlayEmote);\
	mfun(PlayerLoaded);\
	mfun(PopEquippedItemsState);\
	mfun(RebuildCancel);\
	mfun(RequestDie);\
	mfun(RequestLinkedMission);\
	mfun(RequestResurrect);\
	mfun(RequestSmashPlayer);\
	mfun(RequestUse);\
	mfun(StartBuildingWithItem);\
	mfun(StartSkill);\
	mfun(SyncSkill);\
	mfun(ServerTerminateInteraction);\
	mfun(SetBuildMode);\
	mfun(SetFlag);\
	mfun(TerminateInteraction);\
	mfun(SetTooltipFlag);\
	mfun(SetGhostReferencePosition);\
	mfun(ReadyForUpdates);\
	mfun(NotifyServerLevelProcessingComplete);\
	mfun(ToggleGhostReferenceOverride);\
	mfun(ToggleSendingPositionUpdates);\
	mfun(ModularBuildFinish);\
	mfun(ModularAssemblyNIFCompleted);\
	mfun(ParseChatMessage);\
	mfun(RequestPlatformResync);\
	mfun(EquipInventory);\
	mfun(SetJetPackMode);\
	mfun(Teleport);\
	mfun(ModifyGhostingDistance);\
	mfun(CancelRailMovement);\
	mfun(ReportBug);\
	mfun(DoneArrangingWithItem);

// 	mfun(BuildExitConfirmation);
#endif

