// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelPlatform.generated.h"

UCLASS()
class ALevelPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelPlatform();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* PlatformMesh;

	class ATiltedGameMode* tiltedGameMode;

	UFUNCTION()
		void onTimerEnd();

	UFUNCTION()
		void OnGameStart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY()
	FTimerHandle spawnTimer;

	UPROPERTY()
		float timer = 0;
};
