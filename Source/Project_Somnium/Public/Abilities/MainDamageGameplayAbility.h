

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

public:

	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

	float GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType);
};
