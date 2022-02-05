// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();	
    if (IsHidden())
    {
        //SetActorTickEnabled(false);
        SetActorEnableCollision(false);
    }
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoor::Flip()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Obstacle Flipped")));
    if (IsHidden())
    {
        //Door started hidden, so we enable it

        //SetActorTickEnabled(true);
        SetActorHiddenInGame(false);
        SetActorEnableCollision(true);
    }
    else
    {
        //Door started shown, so we disable it

        //SetActorTickEnabled(false);
        SetActorHiddenInGame(true);
        SetActorEnableCollision(false);
    }
}

