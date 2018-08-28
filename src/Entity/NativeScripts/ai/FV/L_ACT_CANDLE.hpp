/*
	DRAFT
*/

class ENS_AI_FV_L_ACT_CANDLE : public EntityNativeScript {
public:
	void onStartup() {
		entity->PlayFXEffect("candle_light", 2108, "create");
		SetVar("Smoke", 5);
		SetVar("AmHit", false);
	}
	
	void onOnHit(struct OnHitMessage msg) {
		if(GetVar("AmHit") == false) {
			msg.attacker->UpdateMissionTask("complete", 850, 1, entity);
			
			SetVar("AmHit", true);
			entity->SetHealth(9999);
			entity->StopFXEffect("candle_light");
			entity->PlayFXEffect("candle_smoke", effectID = 2109, "create");
			
			ObjectManager::GetTimer()->AddTimerWithCancel(GetVar("Smoke"), "SmokeTime", entity); 
		}
	}
	
	void onSkillEventFired(struct SkilledEventFiredMessage msg) {
		if(msg.wsHandle == "waterspray") {
			if(GetVar("AmHit") == false) {
				msg.casterID->UpdateMissionTask("complete", 850, 1, entity);
				SetVar("AmHit", true);
				
				StopFXEffect("candle_light");
				PlayFXEffect("candle_smoke", 2109, "create");
				
				ObjectManager::GetTimer()->AddTimerWithCancel(GetVar("Smoke"), "SmokeTime", entity);
			}
		}
	}
	
	void onTimerDone(struct TimerDoneMessage msg) {
		SetVar("AmHit", false);
		StopFXEffect("candle_smoke");
		PlayFXEffect("candle_light", 2108, "create");
	}
}