// Fill out your copyright notice in the Description page of Project Settings.


#include "AgentManager.h"


// Sets default values
AAgentManager::AAgentManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

AAgentManager::~AAgentManager()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("~AAgentManager"));
	}
	/*
	if (ServerThread)
	{
		TerminateThread(ServerThread, 0);
	}
	*/
	io_context.stop();
	//TerminateThread(ServerThread, 0);
}

// Called when the game starts or when spawned
void AAgentManager::BeginPlay()
{
	Super::BeginPlay();
	
	//ServerThread = new std::thread(std::bind(&AAgentManager::StartServer, this));
	//Server = new AgentServer(io_context, *AgentControllers);
	//shared_ptr<asio::io_service::work> work(new asio::io_service::work(io_service));
	//boost::thread t(&asio::io_service::run, &io_service);
	//Server = new AgentServer(io_context, *AgentControllers);
	//a = new A(io_context, AgentControllers);

	



	/*
	if (TerminateThread(ServerThread, 0))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("Terminated Thread"));
		} 
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("FALIIED TO Terminated Thread"));
		}
	}
	*/
}

// Called every frame
void AAgentManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!StartedServer)
	{
		ServerThread = new std::thread(
			[&]()
			{
				StartServer();
			});
		ServerThread->detach();
		StartedServer = true;
	}
	
}

void AAgentManager::StartServer()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("StartServer Called"));
	}
	AgentServer Server(io_context, *AgentControllers);
	io_context.run();
}

