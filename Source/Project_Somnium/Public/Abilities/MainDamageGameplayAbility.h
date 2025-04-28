

#pragma once

#include "CoreMinimal.h"
#include "Abilities/MainGameplayAbility.h"
#include "MainDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_SOMNIUM_API UMainDamageGameplayAbility : public UMainGameplayAbility
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
