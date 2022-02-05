// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Interactions.h"
#include "Lever2.h"
#include "LevelPlatform.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "TiltedGameMode.h"
#include "Blueprint\UserWidget.h"
#include "Components\Image.h"

// Sets default values
APawnCharacter::APawnCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CylinderCharacter = CreateDefaultSubobject<UStaticMeshComponent>("CylinderCharacter");
	SetRootComponent(CylinderCharacter);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm Component");
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(CylinderCharacter);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera Component");
	CameraComponent->SetupAttachment(SpringArmComponent);

}

void APawnCharacter::OnGameStart()
{
	CylinderCharacter->SetSimulatePhysics(true);
}

void APawnCharacter::MoveForwardServer_Implementation(float move, FVector camVector)
{
	ForwardMovementDirection.X = FMath::Clamp(move * camVector.X, -1.0f, 1.0f);
	ForwardMovementDirection.Y = FMath::Clamp(move * camVector.Y, -1.0f, 1.0f);
	ForwardMovementDirection.Z = 0.0f;
}

void APawnCharacter::MoveSidewaysServer_Implementation(float move, FVector camVector)
{
	SidewaysMovementDirection.X = FMath::Clamp(-(move * camVector.Y), -1.0f, 1.0f);
	SidewaysMovementDirection.Y = FMath::Clamp(move * camVector.X, -1.0f, 1.0f);
	SidewaysMovementDirection.Z = 0.0f;
}

void APawnCharacter::PhysicsMovementForward(float move)
{
	MoveForwardServer(move, CameraComponent->GetComponentRotation().Vector());
}

void APawnCharacter::PhysicsMovementSideways(float move)
{
	MoveSidewaysServer(move, CameraComponent->GetComponentRotation().Vector());
}

void APawnCharacter::PauseGame()
{
	Server_PauseGame();
}

void APawnCharacter::Server_PauseGame_Implementation()
{
	ATiltedGameMode* gameMode = Cast<ATiltedGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->Pause();
}

void APawnCharacter::ClearLevelCheat()
{
	Server_ClearLevelCheat();
}

void APawnCharacter::Server_ClearLevelCheat_Implementation()
{
	ATiltedGameMode* gameMode = Cast<ATiltedGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->LevelComplete();
}

void APawnCharacter::Open()
{
	Server_Open();
}

void APawnCharacter::Server_Open_Implementation()
{
	if (InteractableObj)
	{
		InteractableObj->Interact();
	}
}

void APawnCharacter::ChangeColor()
{
	//Server_ChangeColor();


	if (refTag == "Red")
	{
		CylinderCharacter->UPrimitiveComponent::SetCustomDepthStencilValue(6);
	}
	if (refTag == "Blue")
	{
		CylinderCharacter->UPrimitiveComponent::SetCustomDepthStencilValue(5);
	}
}

void APawnCharacter::Server_ChangeColor_Implementation()
{
	if (refTag == "Red")
	{
		CylinderCharacter->UPrimitiveComponent::SetCustomDepthStencilValue(6);
	}
	if (refTag == "Blue")
	{
		CylinderCharacter->UPrimitiveComponent::SetCustomDepthStencilValue(5);
	}
}


// Called when the game starts or when spawned
void APawnCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	platform = UGameplayStatics::GetActorOfClass(GetWorld(), ALevelPlatform::StaticClass());
	if (GetWorld()->IsServer())
	{
		if (ATiltedGameMode* gameMode = Cast<ATiltedGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			FScriptDelegate eventDelegate;
			eventDelegate.BindUFunction(this, "OnGameStart");
			gameMode->onGameStart.Add(eventDelegate);
		}
	}
	
	//Adds the UI widget to the screen
	HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
	HUDWidget->AddToViewport();


	//this->Tags.Add("Player");
}

