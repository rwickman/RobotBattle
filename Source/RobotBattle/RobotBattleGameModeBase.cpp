// Copyright Epic Games, Inc. All Rights Reserved.


#include "RobotBattleGameModeBase.h"



void ARobotBattleGameModeBase::StartPlay()
{
	Super::StartPlay();
	FActorSpawnParameters ManagerSpawnParams;
	ManagerSpawnParams.Name = TEXT("AgentManager");
	UWorld* World = GetWorld();
	AAgentPlayerController* AgentPlayerController = World->SpawnActor<AAgentPlayerController>(AAgentPlayerController::StaticClass());
	if (AgentPlayerController)
	{
		AgentPlayerController->SetupCallback = [&]() { SetupGame(0); };
		AgentControllers.push_back(AgentPlayerController);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Couldn't Create agent controller!"));
	}
	
	
	if (World)
	{
		AgentManager_ = World->SpawnActor<AAgentManager>(AAgentManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, ManagerSpawnParams);
		AgentManager_->AgentControllers = &AgentControllers;
	}
	
	//SetupGame(0);
	
}

void ARobotBattleGameModeBase::SetupGame(int ControllerIndex)
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

			APlayerFightingCharacter* AgentFighter= World->SpawnActor<APlayerFightingCharacter>(AgentPlayerClass, create_loc, FRotator::ZeroRotator, spawn_params);
			if (AgentFighter)
			{
				if (AgentFighters.Num() > ControllerIndex)
				{
					AgentFighters[ControllerIndex] = AgentFighter;
				}
				else
				{
					AgentFighters.Add(AgentFighter);
				}
				
				if (AgentControllers.size() > ControllerIndex && AgentControllers[ControllerIndex])
				{
					AgentControllers[ControllerIndex]->Possess(AgentFighter);
					//AgentControllers[ControllerIndex]->Setup();
				}
	
				AgentFighter->SetActorLabel(TEXT("PlayerAgentCharacter"));
				//created_player->ShouldRandomMove = true;
				AgentFighter->DeadCallback = [&, ControllerIndex](ABaseFightingCharacter* DeadPlayer) {ARobotBattleGameModeBase::RestartGame(DeadPlayer, ControllerIndex); };
			}
			

			spawn_params.Name = TEXT("AgentPlayerCharacter");
			FVector create_loc2(178.0f, 840.0f, 120.000008f);
			AAIFightingCharacter* AIFighter = World->SpawnActor<AAIFightingCharacter>(AIPlayerClass, create_loc2, FRotator::ZeroRotator, spawn_params);
			if (AIFighter && AgentFighter)
			{
				AIFighter->MoveToActor(AgentFighter);
				AIFighter->DeadCallback = [&, ControllerIndex](ABaseFightingCharacter* DeadPlayer) {ARobotBattleGameModeBase::RestartGame(DeadPlayer, ControllerIndex); };
				
				if (AIFighters.Num() > ControllerIndex)
				{
					AIFighters[ControllerIndex] = AIFighter;
				}
				else
				{
					AIFighters.Add(AIFighter);
				}
				
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

void ARobotBattleGameModeBase::RestartGame(ABaseFightingCharacter* DeadPlayer, int EnvironmentID)
{
	AAgentPlayerController* DeadPlayerController = Cast<AAgentPlayerController>(DeadPlayer->Controller);

	if (DeadPlayerController)
	{
		DeadPlayerController->TerminateEpisode();
		AIFighters[EnvironmentID]->DestroyCharacter();
	}
	else
	{
		DeadPlayerController = Cast<AAgentPlayerController>(AgentFighters[EnvironmentID]->Controller);
		DeadPlayerController->TerminateEpisode();
		AgentFighters[EnvironmentID]->DestroyCharacter();
	}
}