// Fill out your copyright notice in the Description page of Project Settings.

#include "TiltedPlayerState.h"
#include "TCPClient.h"
#include "Blueprint\UserWidget.h"
#include "Components\Button.h"
#include "Components\ScrollBox.h"
#include "Components\VerticalBox.h"
#include "Components\VerticalBoxSlot.h"
#include "Components\TextBlock.h"
#include "SessionButton.h"
#include "Gameframework\Controller.h"
#include "Kismet/GameplayStatics.h"

void ATiltedPlayerState::BeginPlay()
{
	FString levelname = GetWorld()->GetMapName();
	levelname.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *levelname);
	if (levelname == "MainMenuMap")
	{
		tcpClient = new TCPClient(this);

		canConnectToGameSession = false;

		sessionList = new TArray<FSessionInfo*>();

		//UUserWidget* MainMenu = Cast<UUserWidget>(GetWorld()->GetLevelScriptActor());
		MainMenuBP = CreateWidget<UUserWidget>(GetWorld(), MainMenuBP_Class);
		MainMenuBP->AddToViewport();

		UGameplayStatics::PlaySound2D(this, backgroundMusic);
		//UGameplayStatics::SpawnSoundAtLocation(this, backgroundMusic, GetActorLocation());

		UButton* NewSessionButton = Cast<UButton>(MainMenuBP->GetWidgetFromName(TEXT("CreateMatchBtn")));
		UButton* FindSessionButton = Cast<UButton>(MainMenuBP->GetWidgetFromName(TEXT("FindMatchBtn")));

		if (NewSessionButton)
		{
			NewSessionButton->OnClicked.AddDynamic(this, &ATiltedPlayerState::OnNewSessionClicked);
		}
		if (FindSessionButton)
		{
			FindSessionButton->OnClicked.AddDynamic(this, &ATiltedPlayerState::OnFindSessionClicked);
			ServerListScrollBox = Cast<UScrollBox>(MainMenuBP->GetWidgetFromName(TEXT("LobbiesScrollBox")));
		}
		GetWorld()->GetTimerManager().SetTimer(ServerListTimerHandler, this, &ATiltedPlayerState::OnUpdateServerList, 2, true);
	}
}

void ATiltedPlayerState::OnNewSessionClicked()
{
	tcpClient->CreateNewGameSession("Session");
}

void ATiltedPlayerState::OnFindSessionClicked()
{
	/*MainMenuBP->RemoveFromViewport();
	if (LobbiesBP_Class)
	{
		LobbiesBP = CreateWidget<UUserWidget>(GetWorld(), LobbiesBP_Class);
		LobbiesBP->AddToViewport();

		ServerListScrollBox = Cast<UScrollBox>(LobbiesBP->GetWidgetFromName(TEXT("LobbiesScrollBox")));

		GetWorld()->GetTimerManager().SetTimer(ServerListTimerHandler, this, &ATiltedPlayerState::OnUpdateServerList, 2, true);
	}*/
}

void ATiltedPlayerState::OnUpdateServerList()
{
	//UE_LOG(LogTemp, Warning, TEXT("UPDATING! 1"));
	if ((tcpClient) && (tcpClient->IsConnected()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("UPDATING! 2"));
		if (sessionList->Num() > 0)
		{
			//UE_LOG(LogTemp, Warning, TEXT("UPDATING! 3"));
			if (MainMenuBP)
			{
				//UE_LOG(LogTemp, Warning, TEXT("UPDATING! 4"));
				TArray<UWidget*> allButtons = ServerListScrollBox->GetAllChildren();
				for (int i = 0; i < allButtons.Num(); i++)
				{
					allButtons[i]->RemoveFromParent();
				}

				for (int i = 0; i < sessionList->Num(); i++)
				{
					UVerticalBox* ItemWidgetsBox = NewObject<UVerticalBox>();
					ServerListScrollBox->AddChild(ItemWidgetsBox);

					USessionButton* ItemWidget = NewObject<USessionButton>(this);
					ItemWidget->SetSessionInfo((*sessionList)[i]->id, tcpClient);

					UTextBlock* ItemWidgetText = NewObject<UTextBlock>();
					ItemWidgetText->SetText(FText::FromString((*sessionList)[i]->name));
					ItemWidget->AddChild(ItemWidgetText);
					UVerticalBoxSlot* Slot = ItemWidgetsBox->AddChildToVerticalBox(ItemWidget);
					static FMargin Padding(5);
					Slot->SetPadding(Padding);
				}
			}
		}
		if (canConnectToGameSession)
		{
			APlayerController* playerController = GetWorld()->GetFirstPlayerController();
			UE_LOG(LogTemp, Warning, TEXT("CONNECTING..."));

			if (playerController)
			{
				FString cmd = "open " + connectSessionInfo.ip + ":" + FString::FromInt(connectSessionInfo.port);

				UE_LOG(LogTemp, Warning, TEXT("%s"), *cmd);

				canConnectToGameSession = false;
				MainMenuBP->RemoveFromViewport();
				playerController->ConsoleCommand(cmd);
			}
		}
	}
}

void ATiltedPlayerState::UpdateSessionList(FString message)
{
	TArray<FString> Out;
	message.ParseIntoArray(Out, TEXT("|"), true);

	for (int i = 1; i < Out.Num() - 1; i += 2)
	{
		FSessionInfo* info = new FSessionInfo();
		info->id = FCString::Atoi(*Out[i]);
		info->name = Out[i + 1];
		UE_LOG(LogTemp, Warning, TEXT("%s"), *info->name);
		info->ip = "";
		info->port = -1;
		sessionList->Add(info);
	}

	UE_LOG(LogTemp, Warning, TEXT("%d"), sessionList->Num());
}

void ATiltedPlayerState::ConnectToGameSession(FSessionInfo session)
{
	connectSessionInfo = session;
	canConnectToGameSession = true;
}