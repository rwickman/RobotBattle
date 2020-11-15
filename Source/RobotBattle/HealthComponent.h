// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTBATTLE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "Health")
	float maxHealthPoints = 5.0f;

	UFUNCTION()
	// Deal damage and return true if curHitPoints <= 0
	bool TakeDamage(float damage);

	UFUNCTION()
	// Get the current hit points left.
	float GetHealthPoints();

	// Get the percentage of health left
	float GetHealthPercentage();


private:
	float healthPoints = 5.0f;


};
