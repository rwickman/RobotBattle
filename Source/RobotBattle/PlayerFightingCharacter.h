// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseFightingCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "PlayerFightingCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API APlayerFightingCharacter : public ABaseFightingCharacter
{
	GENERATED_BODY()
public:
	APlayerFightingCharacter();	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void MoveRight(float Value);

	// Sets jump flag when key is pressed.
	UFUNCTION()
	void StartJump();

	// Clears jump flag when key is released.
	UFUNCTION()
	void StopJump();

	// FPS camera.
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FPSCameraComponent;
};
