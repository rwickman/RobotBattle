// Fill out your copyright notice in the Description page of Project Settings.


#include "AgentSession.h"
#include <string>

AgentSession::AgentSession(tcp::socket socket, AAgentPlayerController* AgentController)
	: socket_(std::move(socket)),
	AgentController(AgentController)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("AgentSession Created"));
	}
	if (AgentController)
	{
		AgentController->WriteStateCallback = [&]() { EnableSetupComplete(); };
		AgentController->StateCapturedCallback = [&]() {EnableStateCaptured(); };
	}

}

AgentSession::~AgentSession()
{
}

void AgentSession::StartSession()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("START SESSION NOW!!"));
	}

	AgentController->ShouldSetup = true;

	WriteState();
}


void AgentSession::ReadActionHeader()
{
	/*
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green, TEXT("Reading action header"));
	}*/

	auto self(shared_from_this());

	boost::asio::async_read(socket_, boost::asio::buffer(ActionPacket.GetData(), ActionPacket.HEADER_LENGTH),
		[this, self](boost::system::error_code ec, std::size_t length)
		{

		if (!ec && ActionPacket.DecodeHeader())
		{
			ReadActionBody();
		}
	});
}
void AgentSession::ReadActionBody()
{
	/*
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green, TEXT("Reading action"));
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green, FString::FromInt(ActionPacket.GetBodyLength()));
	}
	*/

	auto self(shared_from_this());
	boost::asio::async_read(socket_, boost::asio::buffer(ActionPacket.GetBody(), ActionPacket.GetBodyLength()),
		[this, self](boost::system::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				/*
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green, "INSIDE ACTION READ BODY");
				}
				
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green, ActionPacket.GetBody());
				}*/
				
				
				char* CurBodyContent = new char [ActionPacket.GetBodyLength()];
				ActionPacket.GetBodyContent(CurBodyContent);
				AgentController->SetAction(FString(CurBodyContent));
				delete CurBodyContent;
				WriteState();
			}
		});
}


void AgentSession::ReadRestart()
{
	/*
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green, TEXT("Read Restart"));
	}
	*/

	auto self(shared_from_this());
	socket_.async_read_some(boost::asio::buffer(ActionPacket.GetData(), DataPacket.HEADER_LENGTH),
		[this, self](boost::system::error_code ec, std::size_t length)
		{

		if (!ec && ActionPacket.DecodeHeader())
		{
			/*
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green, TEXT("Restart read!"));
			}
			*/
			// Setup up the next game
			AgentController->ShouldSetup = true;
			// Write the first state of the episode
			// TODO: NEED TO WAIT FOR GAME TO BE SETUP BEFORE CALLING WriteState
			SetupComplete = false;
			std::unique_lock<std::mutex> lk(io_Mutex);
			Setup_CV.wait(lk, [&] {return SetupComplete; });
			/*
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Purple, TEXT("Setup Complete"));
			}
			*/
			WriteState();
		}
	});
}


void AgentSession::WriteState()
{
	/*
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green, TEXT("Writing State!!"));
	}
	*/
	// Get the current state/reward
	StateCaptured = false;
	AgentController->RequestState();
	std::unique_lock<std::mutex> lk(io_Mutex_2);
	State_CV.wait(lk, [&] {return StateCaptured; });
	std::string  CurPairStr = AgentController->ConsumeState();
	
	//FString CurPairStr = AgentController->GetState();
	//std::string CurPairStr = AgentController->ConsumeState();

	bool IsEpisodeTerminated = AgentController->GetIsEpisodeTerminated();
	
	/*
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green,  FString::Printf(TEXT("CurPairStr.Len() is %d"), CurPairStr.size() ) );
	}
	*/
	// Convert to char*
	
	const char* CurPairStrArr = CurPairStr.c_str();//new char[CurPairStr.Len()];//= TCHAR_TO_UTF8(*CurPairStr);


    /*
	for (int i = 0; i < CurPairStr.Len(); i++)
	{
		CurPairStrArr[i] = CurPairStr[i];
	}*/
	
	// Write to packet
	DataPacket.SetBody(CurPairStrArr);
	
	/*
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::FromInt(DataPacket.GetBodyLength()));
	}*/
	//delete CurPairStrArr;
	auto self(shared_from_this());
	boost::asio::async_write(socket_, boost::asio::buffer(DataPacket.GetData(), DataPacket.Length()),
		[this, self, IsEpisodeTerminated](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				if (IsEpisodeTerminated)
				{
					ReadRestart();
				}
				else
				{
					ReadActionHeader();
				}
			}
	});
}


void AgentSession::EnableSetupComplete()
{
	/*
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Purple, TEXT("EnableSetupComplete"));
	}
	*/
	SetupComplete = true;
	Setup_CV.notify_one();
}

void AgentSession::EnableStateCaptured()
{
	StateCaptured = true;
	State_CV.notify_one();
}