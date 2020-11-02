// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerFightingCharacter.h"
#include "HumanPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API AHumanPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AHumanPlayerController();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

public:
	// Called every frame
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

	UFUNCTION()
	void StartAttack();

protected:

	APlayerFightingCharacter* PossessedFighter;

};
