// Copyright Epic Games, Inc. All Rights Reserved.


#include "RobotBattleGameModeBase.h"


void ARobotBattleGameModeBase::StartPlay()
{
	Super::StartPlay();

	SetupGame();
}

void ARobotBattleGameModeBase::SetupGame()
{
	UWorld* world = GetWorld();
	if (world)
	{
		if (PlayerClass)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Creating player agent!"));
			}
			FActorSpawnParameters spawn_params;
			spawn_params.Name = TEXT("PlayerAgentCharacter1");
			spawn_params.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
			FVector create_loc(1044.0f, 35.0f, 120.000008f);

			created_player = world->SpawnActor<AAIFightingCharacter>(PlayerClass, create_loc, FRotator::ZeroRotator, spawn_params);
			if (created_player)
			{
				created_player->SetActorLabel(TEXT("PlayerAgentCharacter"));
				created_player->ShouldRandomMove = true;
				created_player->DeadCallback = [&](ABaseFightingCharacter* DeadPlayer) {ARobotBattleGameModeBase::RestartGame(DeadPlayer); };
			}
			

			spawn_params.Name = TEXT("PlayerAgentCharacter2");
			FVector create_loc2(178.0f, 840.0f, 120.000008f);
			other_player = world->SpawnActor<AAIFightingCharacter>(PlayerClass, create_loc2, FRotator::ZeroRotator, spawn_params);
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