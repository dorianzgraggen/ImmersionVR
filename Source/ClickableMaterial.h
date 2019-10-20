// (c) Dorian Zgraggen 2019, dorianzgraggen.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerCharacter.h"
#include "ClickableMaterial.generated.h"

UCLASS()
class INTERACTIVE_FOYER_API AClickableMaterial : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClickableMaterial();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// return MaterialMesh
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return MaterialMesh; }

	void ApplyMaterial(UMaterialInterface* NewMaterial);

	float DistanceOfPointToMeshSurface(FVector PointLocation, FVector& ClosestSurfacePoint) const;

	UPROPERTY(EditAnywhere)
	float MaterialMeshScale = 0.2;

	UPROPERTY(VisibleAnywhere)
	class UMaterialInterface* AppliedMaterial;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	
	// Static mesh used to preview available materials
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MaterialMesh;

	
};
