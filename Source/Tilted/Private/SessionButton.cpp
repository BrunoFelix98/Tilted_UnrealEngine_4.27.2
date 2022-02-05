// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionButton.h"
#include "TCPClient.h"

USessionButton::USessionButton()
{
	OnClicked.AddDynamic(this, &USessionButton::OnClick);
}

void USessionButton::SetSessionInfo(int i, class TCPClient* client)
{
	sessionID = i;
	tcpClient = client;
}

void USessionButton::OnClick()
{
	tcpClient->JoinGameSession(sessionID);
}