// Called every frame
void APawnCharacter::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	
	UImage* BlueIcon = Cast<UImage>(HUDWidget->GetWidgetFromName(TEXT("BlueWeight")));
	UImage* RedIcon = Cast<UImage>(HUDWidget->GetWidgetFromName(TEXT("RedWeight")));

	if (platform)
	{
		//If the inclination of the platform remains below 30º
		if (FMath::Abs(platform->GetActorRotation().Roll) <= 30 && FMath::Abs(platform->GetActorRotation().Pitch) <= 30)
		{
			//Move pawn character
			if (!ForwardMovementDirection.IsZero())
			{
				const FVector NewLocation = GetActorLocation() + (ForwardMovementDirection * DeltaTime * MovementSpeed);
				SetActorLocation(NewLocation);
			}
			if (!SidewaysMovementDirection.IsZero())
			{
				const FVector NewLocation = GetActorLocation() + (SidewaysMovementDirection * DeltaTime * MovementSpeed);
				SetActorLocation(NewLocation);
			}

			//Maintains actor's rotation equal to the platform's
			CylinderCharacter->GetAttachmentRootActor()->SetActorRotation(platform->GetActorRotation());

			//Apply a force perpendicular to the platform in such a way that forces the pawn character to always be touching the floor
			CylinderCharacter->AddForce(platform->GetActorUpVector() * -200000); //999999
																		

		}
		//If the inclination of the platform exceeds 30º
		//Initiate losing condition
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Platform too tilted!")));

			MovementSpeed = 0;

			//Apply a downwad force
			CylinderCharacter->AddForce(GetGravityDirection() * 999999);
			//Apply a force perpendicular to the platform that slightly pushes the pawn character away from it, forcing it to "roll" off
			CylinderCharacter->AddForce(platform->GetActorUpVector() * 500000);

		}

	}
	//ChangeColor();

	//Keeps the mass of the pawn character as the indicated weight type
	CylinderCharacter->SetMassOverrideInKg(CylinderCharacter->GetFName(), currentWeight, true);

	//Update UI
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnCharacter::StaticClass(), Players);

	
	for (int i = 0; i < Players.Num(); i++)
	{
		APawnCharacter* P = Cast<APawnCharacter>(Players[i]);
		if (P->refTag == "Blue")
		{
			if (P->CylinderCharacter->GetMass() == 100.0f)
			{
				BlueIcon->SetBrushFromTexture(regularWeightIcon);
			}
			if (P->CylinderCharacter->GetMass() == 300.0f)
			{
				BlueIcon->SetBrushFromTexture(heavyWeightIcon);
			}
			if (P->CylinderCharacter->GetMass() == 33.0f)
			{
				BlueIcon->SetBrushFromTexture(lightWeightIcon);
			}
		}
		else if (P->refTag == "Red")
		{
			if (P->CylinderCharacter->GetMass() == 100.0f)
			{
				RedIcon->SetBrushFromTexture(regularWeightIcon);
			}
			if (P->CylinderCharacter->GetMass() == 300.0f)
			{
				RedIcon->SetBrushFromTexture(heavyWeightIcon);
			}
			if (P->CylinderCharacter->GetMass() == 33.0f)
			{
				RedIcon->SetBrushFromTexture(lightWeightIcon);
			}
		}
	}
	/*
	if (this->refTag == "Blue")
	{
		if (CylinderCharacter->GetMass() == 100.0f)
		{
			BlueIcon->SetBrushFromTexture(regularWeightIcon);
		}
		if (CylinderCharacter->GetMass() == 300.0f)
		{
			BlueIcon->SetBrushFromTexture(heavyWeightIcon);
		}
		if (CylinderCharacter->GetMass() == 33.0f)
		{
			BlueIcon->SetBrushFromTexture(lightWeightIcon);
		}
	}
	if (this->refTag == "Red")
	{
		if (CylinderCharacter->GetMass() == 100.0f)
		{
			RedIcon->SetBrushFromTexture(regularWeightIcon);
		}
		if (CylinderCharacter->GetMass() == 300.0f)
		{
			RedIcon->SetBrushFromTexture(heavyWeightIcon);
		}
		if (CylinderCharacter->GetMass() == 33.0f)
		{
			RedIcon->SetBrushFromTexture(lightWeightIcon);
		}
	}
	*/
}

// Called to bind functionality to input
void APawnCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("PhysicsMovementForward", this, &APawnCharacter::PhysicsMovementForward);
	PlayerInputComponent->BindAxis("PhysicsMovementSideways", this, &APawnCharacter::PhysicsMovementSideways);
	PlayerInputComponent->BindAxis("LookUp", this, &APawnCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawnCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("EPress", EInputEvent::IE_Pressed, this, &APawnCharacter::Open);
	PlayerInputComponent->BindAction("Pause", EInputEvent::IE_Pressed, this, &APawnCharacter::PauseGame);
	PlayerInputComponent->BindAction("ClearLevelCheat", EInputEvent::IE_Pressed, this, &APawnCharacter::ClearLevelCheat);
}

