

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * MainGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */

struct FMainGameplayTags
{
public:
	
	static const FMainGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Constitution;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;

	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
    FGameplayTag InputTag_4;
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;

	FGameplayTag Damage;
	FGameplayTag Damage_Physical;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Cold;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Force;
	
	FGameplayTag Attributes_Resistance_Physical;
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Cold;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Force;

	FGameplayTag Attributes_Meta_IncomingXP;

	FGameplayTag Abilities_None;
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	
	FGameplayTag Abilities_HitReact;
	
	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;

	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;
	
	FGameplayTag Abilities_Fire_Firebolt;
	FGameplayTag Cooldown_Fire_Firebolt;
	FGameplayTag Abilities_Lightning_Electrocute;
	
	FGameplayTag CombatSocket_MainHand;
	FGameplayTag CombatSocket_MainHand_Equipped;
	FGameplayTag CombatSocket_OffHand;
	FGameplayTag CombatSocket_OffHand_Equipped;
	FGameplayTag CombatSocket_Tail;

	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;
	
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	TMap<FGameplayTag, FName> CombatSockets;

	FGameplayTag Effects_HitReact;
protected:
	
private:
	
	static FMainGameplayTags GameplayTags;
};
