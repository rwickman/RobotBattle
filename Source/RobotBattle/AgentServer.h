// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

THIRD_PARTY_INCLUDES_START
#pragma push_macro("check")
#undef check

#pragma push_macro("TEXT")
#undef TEXT

#include <boost/asio.hpp>

#pragma pop_macro("check")
#pragma pop_macro("TEXT")
THIRD_PARTY_INCLUDES_END
#include "AgentPlayerController.h"
#include "CoreMinimal.h"

using boost::asio::ip::tcp;


class ROBOTBATTLE_API AgentServer
{
public:
	AgentServer(boost::asio::io_context& io_context, std::vector<AAgentPlayerController*> AgentControllers);
	~AgentServer();

	void StartListening();


private:
	void AcceptConnection();
	tcp::acceptor acceptor_;
	std::vector<AAgentPlayerController*> AgentControllers;

};
