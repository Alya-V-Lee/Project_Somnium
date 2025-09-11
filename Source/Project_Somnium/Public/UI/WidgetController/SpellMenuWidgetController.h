

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/MainWidgetController.h"
#include "GameplayTagContainer.h"
#include "MainGameplayTags.h"
#include "SpellMenuWidgetController.generated.h"

struct FMainGameplayTags;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSelectedSignature,
bool, bSpendPointsButtonEnabled, bool, bEquipButtonEnabled,
FString, DescriptionString, FString, NextLevelDescriptionString);

struct FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
};

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

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnPlayerStatChangedSignature SpellPointsChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FSpellGlobeSelectedSignature SpellGlobeSelectedDelegate;

	UFUNCTION(BlueprintCallable, Category="GAS|Attributes")
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);
	
	UFUNCTION(BlueprintCallable, Category="GAS|Attributes")
	void SpendPointButtonPressed();

private:
	
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints,
		bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton);

	FSelectedAbility SelectedAbility = { FMainGameplayTags::Get().Abilities_None, FMainGameplayTags::Get().Abilities_Status_Locked};
	int32 CurrentSpellPoints = 0;
};
