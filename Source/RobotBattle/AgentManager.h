// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

THIRD_PARTY_INCLUDES_START
#pragma push_macro("check")
#undef check

#pragma push_macro("TEXT")
#undef TEXT

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#pragma pop_macro("check")
#pragma pop_macro("TEXT")
THIRD_PARTY_INCLUDES_END
#include <vector>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AgentPlayerController.h"
#include "AgentManager.generated.h"

UCLASS()
class ROBOTBATTLE_API AAgentManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAgentManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartServer();

	std::vector<AAgentPlayerController*>* AgentControllers;

};
