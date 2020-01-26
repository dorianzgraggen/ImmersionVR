// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "HeadMountedDisplay.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"
#include "Classes/Camera/PlayerCameraManager.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "NavigationSystem.h"
#include "IModularFeature.h"
#include "IMotionController.h"
#include "MotionControllerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "ClickableMaterial.h"
#include "Engine/World.h"
#include "Templates/SubclassOf.h"
#include "Components/MeshComponent.h"
#include "Containers/Array.h"
#include "Components/SplineComponent.h"
#include "UObject/UObjectGlobals.h"
#include "Components/SplineMeshComponent.h"
#include "Public/DrawDebugHelpers.h"
#include "MaterialListComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ApplyButton.h"
#include "Math/Vector.h"
#include "Math/Box.h"
#include "Math/Rotator.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "EngineUtils.h"
#include "OpenDoorComponent.h"
#include "UObject/NameTypes.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundBase.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SphereComponent.h"
#include "Lift.h"
#include "Components/PointLightComponent.h" 

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraHolder = CreateDefaultSubobject<USceneComponent>(TEXT("CamHolder"));
	CameraHolder->SetupAttachment(GetRootComponent());
	Camera->SetupAttachment(CameraHolder);
	CameraHolder->RelativeLocation.Set(0.f, 0.f, -90.15f);

	TeleportationMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportationMarker"));
	TeleportationMarker->SetupAttachment(GetRootComponent());

	Highlighter = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Highlighter"));
	Highlighter->SetupAttachment(GetRootComponent());

	LeftHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHand"));
	LeftHand->SetupAttachment(CameraHolder);
	LeftHand->SetTrackingSource(EControllerHand::Left);

	RightHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHand"));
	RightHand->SetupAttachment(CameraHolder);
	RightHand->SetTrackingSource(EControllerHand::Right);

	//RightHand->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnHandOverlapBegin);

	ArcPath = CreateDefaultSubobject<USplineComponent>(TEXT("ArcPath"));
	ArcPath->SetupAttachment(RightHand);

	/*LeftFist = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftFist"));
	LeftFist->SetupAttachment(LeftHand);

	LeftFinger = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftFinger"));
	LeftFinger->SetupAttachment(LeftHand);*/

	RightRestFaceButton = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightRestFaceButton"));
	RightRestFaceButton->SetupAttachment(RightHand);

	RightPointFaceButton = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightPointFaceButton"));
	RightPointFaceButton->SetupAttachment(RightHand);

	RightRestThumbstick = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightRestThumbstick"));
	RightRestThumbstick->SetupAttachment(RightHand);

	RightPointThumbstick = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightPointThumbstick"));
	RightPointThumbstick->SetupAttachment(RightHand);

	LeftRestFaceButton = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftRestFaceButton"));
	LeftRestFaceButton->SetupAttachment(LeftHand);

	LeftPointFaceButton = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftPointFaceButton"));
	LeftPointFaceButton->SetupAttachment(LeftHand);

	LeftRestThumbstick = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftRestThumbstick"));
	LeftRestThumbstick->SetupAttachment(LeftHand);

	LeftPointThumbstick = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftPointThumbstick"));
	LeftPointThumbstick->SetupAttachment(LeftHand);

	ChangeMaterialText = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChangeMaterialText"));
	ChangeMaterialText->SetupAttachment(RightHand);

	TeleportText = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportText"));
	TeleportText->SetupAttachment(LeftHand);

	ChangeMaterialTextLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChangeMaterialTextLeft"));
	ChangeMaterialTextLeft->SetupAttachment(LeftHand);

	TeleportTextLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportTextLeft"));
	TeleportTextLeft->SetupAttachment(RightHand);

	ControllerLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ControllerLeft"));
	ControllerLeft->SetupAttachment(LeftHand);

	ControllerRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ControllerRight"));
	ControllerRight->SetupAttachment(RightHand);

	UIHelperBase = CreateDefaultSubobject<USceneComponent>(TEXT("UIHelperBase"));
	UIHelperBase->SetupAttachment(CameraHolder);

	SpectatorHelper = CreateDefaultSubobject<USceneComponent>(TEXT("SpectatorHelper"));
	SpectatorHelper->SetupAttachment(CameraHolder);

	SpectatorCamHolder = CreateDefaultSubobject<USceneComponent>(TEXT("SpectatorCamHolder"));
	SpectatorCamHolder->SetupAttachment(SpectatorHelper);

	SpectatorCam = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SpectatorCam"));
	SpectatorCam->SetupAttachment(SpectatorCamHolder);


	PlayerHead = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerHead"));
	PlayerHead->SetupAttachment(Camera);

	UIHelper = CreateDefaultSubobject<USceneComponent>(TEXT("UIHelper"));
	UIHelper->SetupAttachment(UIHelperBase);

	HandDirection = CreateDefaultSubobject<USceneComponent>(TEXT("HandDirection"));
	HandDirection->SetupAttachment(RightPointFaceButton);

	PointingSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PointingSphere"));
	PointingSphere->SetupAttachment(GetRootComponent());
	PointingSphere->SetVisibility(false);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	ChangeMaterialText->SetVisibility(false);
	TeleportText->SetVisibility(false);
	ChangeMaterialTextLeft->SetVisibility(false);
	TeleportTextLeft->SetVisibility(false);
	TeleportationMarker->SetVisibility(false);
	PointingSphereScale = PointingSphereMaxSize / 5;
	PC = Cast<APlayerController>(GetController());
	PC->PlayerCameraManager->StartCameraFade(0, 1, .1f, FLinearColor::Black, false, true);
	StrongHand = RightHand;
	WeakHand = LeftHand;
	ActualTeleportText = TeleportText;
	ActualChangeMaterialText = ChangeMaterialText;
	Highlighter->UnregisterComponent();
	HighlighterScale = Highlighter->GetComponentScale();
	Highlighter->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	ControllerDefaultMaterial = ControllerRight->GetMaterial(2);

	SpectatorCam->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	SpectatorCamHolder->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	SpectatorHelper->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	UE_LOG(LogTemp, Warning, TEXT("Moving"));


	for (TActorIterator<ALift> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		Lift = *ActorItr;
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//UE_LOG(LogTemp, Warning, TEXT("bIsRightHanded: %s"),(bIsRightHanded ? TEXT("True") : TEXT("False")))
	

	UE_LOG(LogTemp, Warning, TEXT("Delta Seconds: %f, FPS: %f"), GetWorld()->GetDeltaSeconds(), 1/GetWorld()->GetDeltaSeconds());


	


	if (Lift->bPlayerIsInside)
	{
		bLiftIsDominant = true;
		if (Lift->bAwaitLeaving)
		{
			SpectatorCamHolder->SetWorldLocation(SpectatorCam->GetComponentLocation());
			SpectatorCamHolder->SetWorldRotation(SpectatorCam->GetComponentRotation());
			bLiftIsDominant = false;
		}
	}
	else {
		bLiftIsDominant = false;
	}

	if (bPlayerHasRecentlyTeleported && !bLiftIsDominant)
	{
		SpectatorHelper->SetWorldLocation(Camera->GetComponentLocation());
		SpectatorHelper->AddWorldOffset(FVector(0, 0, 35));

		bool bRotationInvalid = true;
		float CurrentRotOffset = 0;
		float FinalRotOffset = 0;

		OldSpecCamLocation = SpectatorCamHolder->GetComponentLocation();
		OldSpecCamRotation = SpectatorCamHolder->GetComponentRotation();
		OldSpecCamRotation.Pitch = -18;
		OldSpecCamRotation.Roll = 0;

		do
		{
			SpectatorHelper->SetWorldRotation(FRotator(0, Camera->GetComponentRotation().Yaw + CurrentRotOffset, 0));
			SpectatorCamHolder->SetWorldRotation(SpectatorHelper->GetComponentRotation());
			SpectatorCamHolder->SetWorldLocation(SpectatorHelper->GetComponentLocation() + SpectatorHelper->GetForwardVector() * -120);
			bRotationInvalid = false;

		} while (bRotationInvalid);
		
		TravelVector = SpectatorCamHolder->GetComponentLocation() - OldSpecCamLocation;
		TravelRotator = SpectatorCamHolder->GetComponentRotation() - OldSpecCamRotation;
		TravelRotator.Pitch = -18;
		TravelRotator.Roll = 0;

		ProgressOnTravel = 0;
		bSpectatorCamMustMove = true;
	
		//SpectatorCam->SetWorldLocationAndRotation(SpectatorCamHolder->GetComponentLocation(), SpectatorCam->GetComponentRotation());

		bPlayerHasRecentlyTeleported = false;
	}

	if (bSpectatorCamMustMove && !bLiftIsDominant)
	{
		ProgressOnTravel += GetWorld()->GetDeltaSeconds();

		//FVector AddDistance = TravelVector * (-0.5 * UKismetMathLibrary::Cos(x) + 0.5);

		float Multiplier = SmoothlyCalculatePositionAlongWay(ProgressOnTravel);
		SpectatorCam->SetWorldLocation(OldSpecCamLocation + Multiplier * TravelVector);

		TravelRotator = SpectatorCamHolder->GetComponentRotation() - OldSpecCamRotation;
		TravelRotator.Pitch = 0;
		TravelRotator.Roll = 0;
		SpectatorCam->SetWorldRotation(OldSpecCamRotation + Multiplier * TravelRotator);
		
		//SpectatorCam->AddWorldOffset(TravelVector * (-0.5 * UKismetMathLibrary::Cos(UKismetMathLibrary::GetPI() * ProgressOnTravel) - 1));

		//SpectatorCam->AddWorldOffset(TravelVector * GetWorld()->GetDeltaSeconds());

		if (ProgressOnTravel > 1)
		{
			bSpectatorCamMustMove = false;
		}
	}
	else if (bSpectatorCamMustAdjustRotation && !bLiftIsDominant)
	{
		ProgressOnTravel += GetWorld()->GetDeltaSeconds();
		float Multiplier = SmoothlyCalculatePositionAlongWay(ProgressOnTravel);

		SpectatorCam->SetWorldLocation(OldSpecCamLocation + Multiplier * TravelVector);

		TravelRotator = SpectatorCamHolder->GetComponentRotation() - OldSpecCamRotation;
		TravelRotator.Pitch = 0;
		TravelRotator.Roll = 0;
		SpectatorCam->SetWorldRotation(OldSpecCamRotation + Multiplier * TravelRotator);

		if (ProgressOnTravel > 1)
		{
			bSpectatorCamMustAdjustRotation = false;
		}

	}
	else if (!bPlayerHasRecentlyTeleported && !bLiftIsDominant)
	{
		if (SpectatorCam->GetComponentRotation().Yaw - Camera->GetComponentRotation().Yaw < -45 || SpectatorCam->GetComponentRotation().Yaw - Camera->GetComponentRotation().Yaw > 45)
		{
			OldSpecCamLocation = SpectatorCamHolder->GetComponentLocation();
			OldSpecCamRotation = SpectatorCamHolder->GetComponentRotation();
			OldSpecCamRotation.Pitch = -18;
			OldSpecCamRotation.Roll = 0;
			bool bRotationInvalid = true;

			do
			{
				SpectatorHelper->SetWorldRotation(FRotator(0, Camera->GetComponentRotation().Yaw, 0));
				SpectatorCamHolder->SetWorldRotation(SpectatorHelper->GetComponentRotation());
				SpectatorCamHolder->SetWorldLocation(SpectatorHelper->GetComponentLocation() + SpectatorHelper->GetForwardVector() * -120);
				bRotationInvalid = false;

			} while (bRotationInvalid);

			TravelVector = SpectatorCamHolder->GetComponentLocation() - OldSpecCamLocation;
			TravelRotator = SpectatorCamHolder->GetComponentRotation() - OldSpecCamRotation;
			TravelRotator.Pitch = -18;
			TravelRotator.Roll = 0;

			ProgressOnTravel = 0;
			bSpectatorCamMustAdjustRotation = true;
		}
	}

	if (Lift->bPlayerIsInside)
	{
		SpectatorCam->SetWorldLocation(GetLiftSpecCamLocation());
		SpectatorCam->SetWorldRotation(GetLiftSpecCamRotation());
		UE_LOG(LogTemp, Warning, TEXT("Spec_Location is %f %f %f"), GetLiftSpecCamLocation().X, GetLiftSpecCamLocation().Y, GetLiftSpecCamLocation().Z);

		//UE_LOG(LogTemp, Warning, TEXT("HandRaisedCurrentTime: %f"), RightHandRaisedCurrentSeconds);

		//UE_LOG(LogTemp, Warning, TEXT("Spec_Rotation is %s"), GetLiftSpecCamRotation().ToString());

	}
	
	
	

	// End Spectator



	HandleFingerPlacement();

	if (bDrawArc) UpdateTeleportationElements();

	CorrectCollision();

	if (bRightPointing && bIsRightHanded && bCurrentlyChangingMaterials == false && bCurrentlyTeleporting == false && bAllowPointingAtObject && StrongHand != nullptr)
	{
		FindObjectByPointing();
	}
	else if (bLeftPointing && bIsRightHanded == false && bCurrentlyChangingMaterials == false && bCurrentlyTeleporting == false && bAllowPointingAtObject && StrongHand != nullptr)
	{
		FindObjectByPointing();
	}
	else
	{
		PointingSphere->SetVisibility(false);
		ActualChangeMaterialText->SetVisibility(false);
	}

	if (bCurrentlyChangingMaterials) UpdateApplyButton();

	if (bCurrentlyChangingMaterials) HandleClickingMaterialSpheres();

	// Intro
	if (bWaitingForHand)
	{
		UE_LOG(LogTemp, Warning, TEXT("WaitingForHand"));
		FVector RightHandPos = RightHand->GetComponentLocation();
		FVector LeftHandPos = LeftHand->GetComponentLocation();

		FVector EyesPos = Camera->GetComponentLocation();

		float RightHeightDifference = EyesPos.Z - RightHandPos.Z;
		float LeftHeightDifference = EyesPos.Z - LeftHandPos.Z;
		if (RightHandPos.Z > LeftHandPos.Z)
		{
			LeftHandRaisedCurrentSeconds = 0;
			if (RightHeightDifference < 10)
			{
				RightHandRaisedCurrentSeconds += GetWorld()->GetDeltaSeconds();

				if (!bIsRightHandRaised)
				{
					SetHandMaterial("Right", HandRaisedMaterial);
				}

				bIsRightHandRaised = true;
			}
			else
			{
				RightHandRaisedCurrentSeconds = 0;
				if (!bIsRightHandRaised)
				{
					SetHandMaterial("Right", HandMaterial);
				} 
				bIsRightHandRaised = false;
			}

			if (RightHandRaisedCurrentSeconds >= HandRaisedAcceptedSeconds)
			{
				SetHandMaterial("Right", HandMaterial);
				bIsRightHandRaised = false;
				bWaitingForHand = false;
				bIsRightHanded = true;
				StrongHand = RightHand;
				WeakHand = LeftHand;
				UGameplayStatics::PlaySoundAtLocation(this, SuccessSound, RightHand->GetComponentLocation());

				ActualTeleportText = TeleportText;
				ActualChangeMaterialText = ChangeMaterialText;
				ExplainMatChange();
				UE_LOG(LogTemp, Warning, TEXT("Player is righthanded"));
			}
		}
		else
		{
			RightHandRaisedCurrentSeconds = 0;
			if (LeftHeightDifference < 10)
			{
				LeftHandRaisedCurrentSeconds += GetWorld()->GetDeltaSeconds();

				if (!bIsLeftHandRaised)
				{
					SetHandMaterial("Left", HandRaisedMaterial);
				}
				bIsRightHandRaised = true;
			}
			else
			{
				RightHandRaisedCurrentSeconds = 0;
				LeftHandRaisedCurrentSeconds = 0;
				if (!bIsRightHandRaised) SetHandMaterial("Left", HandMaterial);
				bIsRightHandRaised = false;
			}

			if (LeftHandRaisedCurrentSeconds >= HandRaisedAcceptedSeconds)
			{
				SetHandMaterial("Left", HandMaterial);
				bIsLeftHandRaised = false;
				bWaitingForHand = false;

				bIsRightHanded = false;
				StrongHand = LeftHand;
				WeakHand = RightHand;
				UGameplayStatics::PlaySoundAtLocation(this, SuccessSound, LeftHand->GetComponentLocation());
				ActualTeleportText = TeleportTextLeft;
				ActualChangeMaterialText = ChangeMaterialTextLeft;
				UE_LOG(LogTemp, Warning, TEXT("Player is lefthanded"));
				ExplainMatChange();
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("HandRaisedCurrentTime: %f"), RightHandRaisedCurrentSeconds);
	}

	if (bAwaitClickingOnSphere && bClickedOnMat)
	{
		PlayIntroSound("ApplyButton");
		bAwaitClickingOnSphere = false;
		bAwaitApplying = true;
		FTimerHandle TeleportTaskTimerHandle;
		GetWorldTimerManager().SetTimer(TeleportTaskTimerHandle, this, &APlayerCharacter::AllowPlayTeleportTask, 4.f);

	}

	if (bAllowPlayTeleportTask && bAppliedMat)
	{
		PlayIntroSound("TeleportTask");
		bSuccessfullyChangedMaterials = true;
		bAllowPlayTeleportTask = false;
		bTeleportationAllowed = true;
		if (StrongHand == RightHand)
		{
			ControllerLeft->SetMaterial(1, ControllerGlowingMaterial);
		}
		else
		{
			ControllerRight->SetMaterial(1, ControllerGlowingMaterial);
		}
	}

	if (bTeleportTried && bAllowPlayTeleportDestination)
	{
		PlayIntroSound("TeleportDestination");
		bAllowPlayTeleportDestination = false;
		ActualTeleportText->SetVisibility(true);
	}

	if (bTeleportFinished && bAllowPlayEnjoy)
	{
		PlayIntroSound("Enjoy");
		if (StrongHand == RightHand)
		{
			ControllerLeft->SetMaterial(1, ControllerDefaultMaterial);
		}
		else
		{
			ControllerRight->SetMaterial(1, ControllerDefaultMaterial);
		}
		OpenEntryDoors();

		bAllowPlayEnjoy = false;
	}
}

void APlayerCharacter::OpenEntryDoors()
{
	TArray<AActor*> FoundActors;
	FName Tag = TEXT("actTag");
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, FoundActors);
	if (FoundActors.Num() > 0)
	{
		for (int32 i = 0; i < FoundActors.Num(); i++)
		{
			AActor* FirstActor = FoundActors[i];
			UOpenDoorComponent* FirstComponent = FirstActor->FindComponentByClass<UOpenDoorComponent>();
			FirstComponent->RotateDoor();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No actors found"));
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);

	// Defines which axis mapping belongs to which function
	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Right"), this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAction(TEXT("Teleport"), IE_Released, this, &APlayerCharacter::Teleport);
	PlayerInputComponent->BindAction(TEXT("StartDrawingArc"), IE_Pressed, this, &APlayerCharacter::ShowArc);


	/*PlayerInputComponent->BindAction(TEXT("MakeFistLeft"), IE_Pressed, this, &APlayerCharacter::MakeFistLeft);
	PlayerInputComponent->BindAction(TEXT("PointLeft"), IE_Pressed, this, &APlayerCharacter::PointLeft);*/
	PlayerInputComponent->BindAction(TEXT("MakeFistRight"), IE_Pressed, this, &APlayerCharacter::MakeFistRight);
	
	PlayerInputComponent->BindAction(TEXT("RightPointing"), IE_Pressed, this, &APlayerCharacter::ActivateRightPointing);
	PlayerInputComponent->BindAction(TEXT("RightPointing"), IE_Released, this, &APlayerCharacter::DeactivateRightPointing);

	PlayerInputComponent->BindAction(TEXT("RightFaceButton"), IE_Pressed, this, &APlayerCharacter::ActivateRightFaceButton);
	PlayerInputComponent->BindAction(TEXT("RightFaceButton"), IE_Released, this, &APlayerCharacter::DeactivateRightFaceButton);

	PlayerInputComponent->BindAction(TEXT("LeftPointing"), IE_Pressed, this, &APlayerCharacter::ActivateLeftPointing);
	PlayerInputComponent->BindAction(TEXT("LeftPointing"), IE_Released, this, &APlayerCharacter::DeactivateLeftPointing);

	PlayerInputComponent->BindAction(TEXT("LeftFaceButton"), IE_Pressed, this, &APlayerCharacter::ActivateLeftFaceButton);
	PlayerInputComponent->BindAction(TEXT("RightFaceButton"), IE_Released, this, &APlayerCharacter::DeactivateLeftFaceButton);


	PlayerInputComponent->BindAction(TEXT("ShowUserInterface"), IE_Released, this, &APlayerCharacter::FindObjectByPointing);
	PlayerInputComponent->BindAction(TEXT("SpawnClickableMaterial"), IE_Pressed, this, &APlayerCharacter::SpawnClickableMaterial);
	
	PlayerInputComponent->BindAction(TEXT("GripRight"), IE_Pressed, this, &APlayerCharacter::ActivateRightGrip);
	PlayerInputComponent->BindAction(TEXT("GripRight"), IE_Released, this, &APlayerCharacter::DeactivateRightGrip);

	PlayerInputComponent->BindAction(TEXT("RightChangeMaterial"), IE_Pressed, this, &APlayerCharacter::WantToChangeMaterialRight);
	PlayerInputComponent->BindAction(TEXT("RightChangeMaterial"), IE_Released, this, &APlayerCharacter::DontWantToChangeMaterialRight);

	PlayerInputComponent->BindAction(TEXT("LeftChangeMaterial"), IE_Pressed, this, &APlayerCharacter::WantToChangeMaterialLeft);
	PlayerInputComponent->BindAction(TEXT("LeftChangeMaterial"), IE_Released, this, &APlayerCharacter::DontWantToChangeMaterialLeft);


	PlayerInputComponent->BindAction(TEXT("ChangeLanguage"), IE_Pressed, this, &APlayerCharacter::ChangeLanguage);

	// Intro
	PlayerInputComponent->BindAction(TEXT("StartExperience"), IE_Pressed, this, &APlayerCharacter::StartExperience);
	PlayerInputComponent->BindAction(TEXT("SkipIntro"), IE_Pressed, this, &APlayerCharacter::SkipIntro);
}

/* WASD MOVEMENT FOR TESTING ONLY */
void APlayerCharacter::MoveForward(float Speed)
{
	AddMovementInput(Camera->GetForwardVector(), Speed);
}

void APlayerCharacter::MoveRight(float Speed)
{
	AddMovementInput(Camera->GetRightVector(), Speed);
}

/* =================================== */
/* TELEPORTATION */

void APlayerCharacter::UpdateTeleportationElements()
{
	FVector Location;
	TArray<FVector> Path;
	bHasDestination = isDestinationValid(Path, Location);
	Location += FVector(0, 0, TeleportationMarkerOffset);


	if (bHasDestination)
	{
		TeleportationMarker->SetVisibility(true);
		TeleportationMarker->SetWorldLocation(Location);
		DrawArc(Path);
		if (bTeleportationAllowed) bArcHasBeenDrawn = true;
	}
	else
	{
		TeleportationMarker->SetVisibility(false);
		TArray<FVector> EmptyPath;
		DrawArc(EmptyPath);
	}
}

bool APlayerCharacter::isDestinationValid(TArray<FVector> &OutPath, FVector &OutLocation)
{
	FHitResult HitResult;
	FVector Start = WeakHand->GetComponentLocation();
	
	FVector InLaunchVelocity = WeakHand->GetForwardVector() * ProjectileSpeed;

	FPredictProjectilePathParams Params(InProjectileRadius, Start, InLaunchVelocity, ProjectileSimTime, ECollisionChannel::ECC_Visibility, this);

	// Params.bTraceComplex = true;
	FPredictProjectilePathResult Result;
	bool bHit = UGameplayStatics::PredictProjectilePath(this, Params, Result);

	// Check if player is pointing to point in reach
	if (!bHit) return false;

	for (FPredictProjectilePathPointData PointData : Result.PathData)
	{
		// Add each point of the simulation to array
		OutPath.Add(PointData.Location);
		// UE_LOG(LogTemp, Warning, TEXT("Neuer Punkt in OutPath: %s"), *PointData.Location.ToString());
	}

	// Check if the point is near NavMesh
	FNavLocation NavLocation;
	const UNavigationSystemV1* NavigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this); // Use deprecated NavSystem
	bool bOnNavMesh = NavigationSystem->ProjectPointToNavigation(Result.HitResult.Location, NavLocation, TeleportProjectionExtent);

	if (!bOnNavMesh) return false;

	OutLocation = NavLocation.Location;
	return true;
}

