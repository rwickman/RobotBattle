// Copyright Epic Games, Inc. All Rights Reserved.


#include "RobotBattleGameModeBase.h"



void ARobotBattleGameModeBase::StartPlay()
{
	Super::StartPlay();
	FActorSpawnParameters ManagerSpawnParams;
	ManagerSpawnParams.Name = TEXT("AgentManager");
	AAgentPlayerController* AgentPlayerController = NewObject<AAgentPlayerController>(this);
	if (AgentPlayerController)
	{
		AgentControllers.push_back(AgentPlayerController);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Couldn't Create agent controller!"));
	}
	
	UWorld* World = GetWorld();
	if (World)
	{
		AgentManager_ = World->SpawnActor<AAgentManager>(AAgentManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, ManagerSpawnParams);
		AgentManager_->AgentControllers = &AgentControllers;
	}
	
	SetupGame();
	
}

void ARobotBattleGameModeBase::SetupGame()
{
	UWorld* World = GetWorld();
	if (World)
	{
		if (AIPlayerClass && AgentPlayerClass)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Creating player agent!"));
			}
			

			FActorSpawnParameters spawn_params;
			spawn_params.Name = TEXT("AIPlayerCharacter");
			spawn_params.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
			FVector create_loc(1044.0f, 35.0f, 120.000008f);

			created_player = World->SpawnActor<APlayerFightingCharacter>(AgentPlayerClass, create_loc, FRotator::ZeroRotator, spawn_params);
			if (created_player)
			{
				
				if (AgentControllers.size() > 0 && AgentControllers[0])
				{
					AgentControllers[0]->Possess(created_player);	
				}
	
				created_player->SetActorLabel(TEXT("PlayerAgentCharacter"));
				//created_player->ShouldRandomMove = true;
				created_player->DeadCallback = [&](ABaseFightingCharacter* DeadPlayer) {ARobotBattleGameModeBase::RestartGame(DeadPlayer); };
			}
			

			spawn_params.Name = TEXT("AgentPlayerCharacter");
			FVector create_loc2(178.0f, 840.0f, 120.000008f);
			other_player = World->SpawnActor<AAIFightingCharacter>(AIPlayerClass, create_loc2, FRotator::ZeroRotator, spawn_params);
			if (other_player && created_player)
			{
				other_player->MoveToActor(created_player);
				other_player->DeadCallback = [&](ABaseFightingCharacter* DeadPlayer) {ARobotBattleGameModeBase::RestartGame(DeadPlayer); };
			}
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("No PlayerClass!"));
			}
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("No World!"));
		}
	}
}

void ARobotBattleGameModeBase::RestartGame(ABaseFightingCharacter* DeadPlayer)
{
	if (DeadPlayer == created_player)
	{
		other_player->DestroyCharacter();
	}
	else
	{
		created_player->DestroyCharacter();
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("CALLED RESTART GAME!"));
	}
	SetupGame();
}