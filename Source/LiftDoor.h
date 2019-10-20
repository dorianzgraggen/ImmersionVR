// (c) Dorian Zgraggen 2019, dorianzgraggen.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "LiftDoor.generated.h"

UCLASS()
class INTERACTIVE_FOYER_API ALiftDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALiftDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
		ATriggerVolume* OpenTriggerVolume;

	UPROPERTY(VisibleAnywhere)
		AActor* PlayerPawn;

	UPROPERTY(EditAnywhere)
		USoundBase* OpenSound;

	UPROPERTY(EditAnywhere)
		USoundBase* CloseSound;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* MainMesh;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* InnerDoor;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* OuterDoor;

	UPROPERTY(VisibleAnywhere)
	class ALift* Lift;
};
