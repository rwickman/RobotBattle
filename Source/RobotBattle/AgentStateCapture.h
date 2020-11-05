// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "AgentStateCapture.generated.h"


UENUM()
enum ObjType { Player, Enemy, Floor, Wall, Misc, Nothing };

USTRUCT()
struct FHitObject
{
	GENERATED_BODY()

	UPROPERTY()
	float Health;
	
	UPROPERTY()
	FVector Location;
	
	UPROPERTY()
	FRotator Rotation;
	
	UPROPERTY()
	TEnumAsByte<ObjType> Type;
};

USTRUCT()
struct FState
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<FHitObject> HitResults;
	
	UPROPERTY()
	FHitObject PlayerTransform;
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTBATTLE_API UAgentStateCapture : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAgentStateCapture();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	int NumViewLinecast = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	float ViewOffsetX = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	float ViewDistance = 500.0f;

	FState GetState();

private:
	// Capture the view of the agent using linecast
	TArray<FHitObject> CaptureView();

};
