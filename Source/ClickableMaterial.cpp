// (c) Dorian Zgraggen 2019, dorianzgraggen.com


#include "ClickableMaterial.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "HAL/FileManager.h"
#include "UObject/UObjectGlobals.h"
#include "Materials/Material.h"
#include "ClickableMaterialButton.h"

// Sets default values
AClickableMaterial::AClickableMaterial()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create static mesh component
	MaterialMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MaterialMesh"));
	MaterialMesh->SetupAttachment(GetRootComponent());

	MaterialMesh->OnComponentBeginOverlap.AddDynamic(this, &AClickableMaterial::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AClickableMaterial::BeginPlay()
{
	Super::BeginPlay();
	this->SetActorScale3D(FVector(MaterialMeshScale, MaterialMeshScale, MaterialMeshScale));
}

// Called every frame
void AClickableMaterial::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AClickableMaterial::ApplyMaterial(UMaterialInterface* NewMaterial)
{
	MaterialMesh->SetMaterial(0, NewMaterial);
	AppliedMaterial = NewMaterial;
	//UE_LOG(LogTemp, Warning, TEXT("ApplyMaterial meldet"));
}

void AClickableMaterial::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)  
{  
	UE_LOG(LogTemp, Warning, TEXT("==========================="));
    // Other Actor is the actor that triggered the event. Check that is not ourself.  
    if ( (OtherActor != nullptr ) && (OtherActor != this) && ( OtherComp != nullptr ) )  
    {  
		UE_LOG(LogTemp, Warning, TEXT("Other Actor is: %s"), *OtherComp->GetFName().ToString());
    }  
}

float AClickableMaterial::DistanceOfPointToMeshSurface(FVector PointLocation, FVector& ClosestSurfacePoint) const
{
	return MaterialMesh->GetDistanceToCollision(PointLocation, ClosestSurfacePoint);
}