// Draw the Arc using SplineMesh segments
void APlayerCharacter::DrawArc(const TArray<FVector> &Path)
{
	
	UpdateArc(Path);
	bTeleportTried = true;
	// UE_LOG(LogTemp, Warning, TEXT("Anzahl in MeshPool is %s"), *FString::FromInt(Path.Num()));

	// Hide all SplineMeshes
	for (USplineMeshComponent* SplineMesh : ArcMeshPool)
	{
		SplineMesh->SetVisibility(false);
	}

	// Only show SplineMeshes between the player's hand and the TeleportationMarker
	for (int32 i = 0; i < Path.Num() - 1; i++)
	{
		USplineMeshComponent* SplineMesh = nullptr;

		// Create new SplineMesh and add it to the ArcMeshPool if there isn't already one
		if (ArcMeshPool.Num() <= i)
		{
			SplineMesh = NewObject<USplineMeshComponent>(this);
			SplineMesh->SetMobility(EComponentMobility::Movable);
			SplineMesh->AttachToComponent(ArcPath, FAttachmentTransformRules::KeepRelativeTransform);
			SplineMesh->SetStaticMesh(ArcMesh);
			SplineMesh->SetMaterial(0, ArcMaterial);
			SplineMesh->RegisterComponent();
			ArcMeshPool.Add(SplineMesh);
		}
		// Use SplineMesh from ArcMeshPool
		SplineMesh = ArcMeshPool[i];
		SplineMesh->SetVisibility(true);

		// Make SplineMesh follow the Path from PredictProjectilePath
		FVector StartPos, StartTangent, EndPos, EndTangent;
		ArcPath->GetLocalLocationAndTangentAtSplinePoint(i, StartPos, StartTangent);
		ArcPath->GetLocalLocationAndTangentAtSplinePoint(i + 1, EndPos, EndTangent);
		SplineMesh->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
	}
}

