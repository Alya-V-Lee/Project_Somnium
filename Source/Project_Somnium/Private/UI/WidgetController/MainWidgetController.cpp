


#include "UI/WidgetController/MainWidgetController.h"

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
