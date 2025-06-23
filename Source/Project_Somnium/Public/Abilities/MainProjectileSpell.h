

#pragma once

#include "CoreMinimal.h"
#include "Abilities/MainDamageGameplayAbility.h"
#include "MainProjectileSpell.generated.h"

class AMainProjectile;
class UGameplayEffect;
struct FGameplayTag;
/**
 * 
 */
UCLASS()
class PROJECT_SOMNIUM_API UMainProjectileSpell : public UMainDamageGameplayAbility
{
	GENERATED_BODY()

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch = false, float PitchOverride = 0.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AMainProjectile> ProjectileClass;

};
