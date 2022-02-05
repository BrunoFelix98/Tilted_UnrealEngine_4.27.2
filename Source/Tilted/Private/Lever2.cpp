
#include "Lever2.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "PawnCharacter.h"
#include "Components/ShapeComponent.h"
#include "Interactable.h"


// Sets default values
ALever2::ALever2()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	timer = 5;
}

// Called when the game starts or when spawned
void ALever2::BeginPlay()
{
	Super::BeginPlay();

	offset = 150.0f;

	playerClose = false;

	EPress = (UWidgetComponent*)GetComponentByClass(UWidgetComponent::StaticClass());

	EPress->SetVisibility(false);

	playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	for (int i = 0; i < obsActors.Num(); i++)
	{
		IInteractable* tempActors = (IInteractable*)(obsActors[i]->GetInterfaceAddress(UInteractable::StaticClass()));

		if (tempActors)
		{
			obstacles.Add(tempActors);
		}
	}

	FScriptDelegate OnTriggerDelegateIn;
	FScriptDelegate OnTriggerDelegateOut;

	OnTriggerDelegateIn.BindUFunction(this, "OnOverlapBegin");
	OnTriggerDelegateOut.BindUFunction(this, "OnOverlapEnd");

	Trigger->OnComponentBeginOverlap.Add(OnTriggerDelegateIn);
	Trigger->OnComponentEndOverlap.Add(OnTriggerDelegateOut);
}

// Called every frame
void ALever2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	timer -= DeltaTime;

	if (timer <= 0)
	{
		if (!EPress)
		{
			return;
		}

		APawn* player = GetWorld()->GetFirstPlayerController()->GetPawn();

		FVector currentCamera;

		GetActiveCamera(currentCamera);

		FVector direction = currentCamera - EPress->GetComponentLocation();
		direction.Normalize();
		FRotator lookAtRotator = FRotationMatrix::MakeFromX(direction).Rotator();
		EPress->SetWorldRotation(lookAtRotator);

		if (player)
		{
			playerLocation = player->GetActorLocation();
		}

		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("%f"), GetActorLocation().Distance(GetActorLocation(), playerLocation)));
		IsPlayerClose();
	}
}

bool ALever2::IsPlayerClose()
{
	EPress->SetVisibility(false);

	if ((GetActorLocation().Distance(GetActorLocation(), playerLocation) <= offset))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Player entered lever surroundings"));
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("Player entered lever surroundings")));
		playerClose = true;
		EPress->SetVisibility(true);
	}

	/*if (IsInputKeyDown(EKeys::E))
	{
		Interact();
	}*/

	/*float Dotx = FVector::DotProduct(EPress->GetRelativeLocation(), playerLocation);
	float angleX = acosf(Dotx);

	FVector forward = EPress->GetForwardVector();
	float angle = angleX;
	float rotatedXRight = forward.X * FMath::Cos(FMath::DegreesToRadians(angle)) + forward.Z * FMath::Sin(FMath::DegreesToRadians(angle));
	float rotatedZRight = -forward.X * FMath::Sin(FMath::DegreesToRadians(angle)) + forward.Z * FMath::Cos(FMath::DegreesToRadians(angle));

	FVector newRotatedVectorRight = FVector(rotatedXRight, forward.Y, rotatedZRight);

	EPress->GetRelativeTransform().InverseTransformVector(newRotatedVectorRight);*/

	return playerClose;
}

void ALever2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("EPress", EInputEvent::IE_Pressed, this, &ALever2::Interact);
}

void ALever2::GetActiveCamera(FVector& def)
{
	if (playerController)
	{
		def = playerController->PlayerCameraManager->GetCameraLocation();
	}
}

void ALever2::Interact()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Can u see me??")));
	for (int i = 0; i < obstacles.Num(); i++)
	{
		if (obstacles[i])
		{
			obstacles[i]->Flip();
			UGameplayStatics::SpawnSoundAtLocation(this, buttonSound, GetActorLocation());
		}
	}
}


void ALever2::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawnCharacter* player = Cast<APawnCharacter>(OtherActor);
	if (player)
	{
		player->InteractableObj = this;
		//Tell player that they is close
	}
}

void ALever2::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawnCharacter* player = Cast<APawnCharacter>(OtherActor);
	if (player)
	{
		player->InteractableObj = nullptr;
		//Tell player that they left
	}
}