// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanPlayerController.h"

AHumanPlayerController::AHumanPlayerController(){}


void AHumanPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	APawn* PossessedPawn = GetPawn();
	if (PossessedPawn)
	{
		PossessedFighter = Cast<APlayerFightingCharacter>(PossessedPawn);
	}
}

void AHumanPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &AHumanPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AHumanPlayerController::MoveRight);

	// Set up "look" bindings.
	InputComponent->BindAxis("Turn", this, &AHumanPlayerController::AddYawInput);
	InputComponent->BindAxis("LookUp", this, &AHumanPlayerController::AddPitchInput);

	// Set up "action" bindings.
	InputComponent->BindAction("Jump", IE_Pressed, this, &AHumanPlayerController::StartJump);
	InputComponent->BindAction("Jump", IE_Released, this, &AHumanPlayerController::StopJump);

	InputComponent->BindAction("Attack", IE_Pressed, this, &AHumanPlayerController::StartAttack);

}

void AHumanPlayerController::Tick(float DeltaTime)
{
}

void AHumanPlayerController::MoveForward(float Value)
{
	if (PossessedFighter)
	{
		PossessedFighter->MoveForward(Value);
	}
}

void AHumanPlayerController::MoveRight(float Value)
{
	if (PossessedFighter)
	{
		PossessedFighter->MoveRight(Value);
	}
}

void AHumanPlayerController::StartJump()
{
	if (PossessedFighter)
	{
		PossessedFighter->StartJump();
	}
}

void AHumanPlayerController::StopJump()
{
	if (PossessedFighter)
	{
		PossessedFighter->StopJump();
	}
}

void AHumanPlayerController::StartAttack()
{
	if (PossessedFighter)
	{
		PossessedFighter->SetIsAttacking(true);
	}
}