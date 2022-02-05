// Fill out your copyright notice in the Description page of Project Settings.


#include "TagAssigner.h"
#include "Kismet/GameplayStatics.h"
#include "PawnCharacter.h"

// Sets default values
ATagAssigner::ATagAssigner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATagAssigner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATagAssigner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATagAssigner::NotifyActorBeginOverlap(AActor* OtherActor)
{
    if (OtherActor->IsA<APawnCharacter>())
    {
        if (!OtherActor->ActorHasTag(color))
        {
            TArray<AActor*> FoundBase;
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATagAssigner::StaticClass(), FoundBase);

            int lenght = FoundBase.Num();
            int count = 0;

            for (int i = 0; i < lenght; i++)
            {
                if (!OtherActor->ActorHasTag(Cast<ATagAssigner>(FoundBase[i])->color))
                {
                    count++;
                }
                if (count == lenght)
                {
                    if (color == "Blue")
                    {
                       // Cast<APawnCharacter>(OtherActor)->materi = color;
                        //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Character is now Blue")));
                        Cast<APawnCharacter>(OtherActor)->CylinderCharacter->UPrimitiveComponent::SetCustomDepthStencilValue(5);
                    }
                    if (color == "Red")
                    {
                        //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Character is now Red")));
                        Cast<APawnCharacter>(OtherActor)->CylinderCharacter->UPrimitiveComponent::SetCustomDepthStencilValue(6);
                    }
                    OtherActor->Tags.Add(color);
                    Cast<APawnCharacter>(OtherActor)->refTag = color;
                }
            }
        }
    }
}

