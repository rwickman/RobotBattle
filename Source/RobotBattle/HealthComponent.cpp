// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UHealthComponent::TakeDamage(float damage)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString("INSIDE BASE CHARACTER: ") + FString::SanitizeFloat(GetHealthPoints()));
	healthPoints -= damage;

	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString("Inside TakeDamage: ") + FString::SanitizeFloat(GetHealthPoints()));
	if (healthPoints <= 0.0f)
	{
		healthPoints = 0.0f;
		return true;
	}
	return false;
}

float UHealthComponent::GetHealthPoints()
{
	return healthPoints;
}

float UHealthComponent::GetHealthPercentage()
{
	return GetHealthPoints() / maxHealthPoints;
}