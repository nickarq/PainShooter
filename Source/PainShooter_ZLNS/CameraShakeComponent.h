// CameraShakeComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraShakeBase.h"
#include "CameraShakeComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PAINSHOOTER_ZLNS_API UCameraShakeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCameraShakeComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="CameraShake")
	void StartShake(float Duration = 0.5f, float Intensity = 1.0f);

	UFUNCTION(BlueprintCallable, Category="CameraShake")
	void StopShake();

	/** Clase de CameraShake a reproducir. Si está vacía, el componente usa un fallback simple moviendo el actor (menos recomendado). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CameraShake")
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

private:
	UPROPERTY(EditAnywhere, Category="CameraShake")
	float ShakeDuration = 0.5f;

	UPROPERTY(EditAnywhere, Category="CameraShake")
	float ShakeIntensity = 1.0f;

	bool bIsShaking = false;
	float ShakeTimeRemaining = 0.0f;
	FVector OriginalLocation;

	TObjectPtr<class APlayerCameraManager> CachedCameraManager = nullptr;
	UCameraShakeBase* ActiveShakeInstance = nullptr;
};