void APlayerCharacter::UpdateArc(const TArray<FVector> &Path)
{
	ArcPath->ClearSplinePoints(false);

	// Add every point of the PredictProjectilePath to the Array
	for (FVector Current : Path) {
		ArcPath->AddSplinePoint(Current, ESplineCoordinateSpace::World, false);
	}
	ArcPath->UpdateSpline();
}

void APlayerCharacter::ShowArc()
{
	if (bCurrentlyChangingMaterials)
	{
		return;
	}
	if (!bTeleportationAllowed)
	{
		return;
	}
	if (!bSuccessfullyChangedMaterials)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Arc should appear"));
	bCurrentlyTeleporting = true;
	bDrawArc = true;
}

void APlayerCharacter::HideArc()
{
	bDrawArc = false;
	bArcHasBeenDrawn = false;
	UE_LOG(LogTemp, Warning, TEXT("x-Arc should disappear"));
	for (USplineMeshComponent* SplineMesh : ArcMeshPool)
	{
		SplineMesh->SetVisibility(false);
	}
}

void APlayerCharacter::Teleport()
{
	if (bCurrentlyChangingMaterials || !bAppliedMat)
	{
		return;
	}
	/*if (!bArcHasBeenDrawn)
	{
		return;
	}*/
	FVector HeightOffset = FVector(0, 0, 90.15f);
	bCurrentlyTeleporting = true;

	if (!StillTeleporting)
	{
		if (!bTeleportationAllowed || !bArcHasBeenDrawn)
		{
			return;
		}
		TeleportDestination = TeleportationMarker->GetComponentLocation() + HeightOffset;
		PC->PlayerCameraManager->StartCameraFade(0, 1, 1.f, FLinearColor::Black, false, true);
		FTimerHandle Handle;
		StillTeleporting = true;
		GetWorldTimerManager().SetTimer(Handle, this, &APlayerCharacter::Teleport, 1.f);
		//HideArc();
		DisallowTeleportation(.99f);
	}
	else
	{
		SetActorLocation(TeleportDestination);
		bPlayerHasRecentlyTeleported = true;
		bCurrentlyTeleporting = false;

		PC->PlayerCameraManager->StartCameraFade(1, 0, .5f, FLinearColor::Black);
		bTeleportFinished = true;
		StillTeleporting = false;
		HideArc();
		TeleportationMarker->SetVisibility(false);
	}
}

