// (c) Dorian Zgraggen 2019, dorianzgraggen.com


#include "LiftDoor.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Engine/TriggerVolume.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "PlayerCharacter.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Lift.h"
#include "EngineUtils.h" 

// Sets default values
ALiftDoor::ALiftDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	SetRootComponent(MainMesh);

	InnerDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InnerDoor"));
	InnerDoor->SetupAttachment(MainMesh);

	OuterDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OuterDoor"));
	OuterDoor->SetupAttachment(MainMesh);
}

// Called when the game starts or when spawned
void ALiftDoor::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	for (TActorIterator<ALift> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		Lift = *ActorItr;
	}

	InnerDoor->AddWorldOffset(Lift->DoorDirection);
	OuterDoor->AddWorldOffset(Lift->DoorDirection * 0.5);
}

// Called every frame
void ALiftDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	if (Lift->bDoorsShouldOpen)
	{
		InnerDoor->SetWorldLocation(InnerDoor->GetComponentLocation() + -1 * GetWorld()->GetDeltaSeconds() * Lift->DoorMovementPerSecond);
		OuterDoor->SetWorldLocation(OuterDoor->GetComponentLocation() + -1 * GetWorld()->GetDeltaSeconds() * Lift->DoorMovementPerSecond * 0.5);
	}
	else if (Lift->bDoorsShouldClose)
	{
		InnerDoor->SetWorldLocation(InnerDoor->GetComponentLocation() + GetWorld()->GetDeltaSeconds() * Lift->DoorMovementPerSecond);
		OuterDoor->SetWorldLocation(OuterDoor->GetComponentLocation() + GetWorld()->GetDeltaSeconds() * Lift->DoorMovementPerSecond * 0.5);
	}
}

