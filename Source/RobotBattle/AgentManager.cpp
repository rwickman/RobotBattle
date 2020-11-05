// Fill out your copyright notice in the Description page of Project Settings.


#include "AgentManager.h"
#include "AgentServer.h"

// Sets default values
AAgentManager::AAgentManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAgentManager::BeginPlay()
{
	Super::BeginPlay();
	boost::thread* t = new boost::thread(std::bind(&AAgentManager::StartServer, this));
	
}

// Called every frame
void AAgentManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAgentManager::StartServer()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("StartServer Called"));
	}

	boost::asio::io_context io_context;
	AgentServer Server(io_context, *AgentControllers);
	io_context.run();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("StartServer END"));
	}
}