void APlayerCharacter::DisallowTeleportation(float Seconds)
{
	bTeleportationAllowed = false;
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, this, &APlayerCharacter::AllowTeleportation, Seconds);
}

void APlayerCharacter::AllowTeleportation()
{
	bTeleportationAllowed = true;
}

/* =================================== */
/* CHANGING MATERIALS */


void APlayerCharacter::FindObjectByPointing()
{
	if (bCurrentlyChangingMaterials)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("This far: 1"));


	FVector LineDirection;
	FVector Startpoint = StrongHand->GetComponentLocation();

	if (bIsRightHanded)
	{
		LineDirection = StrongHand->GetForwardVector().RotateAngleAxis(HandOffsetRotation, StrongHand->GetRightVector());
		LineDirection = LineDirection.RotateAngleAxis(HandZRotation, StrongHand->GetUpVector());
		Startpoint += StrongHand->GetRightVector() * LineLocationOffset.Y;
		Startpoint += StrongHand->GetUpVector() * LineLocationOffset.Z;

	}
	else
	{
		LineDirection = StrongHand->GetForwardVector().RotateAngleAxis(HandOffsetRotation, StrongHand->GetRightVector());
		LineDirection = LineDirection.RotateAngleAxis(-HandZRotation, StrongHand->GetUpVector());
		Startpoint += StrongHand->GetRightVector() * LineLocationOffset.Y *-1;
		Startpoint += StrongHand->GetUpVector() * LineLocationOffset.Z;

	}
	
	

	//DrawDebugSphere(GetWorld(), Startpoint, 3, 32, FColor(255, 0, 255));

	//DrawDebugSphere(GetWorld(), Startpoint, 3, 32, FColor(255, 255, 0));

	FHitResult HitResult;
	FVector Start = Startpoint + StrongHand->GetForwardVector() * 10;

	//DrawDebugSphere(GetWorld(), Start, 3, 32, FColor(0, 255, 255));
	FVector End = Start + LineDirection * MaxTeleportDistance;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredComponent(PointingSphere);
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);


	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("This far: 2"));

		PointingSphere->SetVisibility(true);
		// Visualise the LineTrace for debugging
		PointingSphere->SetWorldLocation(HitResult.ImpactPoint);
		// Get the UMaterialListComponent of the Object that has been hit
		AActor* ActorHit = HitResult.GetActor();
		SMComponent = ActorHit->FindComponentByClass<UStaticMeshComponent>();
		UMaterialListComponent* HitComponent = nullptr;
		HitComponent = ActorHit->FindComponentByClass<UMaterialListComponent>();

		// Check if it even has a UMaterialListComponent
		if (HitComponent == NULL || HitComponent == nullptr)
		{
			PointingSphere->SetMaterial(0, ObjectInvalidMaterial);
			ImpactPointExtent = 4;
			PointingSphereScale = PointingSphereMaxSize / 5;
			PointingSphere->SetWorldScale3D(FVector(PointingSphereScale));
			PointingSphere->SetVisibility(true);
			//DrawDebugBox(GetWorld(), HitResult.ImpactPoint, FVector(ImpactPointExtent), FColor::Red, false, -1.f, ECC_WorldStatic, 1.f);
			bGlissandoAllowed = false;
			DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Red, false, -1.f, ECC_WorldStatic, 1.f);
			bObjectPointingAtValid = false;
			ActualChangeMaterialText->SetVisibility(false);

		}
		else
		{

			if (HitComponent->bIsChangeableIntroMaterial && OldHitComponent != HitComponent && bAwaitPointing) 
			{
				ExplainPressing();
				bAwaitPointing = false;
				bAwaitHoldingMatChangeButton = true;
			}
			// Draw Green line and give the sphere at the ImpactPoint a green material
			DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Green, false, -1.f, ECC_WorldStatic, 1.f);
			bObjectPointingAtValid = true;


			PointingSphere->SetMaterial(0, ObjectValidMaterial);
			ActualChangeMaterialText->SetVisibility(true);

			PointingSphere->SetVisibility(true);
			if (OldHitComponent == HitComponent && bCurrentlyWantingToChangeMaterial)
			{
				bGlissandoAllowed = true;
				if (PointingSphere->GetComponentScale().X <= PointingSphereMaxSize / 5)
				{
					PlayGlissSound();
				}

				ImpactPointExtent += 0.5;
				PointingSphereScale += PointingSphereGrowPerSecond * GetWorld()->GetDeltaSeconds();
				PointingSphere->SetWorldScale3D(FVector(PointingSphereScale));
				UE_LOG(LogTemp, Warning, TEXT("This far: 3"));

				//SetPointingSphereInnerRadius(ImpactPointExtent/100);
			}
			else if (bCurrentlyWantingToChangeMaterial)
			{
				bGlissandoAllowed = false;
				ImpactPointExtent = 4;
				PointingSphereScale = PointingSphereMaxSize / 5;
				PointingSphere->SetWorldScale3D(FVector(PointingSphereScale));
				OldHitComponent = HitComponent;

				//SetPointingSphereInnerRadius(0);
			}
			else
			{
				bGlissandoAllowed = false;
				PointingSphereScale = PointingSphereMaxSize / 5;
				PointingSphere->SetWorldScale3D(FVector(PointingSphereScale));
			}

			// DrawDebugBox(GetWorld(), HitResult.ImpactPoint, FVector(ImpactPointExtent), FColor::Blue, false, -1.f, ECC_WorldStatic, 1.f);


			if (PointingSphere->GetComponentScale().X >= PointingSphereMaxSize)
			{
				
				UE_LOG(LogTemp, Warning, TEXT("This far: 4"));
				bGlissandoAllowed = false;
				CurrentObjectMaterials = HitComponent->ListOfMaterials;
				AffectedObject = SMComponent;
				/*PointingSphereScale = 0;
				PointingSphere->SetWorldScale3D(FVector(PointingSphereScale));*/
				PointingSphere->SetVisibility(false);
				ActualChangeMaterialText->SetVisibility(false);
				ActualTeleportText->SetVisibility(false);
				CurrentHitComponent = HitComponent;
				ShowUserInterface();
			}
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, -1.f, ECC_WorldStatic, 1.f);
		bObjectPointingAtValid = false;
		PointingSphere->SetVisibility(false);
		ActualChangeMaterialText->SetVisibility(true);
	}
}

