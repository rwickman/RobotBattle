// Fill out your copyright notice in the Description page of Project Settings.


#include "RewardManagerComponent.h"

// Sets default values for this component's properties
URewardManagerComponent::URewardManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void URewardManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URewardManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float URewardManagerComponent::ConsumeReward()
{
	float ConsumedReward = CurrentReward;
	CurrentReward = 0.0f;
	return ConsumedReward;
}

void URewardManagerComponent::DamageDealt(float Damage)
{
	CurrentReward += Damage * Reward::HurtEnemy;
}


void URewardManagerComponent::DamageTaken(float Damage)
{
	CurrentReward += Damage * Reward::HurtAgent;
}


void URewardManagerComponent::GameEnd(bool GameWon)
{
	if (GameWon)
	{
		CurrentReward += Reward::Won;
	}
	else
	{
		CurrentReward += Reward::Lost;
	}
}
