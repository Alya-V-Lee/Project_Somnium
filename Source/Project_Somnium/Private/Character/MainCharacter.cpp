


#include "Character/MainCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/MainAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Player/MainPlayerController.h"
#include "Player/MainPlayerState.h"
#include "UI/HUD/MainHUD.h"

AMainCharacter::AMainCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator(-30.0f, 90.0f, 0.0f));
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->TargetArmLength = 700.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
}

void AMainCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the server
	InitAbilityActorInfo();
	
}

void AMainCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	// Init ability actor info for the client
	InitAbilityActorInfo();
}

int32 AMainCharacter::GetPlayerLevel() const
{
	const AMainPlayerState* MainPlayerState = GetPlayerState<AMainPlayerState>();
	check (MainPlayerState);

	return MainPlayerState->GetPlayerLevel();
}

void AMainCharacter::InitAbilityActorInfo()
{
	//Super::InitAbilityActorInfo();
	
	AMainPlayerState* MainPlayerState = GetPlayerState<AMainPlayerState>();
	check (MainPlayerState);

	MainPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(MainPlayerState, this);
	Cast<UMainAbilitySystemComponent>(MainPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = MainPlayerState->GetAbilitySystemComponent();
	AttributeSet = MainPlayerState->GetAttributeSet();

	if (AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(GetController()))
	{
		if (AMainHUD* MainHUD = Cast<AMainHUD>(MainPlayerController->GetHUD()))
		{
			MainHUD->InitOverlay(MainPlayerController, MainPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeDefaultAttributes();
}
