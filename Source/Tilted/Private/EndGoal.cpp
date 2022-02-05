// Fill out your copyright notice in the Description page of Project Settings.


#include "EndGoal.h"
#include "Kismet/GameplayStatics.h"
#include "PawnCharacter.h"
#include "TiltedGameMode.h"

// Sets default values
AEndGoal::AEndGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEndGoal::BeginPlay()
{
	Super::BeginPlay();
    
    redCanWin = false;
    blueCanWin = false;

    iRedCanWin = 0;
    iBlueCanWin = 0;
}

// Called every frame
void AEndGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (redCanWin)
    {
        redCounter++;
    }
    if (blueCanWin)
    {
        blueCounter++;
    }

   // GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, FString::Printf(TEXT("Bool: %s"), redCanWin ? TEXT("true") : TEXT("false")));
    //GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(TEXT("%d"), redCounter));
    
    
    if (redCanWin && blueCanWin)
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Level completed!")));
    }
    
}



void AEndGoal::NotifyActorBeginOverlap(AActor* OtherActor)
{

    if (OtherActor->IsA<APawnCharacter>())
    {
        if (OtherActor->ActorHasTag(color))
        {
            if (color == "Blue")
            {
                //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Blue Reached The Goal!")));
                blueCanWin = true;
              
            }

            if (color == "Red")
            {
                //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Red Reached The Goal!")));
                redCanWin = true;
               
            }

        }
    }
}

void AEndGoal::NotifyActorEndOverlap(AActor* OtherActor)
{
    if (OtherActor->IsA<APawnCharacter>())
    {
        if (OtherActor->ActorHasTag(color))
        {
            if (color == "Blue")
            {
                //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Blue Exited The Goal!")));
                blueCanWin = false;
                //blueCounter = 0;
                
            }

            if (color == "Red")
            {
                //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Red Exited The Goal!")));
                redCanWin = false;
                //redCounter = 0;
                
            }
        }
    }
}

