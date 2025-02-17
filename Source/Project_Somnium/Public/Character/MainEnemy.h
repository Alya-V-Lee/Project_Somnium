

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

	/** Enemy Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/** End Enemy Interface */

	/** Combat Interface **/
	virtual int32 GetPlayerLevel() const override {return Level;}	
	/** End Combat Interface **/

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

private:
	
};