void APlayerCharacter::ShowUserInterface()
{
	// Used for the tutorial at the beginning
	if (bAwaitHoldingMatChangeButton)
	{
		PlayIntroSound("ClickOnSphere");
		FTimerHandle ClickHandle;
		bAwaitHoldingMatChangeButton = false;
		GetWorldTimerManager().SetTimer(ClickHandle, this, &APlayerCharacter::AwaitClickingOnSphere, 4.6f);
		if (StrongHand == RightHand)
		{
			ControllerRight->SetMaterial(2, ControllerDefaultMaterial);
		}
		else
		{
			ControllerLeft->SetMaterial(2, ControllerDefaultMaterial);
		}
	}


	// Distance between clickable materials in degrees
	int32 UIMargin = 25;

	// Rotation of the spawned clickable materials
	SpawnRotation = FRotator(0, 0, 0);

	CurrentlySpawnedMaterials.Reset();


	FHitResult HitResult;
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + Camera->GetForwardVector() * 50;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredComponent(PointingSphere);
	TraceParams.AddIgnoredComponent(RightHand);
	TraceParams.AddIgnoredComponent(LeftHand);
	TraceParams.AddIgnoredComponent(RightPointFaceButton);
	TraceParams.AddIgnoredComponent(RightPointThumbstick);
	TraceParams.AddIgnoredComponent(RightRestFaceButton);
	TraceParams.AddIgnoredComponent(RightRestThumbstick);
	TraceParams.AddIgnoredComponent(LeftPointFaceButton);
	TraceParams.AddIgnoredComponent(LeftPointThumbstick);
	TraceParams.AddIgnoredComponent(LeftRestFaceButton);
	TraceParams.AddIgnoredComponent(LeftRestThumbstick);
	TraceParams.AddIgnoredComponent(TeleportText);
	TraceParams.AddIgnoredComponent(TeleportTextLeft);
	TraceParams.AddIgnoredComponent(ChangeMaterialText);
	TraceParams.AddIgnoredComponent(ChangeMaterialTextLeft);

	bool bHitObject;


	// XY Distance of the UI to the player
	int32 DistanceFromCamera = DistanceFromCameraDefault;

	

	bool bUseTwoMatRows = false;
	int32 MatsOnTopRow = 0;
	int32 MatsOnBottomRow = 0;
	if (CurrentObjectMaterials.Num() > 6) bUseTwoMatRows = true;

	// Offset from the center of the UI for the first (top left) clickable material
	int32 UIStartOffset;

	TArray<FVector> SpawnPoints;

	// Variables used for controlling whether it should try again at another position
	bool bMaterialsCanSpawn = false;
	bool bSpawnLocationStatusKnown = false;
	int32 TryNumber = 0;
	AActor* ActorHit = nullptr;
	UMaterialListComponent* ListComponent = nullptr;


	// Calculate where to start the row of materials
	UIStartOffset = -1 * (CurrentObjectMaterials.Num() - 1) / 2 * UIMargin;

	if (bUseTwoMatRows)
	{
		if (CurrentObjectMaterials.Num() % 2 == 0)
		{
			MatsOnTopRow = CurrentObjectMaterials.Num() / 2;
			MatsOnBottomRow = MatsOnTopRow;
		}
		else
		{
			MatsOnTopRow = (CurrentObjectMaterials.Num() + 1) / 2;
			MatsOnBottomRow = MatsOnTopRow - 1;
		}
		UIStartOffset = -1 * MatsOnTopRow / 2 * UIMargin;
	}

	UE_LOG(LogTemp, Warning, TEXT("This far: 5"));

	do
	{
		TryNumber++;
		int32 InvalidLocations = 0;
		
		
		// Set the UIHelperBase to the camera's position and rotation
		FRotator UIHelperRotation = Camera->GetComponentRotation();
		UIHelperRotation += FRotator(0, UIStartOffset, 0);
		UIHelperRotation.Pitch = 0;
		UIHelperRotation.Roll = 0;
		FVector UIHelperLocation = Camera->GetComponentLocation();
		UIHelper->SetWorldLocation(UIHelperLocation);
		UIHelper->SetWorldRotation(UIHelperRotation);

		if (TryNumber == 2) UIHelper->AddWorldRotation(FRotator(0, -60, 0));
		if (TryNumber == 3) UIHelper->AddWorldRotation(FRotator(0, 60, 0));
		if (TryNumber == 4) UIHelper->AddWorldRotation(FRotator(0, -120, 0));
		if (TryNumber == 5) UIHelper->AddWorldRotation(FRotator(0, 120, 0));
		if (TryNumber == 6) UIHelper->AddWorldRotation(FRotator(0, 180, 0));

		SpawnPoints.Reset();
		
		Start = UIHelper->GetComponentLocation();

		// Define a comfortable height of SpawnLocation
		FVector OffsetFromCamera = Camera->GetComponentLocation() - FVector(0, 0, 60);
		if (bUseTwoMatRows) OffsetFromCamera += FVector(0, 0, HeightDifference);
		if (bUseTwoMatRows) DistanceFromCamera = DistanceFromCameraWide;

		for (int32 i = 0; i < CurrentObjectMaterials.Num(); i++)
		{
			if (bUseTwoMatRows && i == MatsOnTopRow)
			{
				// Set properties for spheres on second row
				DistanceFromCamera = DistanceFromCameraDefault;
				OffsetFromCamera = Camera->GetComponentLocation() - FVector(0, 0, 60);
				
				UIHelper->SetWorldLocation(UIHelperLocation);
				UIHelper->SetWorldRotation(UIHelperRotation);

				if (MatsOnTopRow == MatsOnBottomRow)
				{
					UIHelper->AddWorldRotation(FRotator(0, -UIMargin / 2, 0));
				}

				if (TryNumber == 2) UIHelper->AddWorldRotation(FRotator(0, -60, 0));
				if (TryNumber == 3) UIHelper->AddWorldRotation(FRotator(0, 60, 0));
				if (TryNumber == 4) UIHelper->AddWorldRotation(FRotator(0, -120, 0));
				if (TryNumber == 5) UIHelper->AddWorldRotation(FRotator(0, 120, 0));
				if (TryNumber == 6) UIHelper->AddWorldRotation(FRotator(0, 180, 0));
			}

			FVector PotentialLocation = UIHelper->GetComponentLocation() + UIHelper->GetForwardVector() * DistanceFromCamera;
			PotentialLocation.Z = OffsetFromCamera.Z;

			FVector TraceEndLocation = PotentialLocation + (UIHelper->GetForwardVector() + UIHelper->GetUpVector() * -1) * 6;
			bHitObject = GetWorld()->LineTraceSingleByChannel(HitResult, Start, TraceEndLocation, ECC_Visibility, TraceParams);

			if (bHitObject)
			{
				if (bVisualizeMaterialSphereValidity) DrawDebugLine(GetWorld(), Start, TraceEndLocation, FColor::Orange, false, 5.f, ECC_WorldStatic, 1.f);
				InvalidLocations++;
				ActorHit = HitResult.GetActor();
			}
			else
			{
				if (bVisualizeMaterialSphereValidity) DrawDebugLine(GetWorld(), Start, TraceEndLocation, FColor::Green, false, 5.f, ECC_WorldStatic, 1.f);
				UE_LOG(LogTemp, Warning, TEXT("v"));
			}

			SpawnPoints.Add(PotentialLocation);
			UIHelper->AddWorldRotation(FRotator(0, UIMargin, 0));
		}

		if (InvalidLocations == 0)
		{
			FVector TraceEndLocation = UIHelper->GetComponentLocation() + UIHelper->GetForwardVector() * FVector(DistanceFromCamera + 5, DistanceFromCamera + 5, 0);
			TraceEndLocation.Z = OffsetFromCamera.Z;

			// chech if the Apply Button could spawn
			bool bHitObjectForApplyButton = GetWorld()->LineTraceSingleByChannel(HitResult, Start, TraceEndLocation, ECC_Visibility, TraceParams);
			if (bHitObjectForApplyButton)
			{
				bSpawnLocationStatusKnown = false;
				bMaterialsCanSpawn = false;
				if (bVisualizeMaterialSphereValidity) DrawDebugLine(GetWorld(), Start, TraceEndLocation, FColor::Purple, false, 5.f, ECC_WorldStatic, 1.f);
			}
			else
			{
				if (bVisualizeMaterialSphereValidity) DrawDebugLine(GetWorld(), Start, TraceEndLocation, FColor::Green, false, 5.f, ECC_WorldStatic, 1.f);
				bMaterialsCanSpawn = true;
				bSpawnLocationStatusKnown = true;
				UE_LOG(LogTemp, Warning, TEXT("SPI: Materials can spawn"));
				break;
			}

		}
		else if (InvalidLocations > 0 && TryNumber == 6)
		{
			bMaterialsCanSpawn = false;
			bSpawnLocationStatusKnown = true;
		}
		else
		{
			bMaterialsCanSpawn = false;
			bSpawnLocationStatusKnown = false;
		}
	} while (bSpawnLocationStatusKnown == false);

	UE_LOG(LogTemp, Warning, TEXT("This far: 6"));


	if (bMaterialsCanSpawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("This far: 8"));
		// Rotation of the spawned clickable materials
		SpawnRotation = FRotator(0, 0, 0);
		SpawnRotation.Yaw = 0;
		SpawnRotation.Pitch = 0;
		SpawnRotation.Roll = 0;

		// Play audio that says where the spheres are if they are not in front of the user
		if (TryNumber > 1)
		{
			PlayVoiceNoSpace("OtherObject");

		}
		if (TryNumber == 2) PlayVoiceNoSpace("Left");
		if (TryNumber == 3) PlayVoiceNoSpace("Right");
		if (TryNumber == 4) PlayVoiceNoSpace("LeftBack");
		if (TryNumber == 5) PlayVoiceNoSpace("RightBack");
		if (TryNumber == 6) PlayVoiceNoSpace("Back");

		// Spawn the spheres
		for (int32 i = 0; i < CurrentObjectMaterials.Num(); i++)
		{
			NewMaterial = CurrentObjectMaterials[i];


			SpawnLocation = SpawnPoints[i];

			// Set a comfortable height of SpawnLocation
			FVector OffsetFromCamera = Camera->GetComponentLocation() - FVector(0, 0, 60);
			//SpawnLocation.Z = OffsetFromCamera.Z;
			/*SpawnRotation = FRotator(1,1)*/

			SpawnRotation = UIHelper->GetComponentRotation();
			/*SpawnRotation.Roll = 0;
			SpawnRotation.Pitch = 0;
			SpawnRotation.Yaw += 180;
			SpawnRotation += ClickableRotation;*/

			SpawnClickableMaterial();
			UE_LOG(LogTemp, Warning, TEXT("This far: 7"));

		}


		// Set SpawnLocation of the Apply Button
		SpawnLocation = UIHelper->GetComponentLocation() + UIHelper->GetForwardVector() * FVector(DistanceFromCamera, DistanceFromCamera, 0);
		FVector OffsetFromCamera = Camera->GetComponentLocation() - FVector(0, 0, 60);
		SpawnLocation.Z = OffsetFromCamera.Z;
		SpawnRotation = UIHelper->GetComponentRotation() - FRotator(0, -90, 0);
		SpawnRotation.Pitch = 0;
		SpawnRotation.Roll = 0;

		// Spawn Apply Button
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			SpawnedApplyButton = World->SpawnActor<AApplyButton>(ApplyToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
		}


		bCurrentlyChangingMaterials = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Materials can't spawn"));
	}
}

