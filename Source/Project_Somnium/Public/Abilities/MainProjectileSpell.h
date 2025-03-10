

#pragma once

#include "CoreMinimal.h"
#include "Abilities/MainGameplayAbility.h"
#include "MainProjectileSpell.generated.h"

class AMainProjectile;
/**
 * 
 */
UCLASS()
class PROJECT_SOMNIUM_API UMainProjectileSpell : public UMainGameplayAbility
{
	GENERATED_BODY()

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AMainProjectile> ProjectileClass;


};
