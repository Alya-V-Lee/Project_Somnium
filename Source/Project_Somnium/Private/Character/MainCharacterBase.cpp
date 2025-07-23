


#include "Character/MainCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "MainGameplayTags.h"
#include "MovieSceneSequence.h"
#include "AbilitySystem/MainAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
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
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
	
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
	FName SocketName;
	if (MontageTag == FMainGameplayTags::Get().CombatSocket_MainHand)
	{
		SocketName = MainHandSocketName;  
	}
	else if (MontageTag == FMainGameplayTags::Get().CombatSocket_OffHand)
	{
		SocketName = OffHandSocketName;   
	}
	else if (MontageTag == FMainGameplayTags::Get().CombatSocket_Tail)
	{
		SocketName = TailSocketName;
	}
	else if (MontageTag == FMainGameplayTags::Get().CombatSocket_MainHand_Equipped || 
			 MontageTag == FMainGameplayTags::Get().CombatSocket_OffHand_Equipped)
	{
		SocketName = WeaponTipSocketName; 
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unknown MontageTag: %s"), *MontageTag.ToString());
		return FVector::ZeroVector;
	}

	if ((MontageTag == FMainGameplayTags::Get().CombatSocket_MainHand_Equipped || 
		 MontageTag == FMainGameplayTags::Get().CombatSocket_OffHand_Equipped) && Weapon)
	{
		return Weapon->GetSocketLocation(SocketName);
	}
	else
	{
		return GetMesh()->GetSocketLocation(SocketName);
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
	MainASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
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
