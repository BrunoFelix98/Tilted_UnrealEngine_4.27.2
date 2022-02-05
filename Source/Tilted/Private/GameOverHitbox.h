// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameOverHitbox.generated.h"

UCLASS()
class AGameOverHitbox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameOverHitbox();

	UPROPERTY(EditAnywhere, Category = "Widget")
		TSubclassOf<class UUserWidget> GameOverWidgetClass;
	class UUserWidget* GameOverWidget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void NotifyActorBeginOverlap(AActor* OtherActor);
};
