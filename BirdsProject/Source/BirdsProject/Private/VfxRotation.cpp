// Fill out your copyright notice in the Description page of Project Settings.


#include "VfxRotation.h"

// Sets default values for this component's properties
UVfxRotation::UVfxRotation()
{
	PrimaryComponentTick.bCanEverTick = false;
	_rotationSpeed = 1.3f;
	//_movementComponent = GetOwner()->FindComponentByClass<UFloatingPawnMovement>();
}

void UVfxRotation::InitializeVfx(AActor* leader, AActor* player)
{
	_leader = leader;
	_player = player;

	_vfxSpeed = 0.0f;
}

void UVfxRotation::MoveVfx(float deltaTime)
{
	//_vfxSpeed = _player->FindComponentByClass<UFloatingPawnMovement>()->MaxSpeed - 500.0f;
	//FVector MoveDirection = _player->GetActorForwardVector();
	//_movementComponent->AddInputVector(MoveDirection * _vfxSpeed);

	FVector newLocation = _player->GetActorLocation() + _player->GetActorForwardVector() * 350.0f;

	FQuat currentRotation = FQuat(GetOwner()->GetActorRotation());
	FQuat playerQuat = _player->GetActorQuat();
	FQuat newRotation = FQuat::Slerp(currentRotation, playerQuat, deltaTime * _rotationSpeed);

	GetOwner()->SetActorLocation(newLocation);
	GetOwner()->SetActorRotation(newRotation);
}


