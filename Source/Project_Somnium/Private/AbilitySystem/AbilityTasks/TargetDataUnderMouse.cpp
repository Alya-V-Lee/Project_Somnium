


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "Player/MainPlayerController.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	//Super::Activate();
	
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	//PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	AMainPlayerController* MainPC = Cast<AMainPlayerController>(PC);
	if (MainPC)
	{
		FHitResult CursorHit = MainPC->GetCursorHit();
		ValidData.Broadcast(CursorHit.Location);
	}
}
