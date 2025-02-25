


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

	FMainAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(FMainGameplayTags::Get().Attributes_Primary_Strength);
	Info.AttributeValue = AS->GetStrength();
	
	AttributeInfoDelegate.Broadcast(Info);
}