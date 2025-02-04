

#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/MainAbilitySystemComponent.h"
#include "AbilitySystem/MainAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	//Super::BroadcastInitialValues();

	const UMainAttributeSet* MainAttributeSet = CastChecked<UMainAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(MainAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(MainAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(MainAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(MainAttributeSet->GetMaxMana());
	
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	//Super::BindCallbacksToDependencies();

	const UMainAttributeSet* MainAttributeSet = CastChecked<UMainAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(MainAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(MainAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(MainAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(MainAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChanged);

	Cast<UMainAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);
			}
		}
	);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
