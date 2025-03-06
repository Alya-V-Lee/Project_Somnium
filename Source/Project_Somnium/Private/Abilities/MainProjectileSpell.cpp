


#include "Abilities/MainProjectileSpell.h"

#include "Actor/MainProjectile.h"
#include "Interaction/CombatInterface.h"

void UMainProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const bool bIsServer = HasAuthority(&ActivationInfo);
	if (!bIsServer) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		// TODO: Set projectile rotation

		AMainProjectile* Projectile = GetWorld()->SpawnActorDeferred<AMainProjectile>(
        	ProjectileClass,
        	SpawnTransform,
        	GetOwningActorFromActorInfo(),
        	Cast<APawn>(GetOwningActorFromActorInfo()),
        	ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		//TODO: Give projectile a Gameplay Effect Spec for causing damage
		
		Projectile->FinishSpawning(SpawnTransform);
	}


}
