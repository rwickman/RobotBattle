// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include <functional>
#include <string>

THIRD_PARTY_INCLUDES_START
#include <nlohmann/json.hpp>
THIRD_PARTY_INCLUDES_END

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Dom/JsonObject.h"
#include "PlayerFightingCharacter.h"
#include "JsonObjectConverter.h"
#include "AgentStateCapture.h"
#include "AgentPlayerController.generated.h"



// TODO: Put the structs in the same place, perhaps independent file
USTRUCT()
struct FStateRewardPair
{
	GENERATED_BODY()

	UPROPERTY()
	FState State;

	UPROPERTY()
	float Reward;

	UPROPERTY()
	bool IsEpisodeTerminated;

};




USTRUCT()
struct FAction
{
	GENERATED_BODY()

	UPROPERTY()
	// -1 is backward and 1 is forwards
	float MoveForward = 0.0f;

	UPROPERTY()
	// -1 is left and 1 is right
	float MoveRight = 0.0f;

	UPROPERTY()
	// -1 is turn left and 1 turn right
	float TurnRight = 0.0f;

	UPROPERTY()
	// -1 is look down and 1 is look up
	float LookUp = 0.0f;

	UPROPERTY()
	bool Jump = false;

	UPROPERTY()
	bool Attack = false;
};


UCLASS()
class ROBOTBATTLE_API AAgentPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAgentPlayerController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	// Perform an action where the action is a bit string
	void SetAction(const FString Action);

	std::string GetState();

	UFUNCTION()
	void Setup();
	
	UFUNCTION()
	void TerminateEpisode();

	UPROPERTY()
	int ActionSpaceLen = 10;

	UFUNCTION()
	bool GetIsEpisodeTerminated() const;

	std::function<void()> SetupCallback;

	std::function<void()> WriteStateCallback;
	std::function<void()> StateCapturedCallback;

	UPROPERTY()
	bool ShouldSetup = false;

	std::string CapturedState;

	UFUNCTION()
	void RequestState();

	std::string ConsumeState();


	
protected:
	void PerformAction();
	bool IsEpisodeTerminated = false;
	bool IsActionPending = false;
	UPROPERTY()
	bool ShouldGetState = false;

	FAction CurAction;
};
