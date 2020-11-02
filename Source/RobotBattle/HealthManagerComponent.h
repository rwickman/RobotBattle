// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/WidgetComponent.h"
#include "HealthComponent.h"
#include "HealthWidgetComponent.h"
#include "HealthManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTBATTLE_API UHealthManagerComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	// Deal damage and return true if owner had died.
	bool TakeDamage(float damage);

	float GetHealthPoints();

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	UHealthWidgetComponent* HealthWidgetComponent;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	UHealthComponent* Health;


};