void APlayerCharacter::SpawnClickableMaterial()
{
	if (WhatToSpawn != NULL)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;


			AClickableMaterial* SpawnedClickable = World->SpawnActor<AClickableMaterial>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
			SpawnedClickable->ApplyMaterial(NewMaterial);
			CurrentlySpawnedMaterials.Add(SpawnedClickable);

			if (NewMaterial == AffectedObject->GetMaterial(0))
			{
				Highlighter->RegisterComponent();
				Highlighter->SetWorldLocation(SpawnLocation);
			}
		}
	}
}

void APlayerCharacter::HandleClickingMaterialSpheres()
{
	FVector PointerLocation = RightHand->GetComponentLocation() + RightHand->GetForwardVector() * 7 + RightHand->GetUpVector() * -2 + RightHand->GetRightVector() * 4;

	FVector PointerLocationLeft = LeftHand->GetComponentLocation() + LeftHand->GetForwardVector() * 7 + LeftHand->GetUpVector() * -2 + LeftHand->GetRightVector() * -4;
	/*DrawDebugSphere(GetWorld(), PointerLocationLeft, 3, 32, FColor(255, 0, 255));
	DrawDebugSphere(GetWorld(), PointerLocation, 3, 32, FColor(255, 0, 255));*/
	float SizeOfAll = 0;

	if (bCurrentlyChangingMaterials && !bCurrentlyTeleporting && CurrentlySpawnedMaterials.Num() > 0)
	{
		for (AClickableMaterial* SpawnedClickable : CurrentlySpawnedMaterials)
		{
			float MaxDistance = 10;
			Distance = (SpawnedClickable->GetActorLocation() - PointerLocation).Size();
			LeftDistance = (SpawnedClickable->GetActorLocation() - PointerLocationLeft).Size();
			float DistanceToLeft = (SpawnedClickable->GetActorLocation() - PointerLocationLeft).Size();
			float DistanceToRight = (SpawnedClickable->GetActorLocation() - PointerLocation).Size();

			float DistanceToNearest;
		

			if (!bRightPointing && !bLeftPointing)
			{
				return;
			}
			else if (bRightPointing && bLeftPointing)
			{
				if (DistanceToLeft < DistanceToRight)
				{
					DistanceToNearest = DistanceToLeft;
				}
				else
				{
					DistanceToNearest = DistanceToRight;
				}
			}
			else if (bRightPointing && !bLeftPointing)
			{
				DistanceToNearest = DistanceToRight;
			}
			else if (bLeftPointing && !bRightPointing)
			{
				DistanceToNearest = DistanceToLeft;
			}

			float Scale = SpawnedClickable->MaterialMeshScale;
			FVector ClickableScale = FVector(Scale);
			FVector OutFVector;
			FVector DistVector;

			// Make the SpawnedClickable smaller when clicking it
			if (DistanceToNearest < MaxDistance && DistanceToNearest > MaxDistance * 0.85)
			{
				FVector ScaleFactor = FVector(DistanceToNearest / MaxDistance, DistanceToNearest / MaxDistance, DistanceToNearest / MaxDistance);
				SpawnedClickable->SetActorScale3D(ClickableScale * ScaleFactor);
				AllowOtherSounds();
				
				// Scale the highlighter as well if clicking the already selected material
				if (AffectedObject->GetMaterial(0) == SpawnedClickable->AppliedMaterial)
				{
					Highlighter->SetWorldScale3D(HighlighterScale * ScaleFactor);
				}

				if (bRecentlyClicked)
				{
					PlayReleaseSound();
					bRecentlyClicked = false;
				}
			}

			// Lock Scale at certain point, apply material and play a click sound;
			if (DistanceToNearest <= MaxDistance * 0.85)
			{
				if (!bCurrentlyPlayingSounds)
				{
					PlayClickSound();
					bRecentlyClicked = true;
					SuppressOtherSounds();
				}
				SpawnedClickable->SetActorScale3D(ClickableScale * 0.85);
				SMComponentMaterial = SpawnedClickable->AppliedMaterial;
				

				// Lock highlighter's scale
				if (AffectedObject->GetMaterial(0) == SpawnedClickable->AppliedMaterial)
				{
					Highlighter->SetWorldScale3D(HighlighterScale * 0.85);
				}
				Highlighter->SetWorldLocation(HighlighterLocation);
				HighlighterLocation = SpawnedClickable->GetActorLocation();
				
				// just in case if the object's material isn't an item of the array with materials
				Highlighter->RegisterComponent();
				
				AffectedObject->SetMaterial(0, SMComponentMaterial);
				bClickedOnMat = true;
			}

			// Set Scale of SpawnedClickable to default scale if the players's hand isn't touching it
			if (DistanceToNearest > MaxDistance)
			{
				SpawnedClickable->SetActorScale3D(ClickableScale);

				// Reset highlighter's scale
				if (AffectedObject->GetMaterial(0) == SpawnedClickable->AppliedMaterial)
				{
					Highlighter->SetWorldScale3D(HighlighterScale);
				}
			}
			SizeOfAll += SpawnedClickable->GetActorScale3D().Z;
		}
	}
}

