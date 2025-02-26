


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/MainAttributeSet.h"
#include "MainGameplayTags.h"


void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	//Super::BindCallbacksToDependencies();
	
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	//Super::BroadcastInitialValues();

	UMainAttributeSet* AS = CastChecked<UMainAttributeSet>(AttributeSet);
	
	check (AttributeInfo)

	for (auto& Pair : AS->TagsToAttributes)
	{
		FMainAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
		Info.AttributeValue = Pair.Value().GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(Info);
	}
}