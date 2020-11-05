// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

THIRD_PARTY_INCLUDES_START
#pragma push_macro("check")
#undef check

#pragma push_macro("TEXT")
#undef TEXT

#include <boost/asio.hpp>

#pragma pop_macro("check")
#pragma pop_macro("TEXT")
THIRD_PARTY_INCLUDES_END


#include <cstdlib>
#include <memory>
#include <utility>

#include "AgentPlayerController.h"
#include "CoreMinimal.h"

using boost::asio::ip::tcp;

/**
 * 
 */
class ROBOTBATTLE_API AgentSession : public std::enable_shared_from_this<AgentSession>
{
public:
	AgentSession(tcp::socket socket, AAgentPlayerController* AgentController);
	~AgentSession();

	void StartSession();

private:
	void ReadData();
	void WriteData(std::size_t length);

	tcp::socket socket_;
	enum { max_length = 1024 };
	char data_[max_length];

	AAgentPlayerController* AgentController;
};
