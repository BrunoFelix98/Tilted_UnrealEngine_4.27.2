// Fill out your copyright notice in the Description page of Project Settings.


#include "TiltedGameMode.h"
#include "PawnCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "LevelPlatform.h"
#include "Blueprint\UserWidget.h"

ATiltedGameMode::ATiltedGameMode()
{
	SetActorTickEnabled(true);
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	timer = 1;
}

FString ATiltedGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	FString returnValue = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
	
	APlayerStart* playerStart = FindPlayerStart(playersSpawned);

	if (playerStart)
	{
		FVector locationToSpawn = playerStart->GetActorLocation();
		FRotator rotationToSpawn = playerStart->GetActorRotation();
		FActorSpawnParameters spawnParameters;
		spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		APawn* spawnedPawn = Cast<APawn>(GetWorld()->SpawnActor(pawnToSpawn, &locationToSpawn, &rotationToSpawn, spawnParameters));



		NewPlayerController->Possess(spawnedPawn);

		spawnedPawn->SetOwner(NewPlayerController);
		playersSpawned++;
		spawnedPawn->Tags.Add(GetSpawnPointTagFromIndex(playersSpawned));
		return returnValue;
	}
	
	returnValue = "Couldn't find playerStart";
	return returnValue;
}

APlayerStart* ATiltedGameMode::FindPlayerStart(int spawnedPlayers)
{
	FName spawnPointTag = GetSpawnPointTagFromIndex(spawnedPlayers);
	TArray<AActor*> SpawnPoint;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), spawnPointTag, SpawnPoint);


	for (int i = 0; i < SpawnPoint.Num(); i++)
	{
		if (SpawnPoint[i] && SpawnPoint[i]->IsA<APlayerStart>())
		{
			return Cast<APlayerStart>(SpawnPoint[i]);
		}
	}

	return nullptr;
}

FName ATiltedGameMode::GetSpawnPointTagFromIndex(int spawnedPlayers)
{
	if (spawnedPlayers % 2 == 0)
	{
		//OtherActor->Tags.Add(color);
		return "Blue";
	}
		
	else
	{
		return "Red";
	}
		

}

void ATiltedGameMode::PlatformEnablePhysics()
{
	
	if (!platform->PlatformMesh->IsSimulatingPhysics())
	{
		if (this->GetNumPlayers() > 1 && timer <= 0)
		{
			onGameStart.Broadcast();
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("%d"), timer++));
			//if (timer >= 50.0f)
			/*
				platform->PlatformMesh->SetSimulatePhysics(true);
				platform->PlatformMesh->WakeAllRigidBodies();
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("%b"), platform->PlatformMesh->IsSimulatingPhysics()));
			*/
			
		}
	}
}

void ATiltedGameMode::Pause()
{
	if (!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		//Freezes time
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		UGameplayStatics::IsGamePaused(GetWorld());

		//Adds the UI widget to the screen
		PauseWidget = CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass);
		PauseWidget->AddToViewport();

		//Show mouse cursor
		
	}
	else if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		//remove widget
		PauseWidget->RemoveFromViewport();

		//Hide mouse cursor


		//Unfreeze time
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		
	}
}

void ATiltedGameMode::LevelComplete()
{
	//Freezes time
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	UGameplayStatics::IsGamePaused(GetWorld());

	//Adds the UI widget to the screen
	LevelCompleteWidget = CreateWidget<UUserWidget>(GetWorld(), LevelCompleteWidgetClass);
	LevelCompleteWidget->AddToViewport();
}

void ATiltedGameMode::ChangeRedCanWinGMTrue()
{
	redCanWinGM = true;
}

// Called when the game starts or when spawned
void ATiltedGameMode::BeginPlay()
{
	Super::BeginPlay();
	platform = Cast<ALevelPlatform>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelPlatform::StaticClass()));
}

// Called every frame
void ATiltedGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	timer -= DeltaTime;
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("Can you see this?")));
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("%d"), playersSpawned));

	PlatformEnablePhysics();
}
