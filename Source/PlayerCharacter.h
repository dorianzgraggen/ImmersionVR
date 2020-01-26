// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

//Forward declarations
class AClickableMaterial;
class UMaterialListComponent;
class AApplyButton;

UCLASS()
class INTERACTIVE_FOYER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OpenEntryDoors();

	void HandleClickingMaterialSpheres();

	void UpdateApplyButton();

	void HandleFingerPlacement();

	void CorrectCollision();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UCameraComponent* Camera;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Changing Materials|User Interface")
		TSubclassOf<class AClickableMaterial> WhatToSpawn;

	UPROPERTY(EditAnywhere, Category = "Changing Materials|User Interface")
		TSubclassOf<class AApplyButton> ApplyToSpawn;

private:
	void SpawnClickableMaterial();

	

	UPROPERTY(VisibleAnywhere)
		class USceneComponent* CameraHolder;

	UPROPERTY(VisibleAnywhere)
		class USceneComponent* UIHelper;

	UPROPERTY(VisibleAnywhere)
		class USceneComponent* UIHelperBase;

	UPROPERTY(VisibleAnywhere)
		class USceneComponent* SpectatorHelper;

	UPROPERTY(VisibleAnywhere)
		class USceneComponent* SpectatorCamHolder;

	UPROPERTY(VisibleAnywhere)
		class USceneCaptureComponent2D* SpectatorCam;

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* PlayerHead;

	float SmoothlyCalculatePositionAlongWay(float Progress);

	bool bPlayerHasRecentlyTeleported = true;
	bool bSpectatorCamMustMove = false;
	bool bSpectatorCamMustAdjustRotation = false;
	bool bLiftIsDominant = false;
	bool bFlyToLiftSpec;

	UPROPERTY(VisibleAnywhere)
	class ALift* Lift;

	FVector OldSpecCamLocation = FVector(0);
	FRotator OldSpecCamRotation = FRotator(0);

	FVector TravelVector = FVector(0);
	FRotator TravelRotator = FRotator(0);
	float ProgressOnTravel = 0;

	FVector GetLiftSpecCamLocation();
	FRotator GetLiftSpecCamRotation();

	UPROPERTY(VisibleAnywhere)
		class USceneComponent* HandDirection;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* TeleportationMarker;

	UPROPERTY(VisibleAnywhere)
		class USplineComponent* ArcPath;

	UPROPERTY(EditAnywhere)
		TArray<class USplineMeshComponent*> ArcMeshPool;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Highlighter;

	FVector HighlighterLocation = FVector(0);
	FVector HighlighterScale = FVector(0);

	// HANDS
	bool bRightGrip = false;
	void ActivateRightGrip();
	void DeactivateRightGrip();

	bool bRightPointing = false;
	void ActivateRightPointing();
	void DeactivateRightPointing();

	bool bRightFaceButton = false;
	void ActivateRightFaceButton();
	void DeactivateRightFaceButton();

	bool bLeftPointing = false;
	void ActivateLeftPointing();
	void DeactivateLeftPointing();

	bool bLeftFaceButton = false;
	void ActivateLeftFaceButton();
	void DeactivateLeftFaceButton();

	class UMotionControllerComponent* StrongHand;
	class UMotionControllerComponent* WeakHand;

	class UStaticMeshComponent* ActualTeleportText;
	class UStaticMeshComponent* ActualChangeMaterialText;


	FTimerHandle TimerHandle;

	UPROPERTY(VisibleAnywhere)
	AApplyButton* SpawnedApplyButton = nullptr;

	void OnHandOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	float GreenAreaSize = 0;

	void MoveForward(float Speed);
	void MoveRight(float Speed);
	void UpdateTeleportationElements();
	void Teleport();
	void ShowArc();
	void HideArc();
	void DisallowTeleportation(float Seconds);
	void AllowTeleportation();
	bool bTeleportationAllowed = false;

	void UpdateArc(const TArray<FVector> &Path);

	bool bDrawArc = false;
	bool bHasDestination;

	bool bArcHasBeenDrawn = false;

	void DrawArc(const TArray<FVector> &Path);

	UPROPERTY(EditAnywhere, Category = "Teleportation")
		float MaxTeleportDistance = 5000;

	bool StillTeleporting = false;
	FVector TeleportDestination;

	UPROPERTY(EditAnywhere, Category = "Teleportation")
		FVector TeleportProjectionExtent = FVector(100, 100, 100);

	bool isDestinationValid(TArray<FVector> &OutPath, FVector &OutLocation);

	UPROPERTY()
		class UMotionControllerComponent* RightHand;

	UPROPERTY()
		class UMotionControllerComponent* LeftHand;

	UPROPERTY(EditAnywhere, Category = "Teleportation")
		float InProjectileRadius = 10;

	UPROPERTY(EditAnywhere, Category = "Teleportation")
		float ProjectileSpeed = 1000;

	UPROPERTY(EditAnywhere, Category = "Teleportation")
		float ProjectileSimTime = 3;

	UPROPERTY(VisibleAnywhere)
		FVector SpawnLocation;

	UPROPERTY(VisibleAnywhere)
		FRotator SpawnRotation;

	/*UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* LeftFist;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* LeftFinger;*/

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* RightRestFaceButton;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* RightPointFaceButton;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* RightRestThumbstick;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* RightPointThumbstick;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* LeftRestFaceButton;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* LeftPointFaceButton;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* LeftRestThumbstick;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* LeftPointThumbstick;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* HandMaterial;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* HandRaisedMaterial;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* ControllerGlowingMaterial;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* ControllerDefaultMaterial;

	bool bIsRightHandRaised = false;
	bool bIsLeftHandRaised = false;

	void SetHandMaterial(FString LeftOrRight, UMaterialInterface* NewHandMaterial);

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* ChangeMaterialText;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* TeleportText;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* ChangeMaterialTextLeft;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* TeleportTextLeft;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* ControllerLeft;

	UPROPERTY(EditAnywhere)
		USoundBase* SuccessSound;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* ControllerRight;

	UPROPERTY(EditDefaultsOnly, Category = "Teleportation")
		class UStaticMesh* ArcMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Teleportation")
		class UMaterialInterface* ArcMaterial;

	UPROPERTY(VisibleAnywhere, Category = "Changing Materials|Pointing")
		class UStaticMeshComponent* PointingSphere;

	UPROPERTY(EditAnywhere, Category = "Changing Materials|Pointing")
		UMaterialInstance* ObjectValidMaterial;

	UPROPERTY(EditAnywhere, Category = "Changing Materials|Pointing")
		UMaterialInstance* ObjectInvalidMaterial;

	bool bCurrentlyWantingToChangeMaterial = false;

	void WantToChangeMaterial();
	void DontWantToChangeMaterial();

	void WantToChangeMaterialLeft();
	void DontWantToChangeMaterialLeft();

	void WantToChangeMaterialRight();
	void DontWantToChangeMaterialRight();


	UPROPERTY(VisibleAnywhere)
		UMaterialInstanceDynamic* DynamicMaterial;

	float PointingSphereScale = 0;

	UPROPERTY(EditAnywhere, Category = "Changing Materials|Pointing")
	float PointingSphereGrowRate = 30;

	UPROPERTY(EditAnywhere, Category = "Changing Materials|Pointing")
	float PointingSphereGrowPerSecond = 1;

	UPROPERTY(EditAnywhere, Category = "Changing Materials|Pointing")
		float PointingSphereMaxSize = 5;
	
	UPROPERTY(EditAnywhere, Category = "Changing Materials|User Interface")
	float DistanceFromCameraDefault = 50;

	UPROPERTY(EditAnywhere, Category = "Changing Materials|User Interface")
	float DistanceFromCameraWide = 70;
	
	UPROPERTY(EditAnywhere, Category="Changing Materials|User Interface")
	float HeightDifference = 15;

	void MakeFistLeft();
	void PointLeft();
	void MakeFistRight();
	void PointRight();

	void ShowUserInterface();

	void FindObjectByPointing();

	bool bCurrentlyPointing = false;
	bool bCurrentlyPlayingSounds = false;
		
	void SuppressOtherSounds();
	void AllowOtherSounds();

	bool bRecentlyClicked = false;

	UPROPERTY(EditAnywhere, Category = "Teleportation")
	float TeleportationMarkerOffset = -20;

	UPROPERTY(EditAnywhere)
	float HandOffsetRotation = 20;

	UPROPERTY(EditAnywhere)
	float HandZRotation = 20;

	UPROPERTY(EditAnywhere, Category = "Changing Materials|User Interface")
	FVector LineLocationOffset = FVector(0, 2, -2);

	float ImpactPointExtent = 4;

	UPROPERTY(EditAnywhere, Category = "Changing Materials|User Interface")
	FRotator ClickableRotation = FRotator(-45, 0, 0);

	UPROPERTY(EditAnywhere)
	bool UseClickableButton = true;

	UPROPERTY()
	FVector EntryPoint = FVector(NULL);

	UPROPERTY()
	FVector EntryClickablePosition;

	UPROPERTY()
	float DistanceToEntryPoint = -1;

	float Distance;
	float LeftDistance;

	UPROPERTY()
		bool inside = false;

	// Controls for player status
	UPROPERTY(VisibleAnywhere)
	bool bCurrentlyChangingMaterials = false;

	UPROPERTY(VisibleAnywhere)
	bool bCurrentlyTeleporting = false;

	APlayerController* PC;


