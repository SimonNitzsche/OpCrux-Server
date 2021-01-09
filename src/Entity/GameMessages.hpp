#ifndef __ENTITY__GameMessages_HPP__
#define __ENTITY__GameMessages_HPP__
#include "Sessions/ClientSession.hpp"
#include <RakNet/BitStream.h>
#include <RakNet/RakNetTypes.h>
#include "DataTypes/LWOOBJID.hpp"
#include "Enums/EGameMessages.hpp"
#include "Server/WorldServer.hpp"

#include "Enums/EDisconnectReason.hpp"
#include "Enums/ERemoteConnection.hpp"
#include "Enums/EPackets.hpp"
#include "Enums/EInventoryType.hpp"
#include "Enums/EItemType.hpp"

#include "Utils/StringUtils.hpp"

#include <string>

#include "Entity/GameObject.hpp"
#include "Utils/LDFUtils.hpp"

#include "Entity/GMBase.hpp"
#include "Entity/GMUtils.hpp"

class GameMessages {
public:
	static void Deserialize(WorldServer * Instance, ClientSession * session, RakNet::BitStream *bs);

	template<typename T = GM::GMBase>
	static void Send(WorldServer * Instance, SystemAddress systemAddress, DataTypes::LWOOBJID target, T gm, DataTypes::LWOOBJID exclude = 0ULL) {
		RakNet::BitStream bs = RakNet::BitStream();

		LUPacketHeader returnBSHead;
		returnBSHead.protocolID = static_cast<uint8_t>(ID_USER_PACKET_ENUM);
		returnBSHead.remoteType = static_cast<uint16_t>(Enums::ERemoteConnection::CLIENT);
		returnBSHead.packetID = static_cast<uint32_t>(Enums::EClientPacketID::SERVER_GAME_MSG);
		bs.Write(returnBSHead);

		// GM Header
		bs.Write<std::uint64_t>(target);
		bs.Write(T::GetID());

		gm.Serialize(&bs);

		if (systemAddress == UNASSIGNED_SYSTEM_ADDRESS) {
			auto clients = Instance->sessionManager.GetClients();
			for (int i = 0; i < clients.size(); ++i) {
				ClientSession session = clients.at(i);

				if (exclude != session.actorID) {
#define quote(x) #x
					if (Instance->objectsManager->GetObjectByID(session.actorID) == nullptr) continue;
					std::string message = "Broadcasting ";
					message += typeid(T).name();
					message += " to ";
					message += Instance->objectsManager->GetObjectByID(session.actorID)->GetNameStr();
					Logger::log("WRLD", message);
					Instance->rakServer->Send(&bs, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, session.systemAddress, false);
				}
			}
		}
		else
			Instance->rakServer->Send(&bs, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, systemAddress, false);
	}

	template<typename T = GM::GMBase>
	static void Send(Entity::GameObject * playerReceiver, DataTypes::LWOOBJID sender, T gm, DataTypes::LWOOBJID exclude = 0ULL) {
		ClientSession * session = playerReceiver->GetZoneInstance()->sessionManager.GetSession(playerReceiver->GetObjectID());
		if (session == nullptr) return;
		Send(playerReceiver->GetZoneInstance(), session->systemAddress, sender, gm);
	}

	

	template<typename T = GM::GMBase>
	static inline void Broadcast(WorldServer * Instance, DataTypes::LWOOBJID target, T gm) {
		Send(Instance, UNASSIGNED_SYSTEM_ADDRESS, target, gm);
	}

	template<typename T = GM::GMBase>
	static inline void Broadcast(WorldServer * Instance, Entity::GameObject * target, T gm) {
		Send(Instance, UNASSIGNED_SYSTEM_ADDRESS, target->GetObjectID(), gm);
	}

	template<typename T = GM::GMBase>
	static inline void Broadcast(Entity::GameObject* target, T gm, bool excludeSelf = false) {
		Send(target->GetZoneInstance(), UNASSIGNED_SYSTEM_ADDRESS, target->GetObjectID(), gm, excludeSelf ? target->GetObjectID() : DataTypes::LWOOBJID(0ULL));
	}
};

