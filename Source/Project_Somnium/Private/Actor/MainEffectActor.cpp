


#include "Actor/MainEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "NetworkMessage.h"
#include "AbilitySystem/MainAttributeSet.h"

// Sets default values
AMainEffectActor::AMainEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	
	Sphere = CreateDefaultSubobject<USphereComponent>(FName("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
	
}

void AMainEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyState, bool bFromSweep, const FHitResult& SweepResult)
{
	//Todo: Change this to apply some Gameplay Effect. For now, hacking through const with const_cast
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UMainAttributeSet* MainAttributeSet = Cast<UMainAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UMainAttributeSet::StaticClass()));

		UMainAttributeSet* MutableMainAttributeSet = const_cast<UMainAttributeSet*>(MainAttributeSet);
		MutableMainAttributeSet->SetHealth(MainAttributeSet->GetHealth() + 25.f);
		MutableMainAttributeSet->SetMana(MainAttributeSet->GetMana() + 25.f);
		Destroy();
	}
}

void AMainEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

// Called when the game starts or when spawned
void AMainEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AMainEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AMainEffectActor::EndOverlap);
	
}

