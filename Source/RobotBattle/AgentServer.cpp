// Fill out your copyright notice in the Description page of Project Settings.


#include "AgentServer.h"

AgentServer::AgentServer()
{
}

AgentServer::~AgentServer()
{
}

void AgentServer::StartServer()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
}

