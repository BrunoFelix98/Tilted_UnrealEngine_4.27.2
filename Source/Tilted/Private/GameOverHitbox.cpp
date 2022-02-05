// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverHitbox.h"
#include "Kismet/GameplayStatics.h"
#include "PawnCharacter.h"
#include "Blueprint\UserWidget.h"

// Sets default values
AGameOverHitbox::AGameOverHitbox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AGameOverHitbox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameOverHitbox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AGameOverHitbox::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->IsA<APawnCharacter>())
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Game Over!")));

		//Freezes time
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		UGameplayStatics::IsGamePaused(GetWorld());

		//Adds the UI widget to the screen
		GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
		GameOverWidget->AddToViewport();
	}
}

