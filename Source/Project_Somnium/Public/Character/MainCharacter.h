

#pragma once

#include "CoreMinimal.h"
#include "Character/MainCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "MainCharacter.generated.h"

class UCapsuleComponent;
class UArrowComponent;
class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class PROJECT_SOMNIUM_API AMainCharacter : public AMainCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:

	AMainCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** Player Interface */
	virtual void AddToXP_Implementation(int32 InXP) override;
	/** End Player Interface */

	/** Combat Interface */
	virtual int32 GetPlayerLevel() const override;
	/** End Combat Interface */

private:
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;
	
};
