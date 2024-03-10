// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
//#include "GameFramework/FloatingPawnMovement.h"
#include "VfxRotation.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BIRDSPROJECT_API UVfxRotation : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UVfxRotation();

	UFUNCTION(BlueprintCallable, Category = "vfxRotation")
	void InitializeVfx(AActor* leader, AActor* player);
	UFUNCTION(BlueprintCallable, Category = "vfxRotation")
	void MoveVfx(float deltaTime);

private:
	AActor* _leader;
	AActor* _player;
	//UFloatingPawnMovement* _movementComponent;
	FQuat _targetQuat;
	float _rotationSpeed;
	float _vfxSpeed;
};