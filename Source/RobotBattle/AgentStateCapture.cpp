// Fill out your copyright notice in the Description page of Project Settings.


#include "AgentStateCapture.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"
#include "BaseFightingCharacter.h"

// Sets default values for this component's properties
UAgentStateCapture::UAgentStateCapture()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAgentStateCapture::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAgentStateCapture::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//TArray<FHitObject> View = CaptureView();
}

FState UAgentStateCapture::GetState()
{
	FState CurState;
	FHitObject PlayerTransform;
	AActor* Owner = GetOwner();
	if (Owner)
	{
		ABaseFightingCharacter* OwnerFighter = Cast<ABaseFightingCharacter>(Owner);
		if (OwnerFighter)
		{
			PlayerTransform.Health = OwnerFighter->GetHealthPoints();
		}
		else
		{
			PlayerTransform.Health = -1.0f;
		}
		PlayerTransform.Location = Owner->GetActorLocation();
		PlayerTransform.Rotation = Owner->GetActorRotation();
		PlayerTransform.Type = ObjType::Player;
	}
	CurState.PlayerTransform = PlayerTransform;
	CurState.HitResults = CaptureView();
	return CurState;
}

TArray<FHitObject> UAgentStateCapture::CaptureView()
{
	AActor* Owner = GetOwner();
	TArray<FHitObject> HitResults;
	UWorld * World = GetWorld();
	if (Owner)
	{
		
		FHitResult OutHit;
		FVector Start = GetComponentLocation();

		// Move the start forward
		Start.X += 200.f;

		// Get the look forward direction of the Actor
		// Get the camera transform.
		FVector CameraLocation;
		FRotator CameraRotation;

		FVector ViewOffset(ViewOffsetX, 0.0f, 0.0f);
		Owner->GetActorEyesViewPoint(CameraLocation, CameraRotation);
		FVector ViewStart = CameraLocation + FTransform(CameraRotation).TransformVector(ViewOffset);

		FVector ViewEndOffset(ViewDistance + ViewOffsetX, 0.0f, 0.0f);

		FVector ViewEnd = CameraLocation + FTransform(CameraRotation).TransformVector(ViewEndOffset);
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(Owner);


		float cur_angle = 0.0f;
		for (int i = 0; i < NumViewLinecast; ++i)
		{
			FHitObject CurHit;
			float x_angle;
			float y_angle;
			
			FMath::SinCos(&y_angle, &x_angle, cur_angle);
			cur_angle += 2 * PI / NumViewLinecast;

			FRotator AngleRot(x_angle, y_angle, 0.0f);
			
			// TODO: Need to fix linecast to always be "perfect" circle
			FVector RotViewEnd = FTransform(AngleRot).TransformVector(ViewEnd);

			DrawDebugLine(World, ViewStart, RotViewEnd, FColor::Green, false, 1, 0, 1.0f);

			if (World->LineTraceSingleByChannel(OutHit, ViewStart, RotViewEnd, ECC_WorldStatic, CollisionParams))
			{
				AActor* HitActor = OutHit.GetActor();
				FString HitName(HitActor->GetName());
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("The Component Being Hit is: %s"), *OutHit.GetActor()->GetName()));
				ABaseFightingCharacter* HitFighter = Cast<ABaseFightingCharacter>(HitActor);
				if (HitFighter)
				{
					CurHit.Health = HitFighter->GetHealthPoints();
				}
				else
				{
					CurHit.Health = -1.0f;
				}
				CurHit.Location = HitActor->GetActorLocation();
				CurHit.Rotation = HitActor->GetActorRotation();

				// Set the type
				// TODO: Optimize this to use the class instead
				if (HitName.Contains("Fighter"))
				{
					CurHit.Type = ObjType::Enemy;
				}
				else if (HitName.Contains("Floor"))
				{
					CurHit.Type = ObjType::Floor;
				}
				else if (HitName.Contains("Wall"))
				{
					CurHit.Type = ObjType::Wall;
				}
				else
				{
					CurHit.Type = ObjType::Misc;
				}

			}
			else
			{
				CurHit.Health = 0.0f;
				CurHit.Location = FVector::ZeroVector;
				CurHit.Rotation = FRotator::ZeroRotator;
				CurHit.Type = ObjType::Nothing;
			}
			// Add the information about the current linecast
			HitResults.Add(CurHit);
		}
		
	}
	return HitResults;
}