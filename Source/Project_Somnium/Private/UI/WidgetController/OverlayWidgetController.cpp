

#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/MainAbilitySystemComponent.h"
#include "AbilitySystem/MainAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/MainPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	//Super::BroadcastInitialValues();

	const UMainAttributeSet* MainAttributeSet = CastChecked<UMainAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(MainAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(MainAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(MainAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(MainAttributeSet->GetMaxMana());
	
}

void UOverlayWidgetController::BindAttributeChange(FGameplayAttribute Attribute,
                                                   FOnAttributeChangedSignature& AttributeData)
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddLambda(
		[this, &AttributeData](const FOnAttributeChangeData& Data)
		{
			AttributeData.Broadcast(Data.NewValue);
		}
	);
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	//Super::BindCallbacksToDependencies();
	AMainPlayerState* MainPlayerState = CastChecked<AMainPlayerState>(PlayerState);
	MainPlayerState->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	
	const UMainAttributeSet* MainAttributeSet = CastChecked<UMainAttributeSet>(AttributeSet);
	
	BindAttributeChange(MainAttributeSet->GetHealthAttribute(), OnHealthChanged);
	BindAttributeChange(MainAttributeSet->GetMaxHealthAttribute(), OnMaxHealthChanged);
	BindAttributeChange(MainAttributeSet->GetManaAttribute(), OnManaChanged);
	BindAttributeChange(MainAttributeSet->GetMaxManaAttribute(), OnMaxManaChanged);

	if (UMainAbilitySystemComponent* MainASC = Cast<UMainAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (MainASC->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(MainASC);
		}
		else
		{
			MainASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);
		}

		MainASC->EffectAssetTags.AddLambda(
        	[this](const FGameplayTagContainer& AssetTags)
        	{
        		for (const FGameplayTag& Tag : AssetTags)
        		{
        			FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
        			if (Tag.MatchesTag(MessageTag))
        			{
        				const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
        				MessageWidgetRow.Broadcast(*Row);
        			}
        		}
        	}
        );
	}
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UMainAbilitySystemComponent* MainAbilitySystemComponent)
{
	if (!MainAbilitySystemComponent->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, MainAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
		{
			FMainAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(MainAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
			Info.InputTag = MainAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		}
	);
	MainAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP) const
{
	const AMainPlayerState* MainPlayerState = CastChecked<AMainPlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = MainPlayerState->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfo, please fill out MainPlayerState Blueprint"));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;

		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}
