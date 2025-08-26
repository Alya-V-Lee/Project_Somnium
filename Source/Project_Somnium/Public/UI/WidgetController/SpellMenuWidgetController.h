

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/MainWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PROJECT_SOMNIUM_API USpellMenuWidgetController : public UMainWidgetController
{
	GENERATED_BODY()

public:

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
};
