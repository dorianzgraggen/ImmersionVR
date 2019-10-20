// (c) Dorian Zgraggen 2019, dorianzgraggen.com


#include "ApplyButton.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
AApplyButton::AApplyButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ApplyButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ApplyButtonMesh"));
}

// Called when the game starts or when spawned
void AApplyButton::BeginPlay()
{
	Super::BeginPlay();
	
	auto NMaterial = ApplyButtonMesh->GetMaterial(0);
	DynamicMaterial = UMaterialInstanceDynamic::Create(NMaterial, NULL);
	ApplyButtonMesh->SetMaterial(0, DynamicMaterial);
}

// Called every frame
void AApplyButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*float num = 0.5f + FMath::Cos(GetWorld()->TimeSeconds) / 2;
	DynamicMaterial->SetScalarParameterValue(FName("GreenAreaSize"), num);*/
}

void AApplyButton::UpdateGreenArea(float Size)
{
	DynamicMaterial->SetScalarParameterValue(FName("GreenAreaSize"), Size);
}
