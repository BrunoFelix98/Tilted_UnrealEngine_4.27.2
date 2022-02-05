// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelPlatform.h"
#include "TiltedGameMode.h"
#include "PawnCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Engine/EngineTypes.h"

// Sets default values
ALevelPlatform::ALevelPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>("Platform");
	SetRootComponent(PlatformMesh);

	
}

void ALevelPlatform::onTimerEnd()
{
	PlatformMesh->SetSimulatePhysics(true);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Can u see me??")));
}

void ALevelPlatform::OnGameStart()
{
	PlatformMesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void ALevelPlatform::BeginPlay()
{
	Super::BeginPlay();

	tiltedGameMode = (ATiltedGameMode*)GetWorld()->GetAuthGameMode();

	if (HasAuthority())
	{
		if (ATiltedGameMode* gameMode = Cast<ATiltedGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			FScriptDelegate eventDelegate;
			eventDelegate.BindUFunction(this, "OnGameStart");
			gameMode->onGameStart.Add(eventDelegate);
		}
	}
	//GetWorld()->GetTimerManager().SetTimer(spawnTimer, this, &ALevelPlatform::onTimerEnd, 2.0f, false);
	//PlatformMesh->SetSimulatePhysics(true);
}

// Called every frame
void ALevelPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	/*if (!PlatformMesh->IsSimulatingPhysics(PlatformMesh->GetFName()))
	{
		if (tiltedGameMode != nullptr)
		{
			if (tiltedGameMode->GetNumPlayers() > 1)
			{
				timer++;
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("%f"), timer));
				if (timer >= 50.0f)
					PlatformMesh->SetSimulatePhysics(true);
			}
		}
	}*/
	
}

