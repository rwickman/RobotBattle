// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseFightingCharacter.h"
#include "AIFightingCharacter.h"
#include "RobotBattleGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API ARobotBattleGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	virtual void StartPlay() override;

public:
	// The PlayerCharacter to spawn.
	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TSubclassOf <class ABaseFightingCharacter> PlayerClass;

	void SetupGame();
	void RestartGame(ABaseFightingCharacter* DeadPlayer);

private:
	AAIFightingCharacter* other_player;
	AAIFightingCharacter* created_player;
};
