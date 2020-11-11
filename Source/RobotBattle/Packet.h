// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ROBOTBATTLE_API Packet
{
public:
	Packet();
	~Packet();

	char* GetData();

	std::size_t Length() const;

	char* GetBody();

	void GetBodyContent(char* BodyContent);

	std::size_t GetBodyLength() const;

	void SetBodyLength(std::size_t NewBodyLength);

	void SetBody(const char* BodyContent);
	
	bool DecodeHeader();

	void EncodeHeader();

	static const int HEADER_LENGTH = 4;
	static const int MAX_BODY_LENGTH = 4096;

protected:
	char Data[HEADER_LENGTH + MAX_BODY_LENGTH];
	std::size_t BodyLength;

};
