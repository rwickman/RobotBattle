// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseFightingCharacter.h"

#include "Engine/EngineTypes.h"



// Sets default values
ABaseFightingCharacter::ABaseFightingCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	healthManager = CreateDefaultSubobject<UHealthManagerComponent>(TEXT("HealthWidgetComponent"));
	healthManager->SetupAttachment(RootComponent);
	rHandSocket = GetMesh()->GetSocketByName(TEXT("hand_rSocket"));
}

// Called when the game starts or when spawned
void ABaseFightingCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	FVector socketLoc = GetMesh()->GetSocketLocation(TEXT("hand_rSocket"));
	FRotator socketRot = GetMesh()->GetSocketRotation(TEXT("hand_rSocket"));
	UWorld* World = GetWorld();
	if (KatanaClass && World)
	{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			katana = World->SpawnActor<AKatana>(KatanaClass, socketLoc, socketRot, SpawnParams);
			katana->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "hand_rSocket");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("NO KatanaClass!"));
	}
}

// Called every frame
void ABaseFightingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void ABaseFightingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ABaseFightingCharacter::TakeDamage(float damage)
{
	bool died = healthManager->TakeDamage(damage);
	if (died)
	{
		Die();
	}
	// TODO: Should die after sends boolean that it is dying
	return died;
}

float ABaseFightingCharacter::GetHealthPoints()
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString("HEALTH PERCENT: ") + FString::SanitizeFloat(healthManager->GetHealthPercentage()));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString("INSIDE BASE CHARACTER: ") + FString::SanitizeFloat(healthManager->GetHealthPoints()));
	}
	return healthManager->GetHealthPoints();
}

void ABaseFightingCharacter::SetIsAttacking(bool attacking)
{
	IsAttacking = attacking;
	katana->SetIsOwnerAttacking(attacking);
}

void ABaseFightingCharacter::Die()
{
	if (DeadCallback)
	{
		DeadCallback(this);
	}
	DestroyCharacter();
}

void ABaseFightingCharacter::DestroyCharacter()
{
	katana->Destroy();
	Destroy();
}

bool ABaseFightingCharacter::GetCanAttack()
{
	if (katana)
	{
		return katana->CanAttack();
	}
	else
	{
		return false;
	}
	
}

void ABaseFightingCharacter::ResetAttack()
{
	SetIsAttacking(false);
	AttackAnimActive = false;
	katana->ResetAttack();
}

void ABaseFightingCharacter::StartAttackAnim()
{
	AttackAnimActive = true;
}