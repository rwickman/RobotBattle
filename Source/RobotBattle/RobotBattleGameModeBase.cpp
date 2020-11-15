// Copyright Epic Games, Inc. All Rights Reserved.


#include "RobotBattleGameModeBase.h"



void ARobotBattleGameModeBase::StartPlay()
{
	Super::StartPlay();

	// TODO: Encode this in a better way (perhaps create environment blueprint)
	// Craete spawn locations
	AgentSpawnLocations.push_back(FVector(577.0f, 405.0f, 120.000008f));
	AgentSpawnLocations.push_back(FVector(577.0f, 2426.0f, 120.000008f));
	AgentSpawnLocations.push_back(FVector(2306.0f, 2426.0f, 120.000008f));
	AgentSpawnLocations.push_back(FVector(2306.0f, 438.0f, 120.000008f));

	AISpawnLocations.push_back(FVector(178.0f, 840.0f, 120.000008f));
	AISpawnLocations.push_back(FVector(178.0f, 2828.0f, 120.000008f));
	AISpawnLocations.push_back(FVector(1907.0, 2828.0f, 120.000008f));
	AISpawnLocations.push_back(FVector(1907.0, 840.0f, 120.000008f));

	FActorSpawnParameters ManagerSpawnParams;
	ManagerSpawnParams.Name = TEXT("AgentManager");
	UWorld* World = GetWorld();
	
	// Create the AgentControllers
	for (int i = 0; i < 4; ++i)
	{
		AAgentPlayerController* AgentPlayerController = World->SpawnActor<AAgentPlayerController>(AAgentPlayerController::StaticClass());
		if (AgentPlayerController)
		{
			AgentPlayerController->SetupCallback = [&, i]() { SetupGame(i); };
			AgentControllers.push_back(AgentPlayerController);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Couldn't Create agent controller!"));
		}
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
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Yellow, TEXT("SetupGame on ControllerIndex ") + FString::FromInt(ControllerIndex));
	}
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
			spawn_params.Name = FName( *("AIPlayerCharacter_" + FString::FromInt(ControllerIndex)));
			spawn_params.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;

			APlayerFightingCharacter* AgentFighter= World->SpawnActor<APlayerFightingCharacter>(AgentPlayerClass, AgentSpawnLocations[ControllerIndex], FRotator::ZeroRotator, spawn_params);
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
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("AgentController is Possessing AgentFighter"));
					}
					AgentControllers[ControllerIndex]->Possess(AgentFighter);
					//AgentControllers[ControllerIndex]->Setup();
				}
	
				AgentFighter->SetActorLabel(TEXT("PlayerAgentCharacter_")+ FString::FromInt(ControllerIndex));
				//created_player->ShouldRandomMove = true;
				AgentFighter->DeadCallback = [&, ControllerIndex](ABaseFightingCharacter* DeadPlayer) {ARobotBattleGameModeBase::RestartGame(DeadPlayer, ControllerIndex); };
			}
			

			spawn_params.Name = FName( *("AgentPlayerCharacter_" + FString::FromInt(ControllerIndex)));
			AAIFightingCharacter* AIFighter = World->SpawnActor<AAIFightingCharacter>(AIPlayerClass, AISpawnLocations[ControllerIndex], FRotator::ZeroRotator, spawn_params);
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