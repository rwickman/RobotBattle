// Fill out your copyright notice in the Description page of Project Settings.

#include "Katana.h"
#include "BaseFightingCharacter.h"

// Sets default values
AKatana::AKatana()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//RootComponent = katanaMesh;//->SetupAttachment(RootComponent);
	//katanaMesh->SetupAttachment(RootComponent);
	

	// Create katana mesh
	katanaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KatanaMesh"));
	RootComponent = katanaMesh;
	//katanaMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> katanaVisualAsset(TEXT("/Game/Meshes/Weapons/Katana/Katana"));
	if (katanaVisualAsset.Succeeded())
	{
		katanaMesh->SetStaticMesh(katanaVisualAsset.Object);
		katanaMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		katanaMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	}

	// Create box collision
	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	boxCollision->SetBoxExtent(FVector(9.0f, 50.0f, 400.0f), false);
	boxCollision->SetRelativeLocation(FVector(0.0f, 25.0f, 440.0f));
	boxCollision->SetupAttachment(RootComponent);
	boxCollision->OnComponentBeginOverlap.AddDynamic(this, &AKatana::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AKatana::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AKatana::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::SanitizeFloat(TimeSinceAttack));
	if (TimeSinceAttack < DPS)
	{
		TimeSinceAttack += DeltaTime;
	}
}

void AKatana::SetIsOwnerAttacking(bool attacking)
{
	IsOwnerAttacking = attacking;
}

void AKatana::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// TODO: Restrict this to only allow for certian DPS
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("Overlap Begin"));
	if (OtherActor && (OtherActor != this) && (OtherActor != GetOwner()) && OtherComp && CanAttack() && !HurtEnemies.Contains(OtherComp->GetUniqueID()))
	{
		ABaseFightingCharacter* FightCharacter = Cast<ABaseFightingCharacter>(OtherActor);
		if (FightCharacter)
		{
			HurtEnemies.Add(OtherComp->GetUniqueID());
			FightCharacter->ApplyDamage(damagePoints);
		}
	}
}

bool AKatana::CanAttack()
{
	if (IsOwnerAttacking && TimeSinceAttack >= DPS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AKatana::ResetAttack()
{
	TimeSinceAttack = 0.0f;
	HurtEnemies.Empty();
}