// The Game Messages:

// Define by include
#include "Entity/GameMessages/ActivateNDAudioMusicCue.hpp"
#include "Entity/GameMessages/ActivityStart.hpp"
#include "Entity/GameMessages/AddItemToInventoryClientSync.hpp"
#include "Entity/GameMessages/AddSkill.hpp"
#include "Entity/GameMessages/ArrivedAtDesiredWaypoint.hpp"
#include "Entity/GameMessages/BuildExitConfirmation.hpp"
#include "Entity/GameMessages/BuildModeSet.hpp"
#include "Entity/GameMessages/BuyFromVendor.hpp"
#include "Entity/GameMessages/CancelRailMovement.hpp"
#include "Entity/GameMessages/CastSkill.hpp"
#include "Entity/GameMessages/ClientItemConsumed.hpp"
#include "Entity/GameMessages/DeactivateNDAudioMusicCue.hpp"
#include "Entity/GameMessages/Die.hpp"
#include "Entity/GameMessages/DoneArrangingWithItem.hpp"
#include "Entity/GameMessages/DropClientLoot.hpp"
#include "Entity/GameMessages/EchoStartSkill.hpp"
#include "Entity/GameMessages/EchoSyncSkill.hpp"
#include "Entity/GameMessages/EmotePlayed.hpp"
#include "Entity/GameMessages/EnableRebuild.hpp"
#include "Entity/GameMessages/EquipInventory.hpp"
#include "Entity/GameMessages/FactionTriggerItemEquipped.hpp"
#include "Entity/GameMessages/FactionTriggerItemUnEquipped.hpp"
#include "Entity/GameMessages/FireEventClientSide.hpp"
#include "Entity/GameMessages/FireEventServerSide.hpp"
#include "Entity/GameMessages/GoToWaypoint.hpp"
#include "Entity/GameMessages/HasBeenCollected.hpp"
#include "Entity/GameMessages/ItemEquipped.hpp"
#include "Entity/GameMessages/Knockback.hpp"
#include "Entity/GameMessages/MatchRequest.hpp"
#include "Entity/GameMessages/MatchResponse.hpp"
#include "Entity/GameMessages/MatchUpdate.hpp"
#include "Entity/GameMessages/MissionDialogueOK.hpp"
#include "Entity/GameMessages/ModifyGhostingDistance.hpp"
#include "Entity/GameMessages/ModifyLegoScore.hpp"
#include "Entity/GameMessages/ModularBuildFinish.hpp"
#include "Entity/GameMessages/ModuleAssemblyDBDataForClient.hpp"
#include "Entity/GameMessages/ModularAssemblyNIFCompleted.hpp"
#include "Entity/GameMessages/ModuleAssemblyQueryData.hpp"
#include "Entity/GameMessages/MoveItemBetweenInventoryTypes.hpp"
#include "Entity/GameMessages/MoveItemInInventory.hpp"
#include "Entity/GameMessages/NotifyClientFlagChange.hpp"
#include "Entity/GameMessages/NotifyClientObject.hpp"
#include "Entity/GameMessages/NotifyMission.hpp"
#include "Entity/GameMessages/NotifyMissionTask.hpp"
#include "Entity/GameMessages/NotifyRewardMailed.hpp"
#include "Entity/GameMessages/NotifyServerLevelProcessingComplete.hpp"
#include "Entity/GameMessages/NotifyVehicleOfRacingObject.hpp"
#include "Entity/GameMessages/OfferMission.hpp"
#include "Entity/GameMessages/ParseChatMessage.hpp"
#include "Entity/GameMessages/PickupCurrency.hpp"
#include "Entity/GameMessages/PickupItem.hpp"
#include "Entity/GameMessages/PlatfromResync.hpp"
#include "Entity/GameMessages/PlayAnimation.hpp"
#include "Entity/GameMessages/PlayCinematic.hpp"
#include "Entity/GameMessages/PlayEmbeddedEffectOnAllClientsNearObject.hpp"
#include "Entity/GameMessages/PlayEmote.hpp"
#include "Entity/GameMessages/PlayerLoaded.hpp"
#include "Entity/GameMessages/PlayerReady.hpp"
#include "Entity/GameMessages/PlayerSetCameraCyclingMode.hpp"
#include "Entity/GameMessages/PlayFXEffect.hpp"
#include "Entity/GameMessages/PlayNDAudioEmitter.hpp"
#include "Entity/GameMessages/ProximityUpdate.hpp"
#include "Entity/GameMessages/PopEquippedItemsState.hpp"
#include "Entity/GameMessages/RacingPlayerLoaded.hpp"
#include "Entity/GameMessages/ReadyForUpdates.hpp"
#include "Entity/GameMessages/RebuildCancel.hpp"
#include "Entity/GameMessages/RebuildNotifyState.hpp"
#include "Entity/GameMessages/ReportBug.hpp"
#include "Entity/GameMessages/RemoveItemFromInventory.hpp"
#include "Entity/GameMessages/RemoveSkill.hpp"
#include "Entity/GameMessages/RequestDie.hpp"
#include "Entity/GameMessages/RequestLinkedMission.hpp"
#include "Entity/GameMessages/RequestResurrect.hpp"
#include "Entity/GameMessages/RequestSmashPlayer.hpp"
#include "Entity/GameMessages/RequestUse.hpp"
#include "Entity/GameMessages/RespondToMission.hpp"
#include "Entity/GameMessages/RestoreToPostLoadStats.hpp"
#include "Entity/GameMessages/RequestPlatformResync.hpp"
#include "Entity/GameMessages/Resurrect.hpp"
#include "Entity/GameMessages/ScriptNetworkVarUpdate.hpp"
#include "Entity/GameMessages/ServerTerminateInteraction.hpp"
#include "Entity/GameMessages/SetBuildMode.hpp"
#include "Entity/GameMessages/SetCurrency.hpp"
#include "Entity/GameMessages/SetEmoteLockState.hpp"
#include "Entity/GameMessages/SetGhostReferencePosition.hpp"
#include "Entity/GameMessages/SetInventorySize.hpp"
#include "Entity/GameMessages/SetJetPackMode.hpp"
#include "Entity/GameMessages/SetFlag.hpp"
#include "Entity/GameMessages/SetGravityScale.hpp"
#include "Entity/GameMessages/SetPlayerControlScheme.hpp"
#include "Entity/GameMessages/SetTooltipFlag.hpp"
#include "Entity/GameMessages/StartArrangingWithItem.hpp"
#include "Entity/GameMessages/StartBuildingWithItem.hpp"
#include "Entity/GameMessages/StartCelebrationEffect.hpp"
#include "Entity/GameMessages/StartSkill.hpp"
#include "Entity/GameMessages/StopFXEffect.hpp"
#include "Entity/GameMessages/StopPathing.hpp"
#include "Entity/GameMessages/SyncSkill.hpp"
#include "Entity/GameMessages/TerminateInteraction.hpp"
#include "Entity/GameMessages/Teleport.hpp"
#include "Entity/GameMessages/TimerDone.hpp"
#include "Entity/GameMessages/ToggleGhostReferenceOverride.hpp"
#include "Entity/GameMessages/ToggleSendingPositionUpdates.hpp"
#include "Entity/GameMessages/TransferToZone.hpp"
#include "Entity/GameMessages/TransferToZoneCheckedIM.hpp"
#include "Entity/GameMessages/UnEquipInventory.hpp"
#include "Entity/GameMessages/UpdatePlayerStatistic.hpp"
#include "Entity/GameMessages/UpdateReputation.hpp"
#include "Entity/GameMessages/UseItemResult.hpp"
#include "Entity/GameMessages/UseNonEquipmentItem.hpp"
#include "Entity/GameMessages/VehicleUnlockInput.hpp"
#include "Entity/GameMessages/VendorOpenWindow.hpp"
#include "Entity/GameMessages/VendorStatusUpdate.hpp"
#include "Entity/GameMessages/VendorTransactionResult.hpp"

#endif
