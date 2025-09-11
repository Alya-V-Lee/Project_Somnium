

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "MainPlayerState.generated.h"

class ULevelUpInfo;
class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /*StatValue*/);

/**
 * 
 */
UCLASS()
class PROJECT_SOMNIUM_API AMainPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AMainPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;
	
	FOnPlayerStatChanged OnLevelChangedDelegate;
	FOnPlayerStatChanged OnXPChangedDelegate;
	FOnPlayerStatChanged OnAttributePointsChangedDelegate;
	FOnPlayerStatChanged OnSpellPointsChangedDelegate;

	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetXP() const { return XP; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }
	
	void SetLevel (int32 InLevel) {Level = InLevel; OnLevelChangedDelegate.Broadcast(Level);};
	void SetXP(int32 InXP) {XP = InXP; OnXPChangedDelegate.Broadcast(XP);};
	void SetAttributePoints(int32 InAttributePoints) {AttributePoints = InAttributePoints; OnAttributePointsChangedDelegate.Broadcast(AttributePoints);};
	void SetSpellPoints(int32 InSpellPoints) {SpellPoints = InSpellPoints; OnSpellPointsChangedDelegate.Broadcast(SpellPoints);};
	
	void AddToLevel (int32 InLevel) {Level += InLevel; OnLevelChangedDelegate.Broadcast(Level);};
	void AddToXP(int32 InXP) {XP += InXP; OnXPChangedDelegate.Broadcast(XP);};
	void AddToAttributePoints (int32 InAttributePoints) {AttributePoints += InAttributePoints; OnAttributePointsChangedDelegate.Broadcast(AttributePoints);};
	void AddToSpellPoints (int32 InSpellPoints) {SpellPoints += InSpellPoints; OnSpellPointsChangedDelegate.Broadcast(SpellPoints);};

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:

	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;

	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_XP)
	int32 XP = 0;

	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints = 0;

	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_SpellPoints)
	int32 SpellPoints = 0;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel) {OnLevelChangedDelegate.Broadcast(Level);}

	UFUNCTION()
	void OnRep_XP(int32 OldXP) {OnXPChangedDelegate.Broadcast(XP);};

	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints) {OnAttributePointsChangedDelegate.Broadcast(AttributePoints);};

	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints) {OnSpellPointsChangedDelegate.Broadcast(SpellPoints);};
};
