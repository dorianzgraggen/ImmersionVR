// (c) Dorian Zgraggen 2019, dorianzgraggen.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TriggerVolume.h"
#include "Lift.generated.h"

UCLASS()
class INTERACTIVE_FOYER_API ALift : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALift();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveLiftToPlayer();

	bool bPlayerIsInside = false;
	bool bMoving = false;
	bool bLiftIsUpstairs = false;
	bool bDoorsClosed = true;

	bool bDoorsShouldOpen = false;
	bool bDoorsShouldClose = false;

	bool bLiftShouldMove = false;

	bool bAwaitLeaving = false;
	bool bAwaitEntering = false;

	bool bPlayerWasOutside = true;
	
	UPROPERTY(EditAnywhere)
	FVector DoorDirection = FVector(0, -150, 0);
	UPROPERTY(EditAnywhere)
	FVector DoorMovementPerSecond = FVector(0, -75, 0);
	FVector DoorDistanceMoved = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere)
	float HeightDifference = 430;

	UPROPERTY(EditAnywhere)
	float HeightPerSecond = 40;

	FVector StartPosition;
	FVector EndPosition;


	UPROPERTY(VisibleAnywhere)
	AActor* PlayerPawn;

	// Components

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Main;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* InnerDoor;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* OuterDoor;
	
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* LowerSound;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* UpperSound;


	// Sounds

	UPROPERTY(EditAnywhere)
	USoundBase* OpenSound;

	UPROPERTY(EditAnywhere)
	USoundBase* CloseSound;

	UPROPERTY(EditAnywhere)
	USoundBase* MainSound;

	// Trigger Volumes

	UPROPERTY(EditAnywhere)
	ATriggerVolume* CloseLowerVolume;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* OpenLowerVolume;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* CloseUpperVolume;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* OpenUpperVolume;
};
