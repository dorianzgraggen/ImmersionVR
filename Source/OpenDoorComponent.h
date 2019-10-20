// (c) Dorian Zgraggen 2019, dorianzgraggen.com

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OpenDoorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INTERACTIVE_FOYER_API UOpenDoorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void RotateDoor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator RotationPerSecond;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator GoalRotation;

private:
	bool bShouldItRotate = false;
};
