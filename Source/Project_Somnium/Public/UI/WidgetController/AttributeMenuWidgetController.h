

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/MainWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_SOMNIUM_API UAttributeMenuWidgetController : public UMainWidgetController
{
	GENERATED_BODY()

public:

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
};
