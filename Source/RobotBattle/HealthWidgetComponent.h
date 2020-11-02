// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "HealthWidget.h"
#include "HealthWidgetComponent.generated.h"


/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API UHealthWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthWidgetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void UpdateWidgetRotation();

	void SetHealthPercentage(float healthProgress);

	UPROPERTY(EditAnywhere, Category = "Health")
	TSubclassOf<UHealthWidget> healthBarWidget;

protected:
	UPROPERTY()
	UHealthWidget* currentWidget;

private:
	APlayerCameraManager* playerCamManager;
};
