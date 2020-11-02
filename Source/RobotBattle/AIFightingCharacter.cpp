// Fill out your copyright notice in the Description page of Project Settings.


#include "AIFightingCharacter.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Runtime/NavigationSystem/Public/AbstractNavData.h"
#include "AI/NavigationSystemBase.h"
#include "NavigationSystem.h"


AAIFightingCharacter::AAIFightingCharacter()
{
}

// Called when the game starts or when spawned
void AAIFightingCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (shouldMove && ShouldRandomMove)
	{
		RandomMove();
	}
}

// Called every frame
void AAIFightingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// TODO: Stop attacking when actor died
	// TODO: Reset actors when one of them died
	if (shouldMove)
	{
		if (ShouldRandomMove)
		{
			if (!TargetSet)
			{
				RandomMove();
			}
			else
			{
				float distance = FVector::Distance(GoalLocation, GetActorLocation());
				if (distance <= StoppingDistance)
				{
					TargetSet = false;
				}
			}
			
		}
		else if (GoalActor) {
			GoalLocation = GoalActor->GetActorLocation();
			float distance = FVector::Distance(GoalLocation, GetActorLocation());
			if (distance <= StoppingDistance)
			{
				TargetSet = false;
				SetIsAttacking(true);
			}
			else
			{
				SetIsAttacking(false);
				if (!TargetSet)
				{
					MoveToActor(GoalActor);
				}
			}
		}
	}
}

void AAIFightingCharacter::RandomMove()
{
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	FNavLocation NavLoc;
	bool bSuccess = NavSys->GetRandomReachablePointInRadius(GetActorLocation(), RandomMoveRadius, NavLoc);
	if (bSuccess && Controller)
	{
		Cast<AAIController>(Controller)->MoveToLocation(NavLoc.Location);
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, NavLoc.Location.ToString());
		GoalLocation = NavLoc.Location;
		TargetSet = true;
	}
}

void AAIFightingCharacter::MoveToActor(AActor* Goal)
{
	if (Controller)
	{
		GoalActor = Goal;
		Cast<AAIController>(Controller)->MoveToActor(Goal);
		GoalLocation = Goal->GetActorLocation();
		TargetSet = true;
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, "AIFighter Controller does not exist!!");
		}
	}
	
}