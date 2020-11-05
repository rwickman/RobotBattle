// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RewardManagerComponent.generated.h"

UENUM()
enum Reward
{
	HurtEnemy = 1,
	HurtAgent = -1,
	Won = 10,
	Lost = -10
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTBATTLE_API URewardManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URewardManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// Get the current reward and reset it
	float ConsumeReward();

	// Adds rewardfor damage dealt to enemies
	void DamageDealt(float Damage);

	// Adds reward for damage dealt to agent
	void DamageTaken(float Damage);

	// Adds reward for either game won or lost
	void GameEnd(bool GameWon);

private:
	float CurrentReward = 0.0f;
};
