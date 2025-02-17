

#pragma once

#include "CoreMinimal.h"
#include "Character/MainCharacterBase.h"
#include "MainCharacter.generated.h"

class UCapsuleComponent;
class UArrowComponent;
class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class PROJECT_SOMNIUM_API AMainCharacter : public AMainCharacterBase
{
	GENERATED_BODY()

public:

	AMainCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

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
