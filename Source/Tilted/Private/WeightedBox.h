// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeightedBox.generated.h"

UCLASS()
class AWeightedBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeightedBox();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* Box;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* Trigger;

	//Assign in scene
	UPROPERTY(EditAnywhere, Category = "Components")
		AActor* platform;

	//Weight classes
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weight")
		float normalWeight = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weight")
		float lightWeight = 33.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weight")
		float heavyWeight = 300.0f;
	UPROPERTY(EditAnywhere, Category = "Weight" )
		float currentWeight = normalWeight;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnGameStart();

	UFUNCTION()
	void OnTriggerEnter(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerExit(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

};
