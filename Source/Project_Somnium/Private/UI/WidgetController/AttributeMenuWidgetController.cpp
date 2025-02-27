


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/MainAttributeSet.h"
#include "MainGameplayTags.h"


void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	//Super::BindCallbacksToDependencies();

	UMainAttributeSet* AS = CastChecked<UMainAttributeSet>(AttributeSet);
	check (AttributeInfo)
	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
	[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	FMainAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	//Super::BroadcastInitialValues();

	UMainAttributeSet* AS = CastChecked<UMainAttributeSet>(AttributeSet);
	check (AttributeInfo)

	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}
