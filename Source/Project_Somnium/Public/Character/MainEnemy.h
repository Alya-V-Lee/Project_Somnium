

#pragma once

#include "CoreMinimal.h"
#include "Character/MainCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "MainEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_SOMNIUM_API AMainEnemy : public AMainCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
public:
	AMainEnemy();
	
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

};
