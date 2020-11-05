// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Dom/JsonObject.h"
#include "PlayerFightingCharacter.h"
#include "JsonObjectConverter.h"
#include "AgentPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API AAgentPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAgentPlayerController();

	void GetState();
	
};
