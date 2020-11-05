// Fill out your copyright notice in the Description page of Project Settings.


#include "AgentSession.h"

AgentSession::AgentSession(tcp::socket socket, AAgentPlayerController* AgentController)
	: socket_(std::move(socket)),
	AgentController(AgentController)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("AgentSession Created"));
	}
}

AgentSession::~AgentSession()
{
}

void AgentSession::StartSession()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("START SESSION"));
	}
	ReadData();
}

void AgentSession::ReadData()
{
	auto self(shared_from_this());
	socket_.async_read_some(boost::asio::buffer(data_, max_length),
		[this, self](boost::system::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				WriteData(length);
			}
		});
}

void AgentSession::WriteData(std::size_t length)
{
	auto self(shared_from_this());
	boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
		[this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				ReadData();
			}
	});
}