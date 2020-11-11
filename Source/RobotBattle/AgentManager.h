// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <thread>

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
#include "AgentServer.h"
#include "AgentManager.generated.h"

class A
{
public:
	A(boost::asio::io_context& io_context, std::vector<AAgentPlayerController*>* ag) : io_context(io_context), AgentControllers(ag)
	{
	}
	boost::asio::io_context& io_context;
	std::vector<AAgentPlayerController*>* AgentControllers;
};

UCLASS()
class ROBOTBATTLE_API AAgentManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAgentManager();
	~AAgentManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartServer();

	std::vector<AAgentPlayerController*>* AgentControllers;
	
	boost::asio::io_context io_context;
	

protected:
	std::thread* ServerThread;
	bool StartedServer;
};
