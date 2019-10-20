// (c) Dorian Zgraggen 2019, dorianzgraggen.com


#include "Lift.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Engine/TriggerVolume.h" 	
#include "Kismet/GameplayStatics.h"
#include "Engine/EngineTypes.h"

// Sets default values
ALift::ALift()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Main = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main"));
	SetRootComponent(Main);

	InnerDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InnerDoor"));
	InnerDoor->SetupAttachment(Main);
	
	OuterDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OuterDoor"));
	OuterDoor->SetupAttachment(Main);
}

// Called when the game starts or when spawned
void ALift::BeginPlay()
{
	Super::BeginPlay();

	StartPosition = GetActorLocation();
	EndPosition = StartPosition + FVector(0, 0, HeightDifference);

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	InnerDoor->AddWorldOffset(DoorDirection);
	OuterDoor->AddWorldOffset(DoorDirection * 0.5);

}

// Called every frame
void ALift::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//SetActorLocation(EndPosition);

	MoveLiftToPlayer();

	// Going up
	if (OpenLowerVolume->IsOverlappingActor(PlayerPawn) && bDoorsClosed && !bDoorsShouldOpen && bPlayerWasOutside)
	{
		bDoorsShouldOpen = true;
		UGameplayStatics::PlaySoundAtLocation(this, OpenSound, FVector(1826, 1844, 186));
	}

	if (CloseLowerVolume->IsOverlappingActor(PlayerPawn) && !bDoorsClosed && !bDoorsShouldClose && !bAwaitLeaving)
	{
		bDoorsShouldClose = true;
		UGameplayStatics::PlaySoundAtLocation(this, CloseSound, FVector(1826, 1844, 186));
	}

	if (CloseLowerVolume->IsOverlappingActor(PlayerPawn) && bDoorsClosed && !bDoorsShouldClose && !bLiftShouldMove && !bAwaitLeaving)
	{
		bLiftShouldMove = true;
		bPlayerWasOutside = false;
		bPlayerIsInside = true;
			UGameplayStatics::PlaySound2D(this, MainSound);
	}

	if (bAwaitLeaving && !CloseUpperVolume->IsOverlappingActor(PlayerPawn) && !bDoorsClosed && !CloseLowerVolume->IsOverlappingActor(PlayerPawn))
	{
		bDoorsShouldClose = true;
		bAwaitLeaving = false;
		bPlayerIsInside = false;
		//bPlayerWasOutside = true;
		UGameplayStatics::PlaySoundAtLocation(this, CloseSound, FVector(1826, 1844, 186 + HeightDifference));
	}

	//if (!bPlayerIsInside && !OpenUpperVolume->IsOverlappingActor(PlayerPawn))
	//{
	//	bPlayerWasOutside = true;
	//}

	// Handle going away
	if (!bPlayerIsInside && !bDoorsClosed && !OpenLowerVolume->IsOverlappingActor(PlayerPawn) && !bDoorsShouldClose && !bLiftIsUpstairs)
	{
		bDoorsShouldClose = true;
		UGameplayStatics::PlaySoundAtLocation(this, CloseSound, FVector(1826, 1844, 186));
	}

	// Going down ////////////////////////////

	if (OpenUpperVolume->IsOverlappingActor(PlayerPawn) && bDoorsClosed && !bDoorsShouldOpen && bPlayerWasOutside)
	{
		bDoorsShouldOpen = true;
		//bAwaitEntering = true;
		UGameplayStatics::PlaySoundAtLocation(this, OpenSound, FVector(1826, 1844, 186 + HeightDifference));
	}

	if (CloseUpperVolume->IsOverlappingActor(PlayerPawn) && !bDoorsClosed && !bDoorsShouldClose && !bAwaitLeaving)
	{
		bDoorsShouldClose = true;
		UGameplayStatics::PlaySoundAtLocation(this, CloseSound, FVector(1826, 1844, 186 + HeightDifference));
	}

	if (CloseUpperVolume->IsOverlappingActor(PlayerPawn) && bDoorsClosed && !bDoorsShouldClose && !bLiftShouldMove && !bAwaitLeaving)
	{
		bLiftShouldMove = true;
		bPlayerWasOutside = false;
		bPlayerIsInside = true;
		bLiftIsUpstairs = true;
		UGameplayStatics::PlaySound2D(this, MainSound);
	}

	if (bAwaitLeaving && !CloseLowerVolume->IsOverlappingActor(PlayerPawn) && !bDoorsClosed && !CloseUpperVolume->IsOverlappingActor(PlayerPawn))
	{
		bDoorsShouldClose = true;
		bAwaitLeaving = false;
		bPlayerIsInside = false;
		//bPlayerWasOutside = true;
		UGameplayStatics::PlaySoundAtLocation(this, CloseSound, FVector(1826, 1844, 186));
	}

	if (!bPlayerIsInside && !OpenLowerVolume->IsOverlappingActor(PlayerPawn) && !OpenUpperVolume->IsOverlappingActor(PlayerPawn))
	{
		bPlayerWasOutside = true;
	}

	if (!bPlayerIsInside && !bDoorsClosed && !OpenUpperVolume->IsOverlappingActor(PlayerPawn) && !bDoorsShouldClose && bLiftIsUpstairs)
	{
		bDoorsShouldClose = true;
		UGameplayStatics::PlaySoundAtLocation(this, CloseSound, FVector(1826, 1844, 186 + HeightDifference));
	}




	if (bLiftShouldMove)
	{
		UE_LOG(LogTemp, Warning, TEXT("Er sött move!"));
		if (bLiftIsUpstairs)
		{
			AddActorWorldOffset(FVector(0, 0, -1 * HeightPerSecond * GetWorld()->GetDeltaSeconds()), true);
			UE_LOG(LogTemp, Warning, TEXT("Er sött abegah!"));
			if (GetActorLocation().Z <= StartPosition.Z)
			{
				bDoorsShouldOpen = true;
				UGameplayStatics::PlaySoundAtLocation(this, OpenSound, FVector(1826, 1844, 186));
				bLiftShouldMove = false;
				bLiftIsUpstairs = false;
				bAwaitLeaving = true;
			}
		}
		else
		{
			FHitResult* HitResult = nullptr;
			AddActorWorldOffset(FVector(0, 0, HeightPerSecond * GetWorld()->GetDeltaSeconds()), true);
			if (GetActorLocation().Z > EndPosition.Z)
			{
				bDoorsShouldOpen = true;
				UGameplayStatics::PlaySoundAtLocation(this, OpenSound, FVector(1826, 1844, 186 + HeightDifference));
				bLiftShouldMove = false;
				bLiftIsUpstairs = true;
				bAwaitLeaving = true;
			}
		}
	}


	if (bDoorsShouldOpen)
	{
		InnerDoor->SetWorldLocation(InnerDoor->GetComponentLocation() + -1 * GetWorld()->GetDeltaSeconds() * DoorMovementPerSecond);
		OuterDoor->SetWorldLocation(OuterDoor->GetComponentLocation() + -1 * GetWorld()->GetDeltaSeconds() * DoorMovementPerSecond * 0.5);
		DoorDistanceMoved += -1 * GetWorld()->GetDeltaSeconds() * DoorMovementPerSecond;

		if (DoorDistanceMoved.Size() > DoorDirection.Size())
		{
			bDoorsClosed = false;
			bDoorsShouldOpen = false;
			DoorDistanceMoved = FVector(0);
		}
	}

	if (bDoorsShouldClose)
	{
		InnerDoor->SetWorldLocation(InnerDoor->GetComponentLocation() + GetWorld()->GetDeltaSeconds() * DoorMovementPerSecond);
		OuterDoor->SetWorldLocation(OuterDoor->GetComponentLocation() + GetWorld()->GetDeltaSeconds() * DoorMovementPerSecond * 0.5);
		DoorDistanceMoved += GetWorld()->GetDeltaSeconds() * DoorMovementPerSecond;

		if (DoorDistanceMoved.Size() > DoorDirection.Size())
		{
			bDoorsClosed = true;
			bDoorsShouldClose = false;
			DoorDistanceMoved = FVector(0);
		}
	}
}

void ALift::MoveLiftToPlayer()
{
	if (OpenLowerVolume->IsOverlappingActor(PlayerPawn) && bLiftIsUpstairs)
	{
		SetActorLocation(StartPosition);
		bLiftIsUpstairs = false;
	}
	if (OpenUpperVolume->IsOverlappingActor(PlayerPawn) && !bLiftIsUpstairs)
	{
		SetActorLocation(EndPosition);
		bLiftIsUpstairs = true;
	}
}

