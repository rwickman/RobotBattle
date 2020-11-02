// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	float health;
	float maxHealth;
	
	UPROPERTY(BlueprintReadWrite, Category="Health")
	float healthProgress = 1.0f;
};
