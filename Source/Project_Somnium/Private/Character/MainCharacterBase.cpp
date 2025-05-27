


#include "Character/MainCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "MainGameplayTags.h"
#include "MovieSceneSequence.h"
#include "AbilitySystem/MainAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Project_Somnium/Project_Somnium.h"

// Sets default values
AMainCharacterBase::AMainCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

UAbilitySystemComponent* AMainCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* AMainCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AMainCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath_Implementation();
}

void AMainCharacterBase::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
	bDead = true;
}

// Called when the game starts or when spawned
void AMainCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector AMainCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	// Get the socket name from the map using the MontageTag
	const FName* SocketNamePtr = FMainGameplayTags::Get().CombatSockets.Find(MontageTag);

	if (SocketNamePtr)
	{
		const FName SocketName = *SocketNamePtr;

		// If the tag is for an equipped weapon and a weapon exists, get the socket location from the weapon
		if ((MontageTag == FMainGameplayTags::Get().CombatSocket_MainHand_Equipped || MontageTag == FMainGameplayTags::Get().CombatSocket_OffHand_Equipped) && Weapon)
		{
			return Weapon->GetSocketLocation(SocketName);
		}
		else
		{
			// Otherwise, get the socket location from the character's mesh
			return GetMesh()->GetSocketLocation(SocketName);
		}
	}
	else
	{
		// If the tag isn't found, log a warning and return the actor's location as a fallback
		UE_LOG(LogTemp, Warning, TEXT("MontageTag %s not found in MontageSocketLocations map"), *MontageTag.ToString());
		return FVector();
	}
}

bool AMainCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* AMainCharacterBase::GetAvatar_Implementation()
{
	return this;
}

FTaggedMontage AMainCharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (FTaggedMontage TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTag == MontageTag)
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

void AMainCharacterBase::InitAbilityActorInfo()
{
	
}

void AMainCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level,ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AMainCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void AMainCharacterBase::AddCharacterAbilities()
{
	UMainAbilitySystemComponent* MainASC = CastChecked<UMainAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	MainASC->AddCharacterAbilities(StartupAbilities);
}

void AMainCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatInst);
		StartDissolveTimeline(DynamicMatInst);
	}
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicMatInst);
		StartWeaponDissolveTimeline(DynamicMatInst);
	}
}
