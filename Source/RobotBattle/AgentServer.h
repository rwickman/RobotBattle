// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SocketSubsystem.h"
#include "Sockets.h"
#include "CoreMinimal.h"

/**
 * 
 */
class ROBOTBATTLE_API AgentServer
{
public:
	AgentServer();
	~AgentServer();

	void StartServer();

private:
	FSocket* Socket;
};
