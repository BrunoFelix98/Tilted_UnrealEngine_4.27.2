// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TiltedGameMode.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStartEvent);

UCLASS()
class ATiltedGameMode : public AGameModeBase
{
	GENERATED_BODY()
		ATiltedGameMode();

		virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = TEXT("")) override;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class APawnCharacter> pawnToSpawn;

	class APlayerStart* FindPlayerStart(int spawnedPlayers);
	FName GetSpawnPointTagFromIndex(int spawnedPlayers);

	class APlayerStart* blueStart;
	class APlayerStart* redStart;
	

public:
	UPROPERTY(VisibleAnywhere)
	int playersSpawned;

	bool redCanWinGM;
	bool blueCanWinGM;

	UPROPERTY(VisibleAnywhere)
	class ALevelPlatform *platform;

	UPROPERTY()
		float timer = 0;

	UFUNCTION()
		void PlatformEnablePhysics();

	FOnGameStartEvent onGameStart;

	UFUNCTION()
		void Pause();

	UFUNCTION()
		void LevelComplete();

	UPROPERTY(EditAnywhere, Category = "Widget")
		TSubclassOf<class UUserWidget> PauseWidgetClass;
	class UUserWidget* PauseWidget;

	UPROPERTY(EditAnywhere, Category = "Widget")
		TSubclassOf<class UUserWidget> LevelCompleteWidgetClass;
	class UUserWidget* LevelCompleteWidget;

	UFUNCTION()
		void ChangeRedCanWinGMTrue();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
