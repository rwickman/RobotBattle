// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseFightingCharacter.h"
#include "AIFightingCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API AAIFightingCharacter : public ABaseFightingCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIFightingCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Random Move
	UFUNCTION()
	void RandomMove();

	UFUNCTION()
	void MoveToActor(AActor* Goal);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Navigation")
	bool shouldMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Navigation")
	bool ShouldRandomMove = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Navigation")
	float RandomMoveRadius = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Navigation")
	float StoppingDistance = 50.0f;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Navigation")
	FVector GoalLocation;



protected:
	AActor* GoalActor;
	bool TargetSet;
	bool ChasingTarget;

};
