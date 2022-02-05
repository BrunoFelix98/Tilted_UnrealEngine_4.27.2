// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class TILTED_API TCPClient : public FRunnable
{
public:
	TCPClient(class ATiltedPlayerState* pState);
	~TCPClient();
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	void CreateNewGameSession(FString sessionname);
	void JoinGameSession(int sessionID);
	bool IsConnected();

private:
	FRunnableThread* Thread;
	FSocket* Socket;
	FSocket* ListenerSocket;
	bool running;
	bool connected;

	class ATiltedPlayerState* PlayerState;

};