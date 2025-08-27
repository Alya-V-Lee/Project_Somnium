


#include "UI/WidgetController/MainWidgetController.h"

#include "AbilitySystem/MainAbilitySystemComponent.h"
#include "AbilitySystem/MainAttributeSet.h"
#include "Player/MainPlayerController.h"
#include "Player/MainPlayerState.h"
#include "AbilitySystem/Data/AbilityInfo.h"

void UMainWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UMainWidgetController::BroadcastInitialValues()
{
	
}

void UMainWidgetController::BindCallbacksToDependencies()
{
	
}

void UMainWidgetController::BroadcastAbilityInfo()
{
	if (!GetMainASC()->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
		{
			FMainAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(MainAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
			Info.InputTag = MainAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
			Info.StatusTag = MainAbilitySystemComponent->GetStatusFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		}
	);
	GetMainASC()->ForEachAbility(BroadcastDelegate);
}

AMainPlayerController* UMainWidgetController::GetMainPC()
{
	if (MainPlayerController == nullptr)
	{
		MainPlayerController = Cast<AMainPlayerController>(PlayerController);
	}
	return MainPlayerController;
}

AMainPlayerState* UMainWidgetController::GetMainPS()
{
	if (MainPlayerState == nullptr)
	{
		MainPlayerState = Cast<AMainPlayerState>(PlayerState);
	}
	return MainPlayerState;
}

UMainAbilitySystemComponent* UMainWidgetController::GetMainASC()
{
	if (MainAbilitySystemComponent == nullptr)
	{
		MainAbilitySystemComponent = Cast<UMainAbilitySystemComponent>(AbilitySystemComponent);
	}
	return MainAbilitySystemComponent;
}

UMainAttributeSet* UMainWidgetController::GetMainAS()
{
	if (MainAttributeSet == nullptr)
	{
		MainAttributeSet = Cast<UMainAttributeSet>(AttributeSet);
	}
	return MainAttributeSet;
}
