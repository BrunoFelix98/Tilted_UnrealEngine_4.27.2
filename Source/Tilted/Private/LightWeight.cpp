// Fill out your copyright notice in the Description page of Project Settings.


#include "LightWeight.h"
#include "PawnCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALightWeight::ALightWeight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALightWeight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALightWeight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALightWeight::NotifyActorBeginOverlap(AActor* OtherActor)
{
    if (OtherActor->IsA<APawnCharacter>())
    {
        UGameplayStatics::SpawnSoundAtLocation(this, pickUpSound, GetActorLocation());
        Cast<APawnCharacter>(OtherActor)->currentWeight = Cast<APawnCharacter>(OtherActor)->lightWeight;
        Destroy();
    }
}