public:
	// Intro Stuff
	void StartIntro();
	void StartExperience();
	void ExplainMatChange();
	void ExplainPressing();
	void AwaitClickingOnSphere();
	void AwaitPointing();
	void AllowPlayTeleportTask();

	void SkipIntro();

	float RightHandRaisedCurrentSeconds = 0;
	float LeftHandRaisedCurrentSeconds = 0;
	float HandRaisedAcceptedSeconds = 3;

	bool bWaitingForHand = false;
	bool bExperienceHasStarted = false;
	bool bHandRaisedEnough = false;
	bool bAwaitPointing = false;
	bool bAwaitHoldingMatChangeButton = false;
	bool bAwaitClickingOnSphere = false;
	bool bAwaitApplying;
	bool bClickedOnMat = false;
	bool bAllowPointingAtObject;
	bool bAllowPlayTeleportTask;
	bool bAppliedMat = false;
	bool bTeleportTried = false;
	bool bSuccessfullyChangedMaterials = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "lol")
	bool bAllowPlayTeleportDestination = false;
	bool bTeleportFinished = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "lol")
	bool bAllowPlayEnjoy = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = lol)
	bool bIsRightHanded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsEnglish = false;

	void ChangeLanguage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = lol)
	int32 MusicSkillLevel;

	UPROPERTY(EditAnywhere, Category = "Changing Materials|User Interface")
	bool bVisualizeMaterialSphereValidity = false;

	UPROPERTY(VisibleAnywhere)
		TSubclassOf<AClickableMaterial> ClickableMaterialBlueprint;

	UPROPERTY(VisibleAnywhere)
	class UMaterialInterface* NewMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectMaterials")
	TArray<UMaterialInterface*> CurrentObjectMaterials;


	UPROPERTY(VisibleAnywhere)
	TArray<AClickableMaterial*> CurrentlySpawnedMaterials;

	UPROPERTY(VisibleAnywhere)
	UMaterialListComponent* CurrentHitComponent = nullptr;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* SMComponentMaterial;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SMComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* AffectedObject;

	UPROPERTY(VisibleAnywhere)
	UMaterialListComponent* OldHitComponent = nullptr;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ZZZ")
	void PlayIntroSound(const FString &Message);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ZZZ")
	void PlayVoiceNoSpace(const FString &Message);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ZZZ")
	void PlayClickSound();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ZZZ")
	void PlayReleaseSound();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ZZZ")
	void PlayErrorSound();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ZZZ")
	void PlayGlissSound();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ZZZ")
	void StopGlissSound();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZZZ")
	bool bGlissandoAllowed = false;

	

	UPROPERTY(Visibleanywhere)
	bool bObjectPointingAtValid = false;
};
