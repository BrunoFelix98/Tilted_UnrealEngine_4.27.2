// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TagAssigner.generated.h"

UCLASS()
class ATagAssigner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATagAssigner();

	UPROPERTY(EditAnywhere)
		FName color;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void NotifyActorBeginOverlap(AActor* OtherActor);

};
