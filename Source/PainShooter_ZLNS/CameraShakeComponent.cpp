// CameraShakeComponent.cpp

#include "CameraShakeComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Math/UnrealMathUtility.h"

UCameraShakeComponent::UCameraShakeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCameraShakeComponent::BeginPlay()
{
	Super::BeginPlay();

	// Cache camera manager (best-effort). In multiplayer this should be called on the owning client.
	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0))
		{
			CachedCameraManager = PC->PlayerCameraManager;
		}
	}

	if (AActor* Owner = GetOwner())
	{
		OriginalLocation = Owner->GetActorLocation();
	}
}

void UCameraShakeComponent::StartShake(float Duration, float Intensity)
{
	ShakeDuration = Duration;
	ShakeIntensity = Intensity;
	ShakeTimeRemaining = Duration;
	bIsShaking = true;

	// Preferred path: use camera shake via PlayerCameraManager.
	if (CachedCameraManager && CameraShakeClass)
	{
		// Stop previous instance if any
		if (ActiveShakeInstance)
		{
			CachedCameraManager->StopCameraShake(ActiveShakeInstance, false);
			ActiveShakeInstance = nullptr;
		}

		// Use the overload that's available across UE5 minor versions.
		ActiveShakeInstance = CachedCameraManager->StartCameraShake(CameraShakeClass, ShakeIntensity);
		return;
	}

	// Fallback: move the owning actor (less recommended, but keeps old behavior).
	if (AActor* Owner = GetOwner())
	{
		OriginalLocation = Owner->GetActorLocation();
	}
}

void UCameraShakeComponent::StopShake()
{
	if (CachedCameraManager && ActiveShakeInstance)
	{
		CachedCameraManager->StopCameraShake(ActiveShakeInstance, false);
		ActiveShakeInstance = nullptr;
	}

	if (AActor* Owner = GetOwner())
	{
		Owner->SetActorLocation(OriginalLocation);
	}
	bIsShaking = false;
	ShakeTimeRemaining = 0.0f;
}

void UCameraShakeComponent::TickComponent(
	float DeltaTime,
	enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsShaking)
		return;

	// If we're using camera shakes through the camera manager, we don't need to tick offsets.
	if (CachedCameraManager && ActiveShakeInstance)
	{
		ShakeTimeRemaining -= DeltaTime;
		if (ShakeTimeRemaining <= 0.0f)
		{
			StopShake();
		}
		return;
	}

	if (AActor* Owner = GetOwner())
	{
		ShakeTimeRemaining -= DeltaTime;
		if (ShakeTimeRemaining <= 0.0f)
		{
			Owner->SetActorLocation(OriginalLocation);
			bIsShaking = false;
			return;
		}

		const float Alpha = ShakeTimeRemaining / ShakeDuration;
		const float CurrentIntensity = ShakeIntensity * Alpha;

		const float OffsetX = FMath::FRandRange(-1.0f, 1.0f) * CurrentIntensity;
		const float OffsetY = FMath::FRandRange(-1.0f, 1.0f) * CurrentIntensity;
		const float OffsetZ = FMath::FRandRange(-1.0f, 1.0f) * CurrentIntensity;

		Owner->SetActorLocation(OriginalLocation + FVector(OffsetX, OffsetY, OffsetZ));
	}
}
