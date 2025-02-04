


#include "Character/MainEnemy.h"

#include "NavigationSystemTypes.h"
#include "AbilitySystem/MainAbilitySystemComponent.h"
#include "AbilitySystem/MainAttributeSet.h"
#include "Project_Somnium/Project_Somnium.h"

AMainEnemy::AMainEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UMainAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UMainAttributeSet>("AttributeSet");
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AMainEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	Weapon->SetRenderCustomDepth(true);
}

void AMainEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

void AMainEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	InitAbilityActorInfo();
}

void AMainEnemy::InitAbilityActorInfo()
{
	//Super::InitAbilityActorInfo();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UMainAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
}
