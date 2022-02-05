// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Interactions.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Lever2.generated.h"

UCLASS()
class TILTED_API ALever2 : public APawn, public IInteractions
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALever2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase* buttonSound;

	UPROPERTY(EditAnywhere)
		FVector playerLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UShapeComponent* Trigger;

	UPROPERTY(EditAnywhere)
		float offset;

	UPROPERTY(EditAnywhere)
		bool playerClose;

	UPROPERTY(EditAnywhere)
		class UWidgetComponent* EPress;

	UPROPERTY()
		class APlayerController* playerController;

	UPROPERTY(EditAnywhere, Category = "Info")
		TArray<class AActor*> obsActors;

	UPROPERTY()
		float timer = 0;

	TArray<class IInteractable*> obstacles;

	UFUNCTION()
		bool IsPlayerClose();

	UFUNCTION()
		void GetActiveCamera(FVector& def);

	UFUNCTION(BlueprintCallable)
		virtual void Interact() override;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};