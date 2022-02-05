// Fill out your copyright notice in the Description page of Project Settings.


#include "TCPClient.h"
#include "Sockets.h"
#include "SocketSubSystem.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "TiltedPlayerState.h"

TCPClient::TCPClient(ATiltedPlayerState* pState)
{
	Thread = FRunnableThread::Create(this, TEXT("TCPClientThread"), 0, TPri_Normal);
	PlayerState = pState;
}

TCPClient::~TCPClient()
{
	Stop();
	delete Thread;
}

bool TCPClient::Init()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
	int32 NewSize = 0;
	Socket->SetReceiveBufferSize(1024, NewSize);
	FIPv4Address matchmakingServerIP(127, 0, 0, 1);
	TSharedRef<FInternetAddr> matchmakingServer = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	matchmakingServer->SetIp(matchmakingServerIP.Value);
	matchmakingServer->SetPort(3434);

	connected = Socket->Connect(*matchmakingServer);

	if (connected)
	{
		UE_LOG(LogTemp, Warning, TEXT("CONNECTED!!!!"));
		FString message = TEXT("g|#");
		TCHAR* messageTChar = message.GetCharArray().GetData();

		int32 sent = 0;
		bool succeeded = Socket->Send((uint8*)TCHAR_TO_UTF8(messageTChar), FCString::Strlen(messageTChar), sent);
		if (succeeded)
		{
			UE_LOG(LogTemp, Warning, TEXT("MESSAGE SENT!"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("MESSAGE NOT SENT!"));
		}
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NOT CONNECTED!!!!"));
		return false;
	}

}

uint32 TCPClient::Run()
{
	running = true;
	TArray<uint8> ReceivedData;
	while (running)
	{
		uint32 Size = 0;
		if (Socket->HasPendingData(Size))
		{
			ReceivedData.Init(0, Size);
			int32 Read = 0;
			bool succeeded = Socket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);
			if (succeeded)
			{
				FString ServerMessage = FString(UTF8_TO_TCHAR(ReceivedData.GetData()));
				UE_LOG(LogTemp, Warning, TEXT("RECEIVED: %s"), *ServerMessage);
				if (ServerMessage[0] == 's')
				{
					//UPDATE SERSSION LIST
					PlayerState->UpdateSessionList(ServerMessage);
				}
				else if (ServerMessage[0] == 'o')
				{
					//CONNECT TO GAME SERVER
					TArray<FString> Out;
					ServerMessage.ParseIntoArray(Out, TEXT("|"), true);
					UE_LOG(LogTemp, Warning, TEXT("READY TO CONNECT!"));
					FSessionInfo session;
					session.id = -1;
					session.name = "";
					session.ip = Out[1];
					session.port = FCString::Atoi(*Out[2]);
					session.levelID = FCString::Atoi(*Out[3]);
					PlayerState->ConnectToGameSession(session);
					//UE_LOG(LogTemp, Warning, TEXT("IP: %s"), *Out[1]);
					//UE_LOG(LogTemp, Warning, TEXT("PORT: %s"), *Out[2]);
				}
				else if (ServerMessage[0] == 's')
				{
					//Use this to change the voted level
				}
			}
		}
	}
	return 0;
}

void TCPClient::Stop()
{
	UE_LOG(LogTemp, Warning, TEXT("STOP"));
	running = false;
}

void TCPClient::CreateNewGameSession(FString sessionname)
{
	FString message = TEXT("c|" + sessionname + "|#");
	TCHAR* messageTChar = message.GetCharArray().GetData();
	int32 sent = 0;
	bool succeeded = Socket->Send((uint8*)TCHAR_TO_UTF8(messageTChar), FCString::Strlen(messageTChar), sent);
	if (succeeded)
	{
		UE_LOG(LogTemp, Warning, TEXT("MESSAGE SENT!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MESSAGE NOT SENT!"));
	}
}

void TCPClient::JoinGameSession(int sessionID)
{
	FString message = TEXT("j|" + FString::FromInt(sessionID) + "|#");
	TCHAR* messageTChar = message.GetCharArray().GetData();
	int32 sent = 0;
	bool succeeded = Socket->Send((uint8*)TCHAR_TO_UTF8(messageTChar), FCString::Strlen(messageTChar), sent);
	if (succeeded)
	{
		UE_LOG(LogTemp, Warning, TEXT("MESSAGE SENT!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MESSAGE NOT SENT!"));
	}
}

bool TCPClient::IsConnected()
{
	return connected;
}