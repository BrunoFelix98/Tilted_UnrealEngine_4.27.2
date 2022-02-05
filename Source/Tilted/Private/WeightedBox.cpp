// Fill out your copyright notice in the Description page of Project Settings.


#include "WeightedBox.h"
#include "PawnCharacter.h"
#include <Components/BoxComponent.h>
#include "Kismet/GameplayStatics.h"
#include "LevelPlatform.h"
#include "TiltedGameMode.h"

// Sets default values
AWeightedBox::AWeightedBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UStaticMeshComponent>("Box");
	SetRootComponent(Box);

	Trigger = CreateDefaultSubobject<UBoxComponent>("Trigger");
}

void AWeightedBox::OnGameStart()
{
	Box->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AWeightedBox::BeginPlay()
{
	Super::BeginPlay();
	
	platform = UGameplayStatics::GetActorOfClass(GetWorld(), ALevelPlatform::StaticClass());

	if (Trigger)
	{
		FScriptDelegate onTriggerEnterDelegate;
		onTriggerEnterDelegate.BindUFunction(this, "OnTriggerEnter");
		Trigger->OnComponentBeginOverlap.Add(onTriggerEnterDelegate);

		FScriptDelegate onTriggerExitDelegate;
		onTriggerExitDelegate.BindUFunction(this, "OnTriggerExit");
		Trigger->OnComponentEndOverlap.Add(onTriggerExitDelegate);
	}

	if (HasAuthority())
	{
		if (ATiltedGameMode* gameMode = Cast<ATiltedGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			FScriptDelegate eventDelegate;
			eventDelegate.BindUFunction(this, "OnGameStart");
			gameMode->onGameStart.Add(eventDelegate);
		}
	}

	//Assigns the mass (weight) of the box as the indicated weight type
	//Box->SetMassOverrideInKg(Box->GetFName(), currentWeight, true);
}

// Called every frame
void AWeightedBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (platform)
	{
		//Maintains actor's rotation equal to the platform's
		Box->GetAttachmentRootActor()->SetActorRotation(platform->GetActorRotation());

		//Apply a force perpendicular to the platform in such a way that forces the box to always be touching the floor
		Box->AddForce(platform->GetActorUpVector() * -200000);
	}

	//Assigns the mass (weight) of the box as the indicated weight type
	//Box->SetMassOverrideInKg(Box->GetFName(), currentWeight, true);
}

//Function is currently not being used, but could prove useful for future changes
void AWeightedBox::OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("Entered!")));
}

//Function is currently not being used, but could prove useful for future changes
void AWeightedBox::OnTriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("Exited!")));
}
