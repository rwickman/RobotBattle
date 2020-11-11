// Fill out your copyright notice in the Description page of Project Settings.


#include "AgentPlayerController.h"

AAgentPlayerController::AAgentPlayerController()
{

}

void to_json(nlohmann::json& j, const FHitObject& Hit) {
	j = nlohmann::json{
		{"Health",  Hit.Health},
		{"Location", Hit.Location},
		{"Rotation", Hit.Rotation},
		{"Type", Hit.Type}
	};
}

void to_json(nlohmann::json& j, const FState& State) {
	j = nlohmann::json{
		{"HitResults", State.HitResults},
		{"PlayerTransform", State.PlayerTransform}
	};
}

void to_json(nlohmann::json& j, const FStateRewardPair& Pair) {
	j = nlohmann::json{
		{"State", Pair.State},
		{"Reward", Pair.Reward},
		{"IsEpisodeTerminated", Pair.IsEpisodeTerminated}
	};
}

void AAgentPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AAgentPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (ShouldSetup)
	{
		ShouldSetup = false;
		Setup();
	}
	CapturedState = GetState();
	/*
	if (ShouldGetState)
	{
		
		
		if (StateCapturedCallback)
		{
			StateCapturedCallback();
		}
	}*/

	PerformAction();
}


void AAgentPlayerController::SetAction(const FString Action)
{

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, Action);
	}
	if (Action.Len() >= ActionSpaceLen)
	{
		CurAction = FAction();
		if (Action[0] == '1')
		{
			CurAction.MoveForward += 1;
		}
		if (Action[1] == '1')
		{
			CurAction.MoveForward -= 1;
		}
		if (Action[2] == '1')
		{
			CurAction.MoveRight += 1;
		}
		if (Action[3] == '1')
		{
			CurAction.MoveRight -= 1;
		}
		if (Action[4] == '1')
		{
			CurAction.TurnRight += 1;
		}
		if (Action[5] == '1')
		{
			CurAction.TurnRight -= 1;
		}
		if (Action[6] == '1')
		{
			CurAction.LookUp += 1;
		}
		if (Action[7] == '1')
		{
			CurAction.LookUp -= 1;
		}
		if (Action[8] == '1')
		{
			CurAction.Jump = true;
		}
		if (Action[9] == '1')
		{
			CurAction.Attack = true;
		}
		IsActionPending = true;
	}
	
}

std::string AAgentPlayerController::GetState()
{
	// TODO: Need to handle rewards
	FStateRewardPair CurStateRewardPair;
	APawn* PossessedPawn = GetPawn();
	if (PossessedPawn)
	{
		APlayerFightingCharacter* PossessedFighter = Cast<APlayerFightingCharacter>(PossessedPawn);
		if (PossessedFighter)
		{
			if (!IsEpisodeTerminated)
			{
				CurStateRewardPair.State = PossessedFighter->GetAgentView()->GetState();
			}

			CurStateRewardPair.IsEpisodeTerminated = IsEpisodeTerminated;
			CurStateRewardPair.Reward = PossessedFighter->GetRewardManager()->ConsumeReward();
		}
	}

	nlohmann::json CurPairJson;
	CurPairJson["StateRewardPair"] = CurStateRewardPair;
	//CurPairJson["Reward"] = CurStateRewardPair.Reward;
	//CurPairJson["IsEpisodeTerminated"] = CurStateRewardPair.IsEpisodeTerminated;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, CurPairJson.dump().c_str());
		
	}
	//CurPairJson["HitObject"]
	// TODO: Remove this and replace HitResults with vector
	//std::vector < std::vector > HitResults;
	//CurPairJson["HitResults"] = CurStateRewardPair.State.HitResults;
	//CurPairJson["StateRewardPair"] = CurStateRewardPair;
	/*
	for (FHitObject HitResult : CurStateRewardPair.State.HitResults)
	{
		HitResults
	}*/

	//FString CurPairStr;
	//FJsonObjectConverter::UStructToJsonObjectString(CurStateRewardPair, CurPairStr);

	return CurPairJson.dump();
}

void AAgentPlayerController::Setup()
{
	IsEpisodeTerminated = false;
	IsActionPending = false;
	if (SetupCallback)
	{
		SetupCallback();
	}
	if (WriteStateCallback)
	{
		WriteStateCallback();
	}
}

void AAgentPlayerController::TerminateEpisode()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("Terminating Episode"));
	}
	IsActionPending = false;
	IsEpisodeTerminated = true;
}

void AAgentPlayerController::PerformAction()
{
	APawn* PossessedPawn = GetPawn();
	if (PossessedPawn)
	{
		APlayerFightingCharacter* PossessedFighter = Cast<APlayerFightingCharacter>(PossessedPawn);
		if (PossessedFighter)
		{
			if (IsActionPending)
			{
				IsActionPending = false;
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("PERFORMING ACTION"));
				}
				PossessedFighter->MoveForward(CurAction.MoveForward);
				PossessedFighter->MoveRight(CurAction.MoveRight);
				AddYawInput(CurAction.TurnRight);
				AddPitchInput(CurAction.LookUp);
				if (CurAction.Jump)
				{
					PossessedFighter->StartJump();
				}
				else
				{
					PossessedFighter->StopJump();
				}

				if (CurAction.Attack)
				{
					PossessedFighter->SetIsAttacking(true);
				}
			}
			else
			{
				PossessedFighter->StopJump();
			}
		}
	}
}

bool AAgentPlayerController::GetIsEpisodeTerminated() const
{
	return IsEpisodeTerminated;
}

void AAgentPlayerController::RequestState()
{
	ShouldGetState = true;
}

std::string AAgentPlayerController::ConsumeState()
{
	return CapturedState;
}