// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthManagerComponent.h"

// Sets default values for this component's properties
UHealthManagerComponent::UHealthManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	HealthWidgetComponent = CreateDefaultSubobject<UHealthWidgetComponent>(TEXT("HealthWidgetComponent"));
	//healthWidgetComponent->SetupAttachment(Cast<USceneComponent>(GetOwner());
	HealthWidgetComponent->SetupAttachment(this);


	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
}


// Called when the game starts
void UHealthManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UHealthManagerComponent::TakeDamage(float damage)
{
	bool died = Health->TakeDamage(damage);
	HealthWidgetComponent->SetHealthPercentage(Health->GetHealthPercentage());
	if (died)
	{
		// Owner has died
 
		
	}
	else
	{
		// Update widget to reflect current hit points
		//healthWidgetComponent->
		//health->GetCurrentHitPoints();
	}
	return died;

}

float UHealthManagerComponent::GetHealthPoints()
{
	return Health->GetHealthPoints();
}

