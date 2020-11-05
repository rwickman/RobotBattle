// Fill out your copyright notice in the Description page of Project Settings.


#include "AgentPlayerController.h"

AAgentPlayerController::AAgentPlayerController()
{

}

void AAgentPlayerController::GetState()
{
	// TODO: Need to handle rewards
;
	APawn* PossessedPawn = GetPawn();
	if (PossessedPawn)
	{
		APlayerFightingCharacter* PossessedFighter = Cast<APlayerFightingCharacter>(PossessedPawn);
		//std::vector<FHitObject> HitObjs = PossessedFighter->AgentView->CaptureView();
		FState CurState = PossessedFighter->GetAgentView()->GetState();
	}
	//FJsonObjectConverter::UStructToJsonObjectString();
}