void APlayerCharacter::UpdateApplyButton()
{
	FVector VectorToHand = SpawnedApplyButton->GetActorLocation() - RightHand->GetComponentLocation();
	float DistanceToHand = VectorToHand.Size();

	FVector VectorToLeftHand = SpawnedApplyButton->GetActorLocation() - LeftHand->GetComponentLocation();
	float DistanceToLeftHand = VectorToLeftHand.Size();

	if (DistanceToLeftHand < DistanceToHand)
	{
		DistanceToHand = DistanceToLeftHand;
	}

	// Grow inner sphere of ApplyButton if user's hand is near, reset size if it isn't
	if (DistanceToHand < 17)
	{
		GreenAreaSize += 1.8 * GetWorld()->GetDeltaSeconds();
		SpawnedApplyButton->UpdateGreenArea(GreenAreaSize);
	}
	else {
		GreenAreaSize = 0;
		SpawnedApplyButton->UpdateGreenArea(GreenAreaSize);
	}

	// Destroy all actors of the UI when ApplyButton is fully "loaded"
	if (GreenAreaSize >= 1)
	{
		SpawnedApplyButton->Destroy();
		ActualTeleportText->SetVisibility(true);
		bCurrentlyChangingMaterials = false;
		for (AClickableMaterial* SpawnedClickable : CurrentlySpawnedMaterials)
		{
			SpawnedClickable->Destroy();
			GreenAreaSize = 0;
			SpawnedApplyButton = nullptr;
		}
		bAppliedMat = true;
		if (bAwaitApplying)
		{
			bAppliedMat = true;
			bAwaitApplying = false;
		}

		// Hide highlighter
		Highlighter->UnregisterComponent();
	}
}

void APlayerCharacter::WantToChangeMaterialRight()
{
	if (bIsRightHanded)
	{
		WantToChangeMaterial();
	}
	else
	{
		return;
	}
}

void APlayerCharacter::DontWantToChangeMaterialRight()
{
	if (bIsRightHanded)
	{
		DontWantToChangeMaterial();
	}
	else
	{
		return;
	}
}

void APlayerCharacter::WantToChangeMaterialLeft()
{

	if (!bIsRightHanded)
	{
		WantToChangeMaterial();
	}
	else
	{
		return;
	}
}

void APlayerCharacter::DontWantToChangeMaterialLeft()
{
	if (!bIsRightHanded)
	{
		DontWantToChangeMaterial();
	}
	else
	{
		return;
	}
}

void APlayerCharacter::WantToChangeMaterial()
{
	bCurrentlyWantingToChangeMaterial = true;
	if (!bObjectPointingAtValid)
	{
		PlayErrorSound();
	}
	else
	{
		bGlissandoAllowed = true;
	}
}

void APlayerCharacter::DontWantToChangeMaterial()
{
	bCurrentlyWantingToChangeMaterial = false;
	bGlissandoAllowed = false;
	StopGlissSound();
}

/* =================================== */
/* HAND GESTURES */

void APlayerCharacter::HandleFingerPlacement()
{
	if (bRightFaceButton && bRightPointing)
	{
		RightPointFaceButton->SetVisibility(true);
		RightPointThumbstick->SetVisibility(false);
		RightRestFaceButton->SetVisibility(false);
		RightRestThumbstick->SetVisibility(false);

	}
	else if (!bRightFaceButton && bRightPointing)
	{
		RightPointFaceButton->SetVisibility(false);
		RightPointThumbstick->SetVisibility(true);
		RightRestFaceButton->SetVisibility(false);
		RightRestThumbstick->SetVisibility(false);
	}
	else if (bRightFaceButton && !bRightPointing)
	{
		RightPointFaceButton->SetVisibility(false);
		RightPointThumbstick->SetVisibility(false);
		RightRestFaceButton->SetVisibility(true);
		RightRestThumbstick->SetVisibility(false);
	}
	else
	{
		RightPointFaceButton->SetVisibility(false);
		RightPointThumbstick->SetVisibility(false);
		RightRestFaceButton->SetVisibility(false);
		RightRestThumbstick->SetVisibility(true);
	}

	if (bLeftFaceButton && bLeftPointing)
	{
		UE_LOG(LogTemp, Warning, TEXT("Point and Press A !!!!!!!!!!"));
		LeftPointFaceButton->SetVisibility(true);
		LeftPointThumbstick->SetVisibility(false);
		LeftRestFaceButton->SetVisibility(false);
		LeftRestThumbstick->SetVisibility(false);

	}
	else if (!bLeftFaceButton && bLeftPointing)
	{
		LeftPointFaceButton->SetVisibility(false);
		LeftPointThumbstick->SetVisibility(true);
		LeftRestFaceButton->SetVisibility(false);
		LeftRestThumbstick->SetVisibility(false);
	}
	else if (bLeftFaceButton && !bLeftPointing)
	{
		LeftPointFaceButton->SetVisibility(false);
		LeftPointThumbstick->SetVisibility(false);
		LeftRestFaceButton->SetVisibility(true);
		LeftRestThumbstick->SetVisibility(false);
	}
	else
	{
		LeftPointFaceButton->SetVisibility(false);
		LeftPointThumbstick->SetVisibility(false);
		LeftRestFaceButton->SetVisibility(false);
		LeftRestThumbstick->SetVisibility(true);
	}
}

