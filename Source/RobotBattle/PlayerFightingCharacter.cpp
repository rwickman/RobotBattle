// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFightingCharacter.h"

APlayerFightingCharacter::APlayerFightingCharacter()
{
	// Create a first person camera component.
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPSCameraComponent != nullptr);

	// Attach the camera component to our capsule component.
	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	
	// Position the camera slightly above the eyes.
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));

	// Enable the pawn to control camera rotation.
	FPSCameraComponent->bUsePawnControlRotation = true;
}

void APlayerFightingCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerFightingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerFightingCharacter::MoveForward(float Value)
{
	// Find out which way is "forward" and record that the player wants to move that way.
	FVector direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(direction, Value);
}

void APlayerFightingCharacter::MoveRight(float Value)
{
	// Find out which way is "right" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void APlayerFightingCharacter::StartJump()
{
	bPressedJump = true;
}

void APlayerFightingCharacter::StopJump()
{
	bPressedJump = false;
}