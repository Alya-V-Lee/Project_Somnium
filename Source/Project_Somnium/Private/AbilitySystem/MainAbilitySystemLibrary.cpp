


#include "AbilitySystem/MainAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "MainAbilityTypes.h"
#include "MainGameplayTags.h"
#include "Game/MainGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MainPlayerState.h"
#include "UI/HUD/MainHUD.h"
#include "UI/WidgetController/MainWidgetController.h"

bool UMainAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AMainHUD*& OutMainHUD)
{
	if (APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		OutMainHUD = Cast<AMainHUD>(PC->GetHUD());
		if (AMainHUD* MainHUD = OutMainHUD)
		{
			AMainPlayerState* PS = PC->GetPlayerState<AMainPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			
			OutWCParams.AttributeSet = AS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.PlayerState = PS;
			OutWCParams.PlayerController = PC;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UMainAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	//APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	FWidgetControllerParams WCParams;
	AMainHUD* MainHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, MainHUD))
	{
		return MainHUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UMainAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	//APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	FWidgetControllerParams WCParams;
	AMainHUD* MainHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, MainHUD))
	{
		return MainHUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}

USpellMenuWidgetController* UMainAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AMainHUD* MainHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, MainHUD))
	{
		return MainHUD->GetSpellMenuWidgetController(WCParams);
	}
	return nullptr;
}

void UMainAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);


	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());


	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());


	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UMainAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;
	
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

int32 UMainAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;

	const FCharacterClassDefaultInfo& Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}


UCharacterClassInfo* UMainAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const AMainGameModeBase* MainGameMode = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (MainGameMode == nullptr) return nullptr;

	return MainGameMode->CharacterClassInfo;
}

UAbilityInfo* UMainAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const AMainGameModeBase* MainGameMode = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (MainGameMode == nullptr) return nullptr;

	return MainGameMode->AbilityInfo;
}

bool UMainAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMainGameplayEffectContext* MainEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MainEffectContext->IsBlockedHit();
	}
	return false;
}

bool UMainAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMainGameplayEffectContext* MainEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MainEffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float UMainAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMainGameplayEffectContext* MainEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MainEffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float UMainAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMainGameplayEffectContext* MainEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MainEffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float UMainAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMainGameplayEffectContext* MainEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MainEffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag UMainAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMainGameplayEffectContext* MainEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (MainEffectContext->GetDamageType().IsValid())
		{
			return *MainEffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

bool UMainAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMainGameplayEffectContext* MainEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MainEffectContext->IsCriticalHit();
	}
	return false;
}

void UMainAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FMainGameplayEffectContext* MainEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		MainEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UMainAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsCriticalHit)
{
	if (FMainGameplayEffectContext* MainEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		MainEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UMainAbilitySystemLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInSuccessfulDebuff)
{
	if (FMainGameplayEffectContext* MainEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		MainEffectContext->SetIsSuccessfulDebuff(bInSuccessfulDebuff);
	}
}

void UMainAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float bInDamage)
{
	if (FMainGameplayEffectContext* MainEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		MainEffectContext->SetDebuffDamage(bInDamage);
	}
}

void UMainAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, float bInDuration)
{
	if (FMainGameplayEffectContext* MainEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		MainEffectContext->SetDebuffDuration(bInDuration);
	}
}

void UMainAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle,
	float bInFrequency)
{
	if (FMainGameplayEffectContext* MainEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		MainEffectContext->SetDebuffFrequency(bInFrequency);
	}
}

void UMainAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle,
	const FGameplayTag& InDamageType)
{
	if (FMainGameplayEffectContext* MainEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		MainEffectContext->SetDamageType(DamageType);
	}
}

void UMainAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
                                                           TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
                                                           const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			const bool ImplementsCombatInterface = Overlap.GetActor()->Implements<UCombatInterface>();
			if (ImplementsCombatInterface)
			{
				const bool IsAlive = !ICombatInterface::Execute_IsDead(Overlap.GetActor());
				if (IsAlive)
				{
					OutOverlappingActors.AddUnique(Overlap.GetActor());
				}
			}
		}
	}
}

bool UMainAbilitySystemLibrary::IsFriendly(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriendly = bBothArePlayers || bBothAreEnemies;
	return bFriendly;
}

FGameplayEffectContextHandle UMainAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FMainGameplayTags& GameplayTags = FMainGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);

	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(
		DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, GameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, GameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);
	
	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

	return EffectContextHandle;
}
