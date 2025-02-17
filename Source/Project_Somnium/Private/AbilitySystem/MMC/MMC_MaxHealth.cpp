


#include "AbilitySystem/MMC/MMC_MaxHealth.h"

#include "AbilitySystem/MainAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	ConstitutionDef.AttributeToCapture = UMainAttributeSet::GetConstitutionAttribute();
	ConstitutionDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	ConstitutionDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(ConstitutionDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Constitution = 0.f;
	GetCapturedAttributeMagnitude(ConstitutionDef, Spec, EvaluationParameters, Constitution);
	Constitution = FMath::Max<float>(Constitution, 0.0f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 80.f + 2.5f * Constitution + 10.f * PlayerLevel;
}
