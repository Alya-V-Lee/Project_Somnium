

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacterBase.generated.h"

UCLASS(Abstract)
class PROJECT_SOMNIUM_API AMainCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

};
