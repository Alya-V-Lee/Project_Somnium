

#pragma once

#include "CoreMinimal.h"
#include "Abilities/MainProjectileSpell.h"
#include "MainFirebolt.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_SOMNIUM_API UMainFirebolt : public UMainProjectileSpell
{
	GENERATED_BODY()

public:

	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	
};
