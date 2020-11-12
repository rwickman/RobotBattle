// Fill out your copyright notice in the Description page of Project Settings.


#include "Packet.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>


Packet::Packet()
{
}

Packet::~Packet()
{
}


char* Packet::GetData()
{
	return Data;
}

std::size_t Packet::Length() const
{
	return HEADER_LENGTH + BodyLength;
}

char* Packet::GetBody()
{
	return Data + HEADER_LENGTH;
}

void Packet::GetBodyContent(char* BodyContent)
{
	std::memcpy(BodyContent, GetBody(), BodyLength);
}

std::size_t Packet::GetBodyLength() const
{
	return BodyLength;
}

void Packet::SetBodyLength(std::size_t NewBodyLength)
{
	BodyLength = NewBodyLength;
	if (BodyLength > MAX_BODY_LENGTH)
	{
		BodyLength = MAX_BODY_LENGTH;
	}
	/*
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("IN SetBodyLength MAX_BODY_LENGTH is %d"), MAX_BODY_LENGTH) );
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("IN SetBodyLength BodyLength is %d"), BodyLength) );
	}*/
}

void Packet::SetBody(const char* BodyContent)
{
	/*
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("std::strlen(BodyContent) is %d"), std::strlen(BodyContent)));
	}
	*/
	SetBodyLength(std::strlen(BodyContent));
	std::memcpy(GetBody(), BodyContent, BodyLength);
	EncodeHeader();
	/*
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("After EncodeHeader BodyLength is %d"), BodyLength));
	}*/
}

bool Packet::DecodeHeader()
{
	char Header[HEADER_LENGTH + 1] = "";
	std::strncat(Header, Data, HEADER_LENGTH);
	BodyLength = std::atoi(Header);
	/*
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::FromInt(BodyLength));
	}
	*/
	if (BodyLength > MAX_BODY_LENGTH)
	{
		BodyLength = 0;
		return false;
	}
	return true;
}

void Packet::EncodeHeader()
{
	char Header[HEADER_LENGTH + 1] = "";
	// Write body length and append blank if necessary
	std::sprintf(Header, "%4d", static_cast<int>(BodyLength));
	std::memcpy(Data, Header, HEADER_LENGTH);
}