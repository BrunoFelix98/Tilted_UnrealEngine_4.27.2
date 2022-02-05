// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "SessionButton.generated.h"

/**
 * 
 */
UCLASS()
class USessionButton : public UButton
{
	GENERATED_BODY()

public:
	USessionButton();
	void SetSessionInfo(int i, class TCPClient* client);

protected:
	int sessionID;
	class TCPClient* tcpClient;

	UFUNCTION()
		void OnClick();
	
};
