


#include "Character/MainEnemy.h"

#include "NavigationSystemTypes.h"
#include "AbilitySystem/MainAbilitySystemComponent.h"
#include "AbilitySystem/MainAbilitySystemLibrary.h"
#include "AbilitySystem/MainAttributeSet.h"
#include "Components/Widget.h"
#include "Components/WidgetComponent.h"
#include "Project_Somnium/Project_Somnium.h"
#include "UI/Widget/MainUserWidget.h"

AMainEnemy::AMainEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UMainAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UMainAttributeSet>("AttributeSet");
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
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

	if (UMainUserWidget* MainUserWidget = Cast<UMainUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		MainUserWidget->SetWidgetController(this);
	}

	if (const UMainAttributeSet* MainAS = CastChecked<UMainAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MainAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MainAS->GetMaxHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		OnHealthChanged.Broadcast(MainAS->GetHealth());
        OnMaxHealthChanged.Broadcast(MainAS->GetMaxHealth());
	}
	
}

void AMainEnemy::InitAbilityActorInfo()
{
	//Super::InitAbilityActorInfo();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UMainAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAttributes();
}

void AMainEnemy::InitializeDefaultAttributes() const
{
	//Super::InitializeDefaultAttributes();

	UMainAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}
