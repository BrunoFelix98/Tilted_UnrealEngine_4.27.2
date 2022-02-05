// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TiltedPlayerState.generated.h"

USTRUCT()
struct FSessionInfo {
	GENERATED_BODY()

		UPROPERTY()
		int id;

	UPROPERTY()
		FString name;

	UPROPERTY()
		FString ip;

	UPROPERTY()
		int port;

	UPROPERTY()
		int levelID;
};

UCLASS()
class TILTED_API ATiltedPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnNewSessionClicked();

	UFUNCTION()
		void OnFindSessionClicked();

	class TCPClient* tcpClient;
	TArray<FSessionInfo*>* sessionList;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UUserWidget> LobbiesBP_Class;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UUserWidget> MainMenuBP_Class;
	class UUserWidget* LobbiesBP;
	class UUserWidget* MainMenuBP;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	class UUserWidget* MainMenuBP2;

	class UScrollBox* ServerListScrollBox;

	FTimerHandle ServerListTimerHandler;

	UFUNCTION()
		void OnUpdateServerList();

	bool canConnectToGameSession;
	FSessionInfo connectSessionInfo;

public:
	void UpdateSessionList(FString message);
	void ConnectToGameSession(FSessionInfo session);

	UPROPERTY(EditAnywhere, Category = "Music")
		class USoundBase* backgroundMusic;
};