void APlayerCharacter::PointRight()
{
	RightPointFaceButton->RegisterComponent();
	RightRestFaceButton->UnregisterComponent();
	bCurrentlyPointing = true;
}

void APlayerCharacter::MakeFistRight()
{
	RightPointFaceButton->UnregisterComponent();
	RightRestFaceButton->RegisterComponent();
	bCurrentlyPointing = false;
}

void APlayerCharacter::ActivateRightGrip()
{
	bRightGrip = true;

}

void APlayerCharacter::DeactivateRightGrip()
{
	bRightGrip = false;
}

void APlayerCharacter::ActivateRightPointing()
{
	bRightPointing = true;
	//UE_LOG(LogTemp, Warning, TEXT("Pointing Right"));
}

void APlayerCharacter::DeactivateRightPointing()
{
	bRightPointing = false;
}

void APlayerCharacter::ActivateRightFaceButton()
{
	bRightFaceButton = true;
	//UE_LOG(LogTemp, Warning, TEXT("Pointing Right"));

}

void APlayerCharacter::DeactivateRightFaceButton()
{
	bRightFaceButton = false;
}

void APlayerCharacter::ActivateLeftPointing()
{
	bLeftPointing = true;
	//UE_LOG(LogTemp, Warning, TEXT("Pointing Left"));
}

void APlayerCharacter::DeactivateLeftPointing()
{
	bLeftPointing = false;
}

void APlayerCharacter::ActivateLeftFaceButton()
{
	bLeftFaceButton = true;
	//UE_LOG(LogTemp, Warning, TEXT("Pointing Left"));
}

void APlayerCharacter::DeactivateLeftFaceButton()
{
	bLeftFaceButton = false;
}

/* =================================== */
/* AUDIO */

// Overridden in Blueprint since it's easier for working with sound cues
void APlayerCharacter::PlayClickSound_Implementation()
{
}
void APlayerCharacter::PlayVoiceNoSpace_Implementation(const FString &Message)
{
}
void APlayerCharacter::PlayIntroSound_Implementation(const FString &Message)
{
}
void APlayerCharacter::PlayReleaseSound_Implementation()
{
}
void APlayerCharacter::PlayErrorSound_Implementation()
{
}
void APlayerCharacter::PlayGlissSound_Implementation()
{
}
void APlayerCharacter::StopGlissSound_Implementation()
{
}

// Used to stop glissando when pointing at different material or stopping to press A
void APlayerCharacter::SuppressOtherSounds()
{
	bCurrentlyPlayingSounds = true;
}
void APlayerCharacter::AllowOtherSounds()
{
	bCurrentlyPlayingSounds = false;
}


/* =================================== */
/* INTRO */

void APlayerCharacter::StartIntro()
{
	bExperienceHasStarted = true;
	PlayIntroSound("Welcome");
	PC->PlayerCameraManager->StopCameraFade();
	PC->PlayerCameraManager->StartCameraFade(1, 0, 6.f, FLinearColor::Black);
	PlayIntroSound("StrongHand");
	bWaitingForHand = true;
}

void APlayerCharacter::StartExperience()
{
	UE_LOG(LogTemp, Warning, TEXT("Experience has started"));
	if (!bExperienceHasStarted) StartIntro();
}

void APlayerCharacter::SkipIntro()
{
	UE_LOG(LogTemp, Warning, TEXT("Experience has started"));
	if (!bExperienceHasStarted)
	{
		PC->PlayerCameraManager->StopCameraFade();
		PC->PlayerCameraManager->StartCameraFade(1, 0, 6.f, FLinearColor::Black);
		bExperienceHasStarted = true;
		bAllowPointingAtObject = true;
		bTeleportationAllowed = true;
		bSuccessfullyChangedMaterials = true;
		bIsRightHanded = true;

		ActualChangeMaterialText->SetVisibility(true);
		ActualTeleportText->SetVisibility(true);
		OpenEntryDoors();
		bAppliedMat = true;
	}
}

void APlayerCharacter::ExplainMatChange()
{
	PlayIntroSound("ExplainMatChange");
	PlayIntroSound("Pointing");

	if (StrongHand == RightHand)
	{
		ControllerRight->SetMaterial(2, ControllerGlowingMaterial);
	}
	else
	{
		ControllerLeft->SetMaterial(2, ControllerGlowingMaterial);
	}

	ActualChangeMaterialText->SetVisibility(true);

	FTimerHandle PointingTimerHandle;
	GetWorldTimerManager().SetTimer(PointingTimerHandle, this, &APlayerCharacter::AwaitPointing, 7.9f);
}

void APlayerCharacter::AwaitPointing()
{
	bAwaitPointing = true;
	bAllowPointingAtObject = true;
}

void APlayerCharacter::ExplainPressing()
{
	if (StrongHand == RightHand)
	{
		
	}
	PlayIntroSound("GreenLinePress");
}

void APlayerCharacter::AwaitClickingOnSphere()
{
	bAwaitClickingOnSphere = true;
}

void APlayerCharacter::AllowPlayTeleportTask()
{
	bAllowPlayTeleportTask = true;
}

void APlayerCharacter::SetHandMaterial(FString LeftOrRight, UMaterialInterface* NewHandMaterial)
{
	if (LeftOrRight == "Left")
	{
		LeftRestFaceButton->SetMaterial(0, NewHandMaterial);
		LeftPointFaceButton->SetMaterial(0, NewHandMaterial);
		LeftPointThumbstick->SetMaterial(0, NewHandMaterial);
		LeftRestThumbstick->SetMaterial(0, NewHandMaterial);
	}
	else if (LeftOrRight == "Right")
	{
		RightRestFaceButton->SetMaterial(0, NewHandMaterial);
		RightPointFaceButton->SetMaterial(0, NewHandMaterial);
		RightPointThumbstick->SetMaterial(0, NewHandMaterial);
		RightRestThumbstick->SetMaterial(0, NewHandMaterial);
	}
}

/* =================================== */
/* VARIA */

// Makes the Actor follow the X and Y position of the Camera to correct weird collision when the player isn't in the center of the play area
void APlayerCharacter::CorrectCollision()
{
	FVector CameraPos = Camera->GetComponentLocation();
	FVector ActorPos = GetActorLocation();
	FVector NewActorPos = CameraPos;
	NewActorPos.Z = ActorPos.Z;
	SetActorLocation(NewActorPos);
	FVector CDistance = ActorPos - CameraPos;
	CDistance.Z = 0;
	CameraHolder->AddWorldOffset(CDistance);
}

void APlayerCharacter::ChangeLanguage()
{
	bIsEnglish = !bIsEnglish;
}

float APlayerCharacter::SmoothlyCalculatePositionAlongWay(float Progress)
{
	float Position = -0.5 * UKismetMathLibrary::Cos(Progress * UKismetMathLibrary::GetPI()) + 0.5;
	return Position;
}

FVector APlayerCharacter::GetLiftSpecCamLocation()
{

	UArrowComponent* SceneComponent = Lift->FindComponentByClass<UArrowComponent>();
	return SceneComponent->GetComponentLocation();
}


FRotator APlayerCharacter::GetLiftSpecCamRotation()
{
	UArrowComponent* SceneComponent = Lift->FindComponentByClass<UArrowComponent>();
	return SceneComponent->GetComponentRotation();
}
