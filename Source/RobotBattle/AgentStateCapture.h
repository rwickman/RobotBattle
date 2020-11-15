// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

THIRD_PARTY_INCLUDES_START
#include <nlohmann/json.hpp>
THIRD_PARTY_INCLUDES_END

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
	float Location[3];
	
	UPROPERTY()
	float Rotation[3];

	bool IsAttacking = false;

	ObjType Type;
};

USTRUCT()
struct FState
{
	GENERATED_BODY()
	
	std::vector<FHitObject> HitResults;
	
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
	int NumViewLinecast = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	float ViewOffsetX = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	float ViewDistance = 1000.0f;

	FState GetState();
	
	UPROPERTY()
	FState CurState_;

	bool ShouldGetState = false;

private:
	// Capture the view of the agent using linecast
	std::vector<FHitObject> CaptureView();

};
