// Fill out your copyright notice in the Description page of Project Settings.


#include "AgentServer.h"
#include "AgentSession.h"

AgentServer::AgentServer(boost::asio::io_context& io_context, std::vector<AAgentPlayerController*> AgentControllers)
	: acceptor_(io_context, tcp::endpoint(tcp::v4(), 12001)),
	AgentControllers(AgentControllers)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::FromInt(AgentControllers.size()));
	}
	StartListening();
}

AgentServer::~AgentServer()
{
}


void AgentServer::StartListening()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("StartListening"));
	}
	
	//boost::asio::io_context io_context;
	acceptor_.async_accept(
		[this](boost::system::error_code ec, tcp::socket socket)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::FromInt(CurController));
			}
			if (!ec && AgentControllers.size() > CurController)
			{
				std::make_shared<AgentSession>(std::move(socket), AgentControllers[CurController++])->StartSession();
			}
			StartListening();
		});
	//io_context.run();
}

void AcceptConnection()
{

}