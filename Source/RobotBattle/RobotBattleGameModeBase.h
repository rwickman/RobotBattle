// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseFightingCharacter.h"
#include "AIFightingCharacter.h"
#include "AgentManager.h"
#include "AgentPlayerController.h"
#include "PlayerFightingCharacter.h"
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
	// The AAIPlayerCharacter to spawn.
	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TSubclassOf <class ABaseFightingCharacter> AIPlayerClass;

	// The PlayerCharacter to spawn.
	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TSubclassOf <class ABaseFightingCharacter> AgentPlayerClass;

	void SetupGame();
	void RestartGame(ABaseFightingCharacter* DeadPlayer);
	
	std::vector<AAgentPlayerController*> AgentControllers;

private:
	APlayerFightingCharacter* created_player;
	AAIFightingCharacter* other_player;


	AAgentManager* AgentManager_;


};
