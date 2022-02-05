// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnCharacter.generated.h"

UCLASS()
class APawnCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* CylinderCharacter;

	void PhysicsMovementForward(float move);
	void PhysicsMovementSideways(float move);

	UFUNCTION(Server, Unreliable)
		void MoveForwardServer(float move, FVector camVector);

	UFUNCTION(Server, Unreliable)
		void MoveSidewaysServer(float move, FVector camVector);

	UFUNCTION()
		void OnGameStart();
	

	//Assign in scene
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		AActor* platform;
	//TArray<AActor*> FoundPlatforms;

	//Assing Goal in code
	UPROPERTY()
		FName refTag;

	//Weight classes
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weight")
		float normalWeight = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weight")
		float lightWeight = 33.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weight")
		float heavyWeight = 300.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weight")
		float currentWeight = normalWeight;

	
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UUserWidget> HUDWidgetClass;
	class UUserWidget* HUDWidget;

	UPROPERTY(EditAnywhere, Category = "HUD")
		UTexture2D *regularWeightIcon;

	UPROPERTY(EditAnywhere, Category = "HUD")
		UTexture2D *lightWeightIcon;

	UPROPERTY(EditAnywhere, Category = "HUD")
		UTexture2D *heavyWeightIcon;



protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void PauseGame();

	UFUNCTION(Server, Reliable)
		virtual void Server_PauseGame();

	UFUNCTION()
		virtual void ClearLevelCheat();

	UFUNCTION(Server, Reliable)
		virtual void Server_ClearLevelCheat();

	UFUNCTION()
		virtual void Open();

	UFUNCTION(Server, Reliable)
		virtual void Server_Open();

	UFUNCTION()
		virtual void ChangeColor();

	UFUNCTION(Server, Reliable)
		virtual void Server_ChangeColor();

	class IInteractions* InteractableObj;

private:

	FVector ForwardMovementDirection;
	FVector SidewaysMovementDirection;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float MovementSpeed = 500.0f;
	
};
