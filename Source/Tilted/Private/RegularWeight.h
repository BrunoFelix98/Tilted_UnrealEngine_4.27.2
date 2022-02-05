// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RegularWeight.generated.h"

UCLASS()
class ARegularWeight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARegularWeight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase* pickUpSound;

	UFUNCTION()
		void NotifyActorBeginOverlap(AActor* OtherActor);
};
