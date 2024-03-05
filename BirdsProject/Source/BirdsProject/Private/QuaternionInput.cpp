// Fill out your copyright notice in the Description page of Project Settings.


#include "QuaternionInput.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UQuaternionInput::UQuaternionInput()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UQuaternionInput::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UQuaternionInput::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Formula to convert a Euler angle in degrees to a quaternion rotation
FQuat UQuaternionInput::Euler_To_Quaternion(FRotator Current_Rotation)
{
    FQuat q;                                            // Declare output quaternion
    float yaw = Current_Rotation.Yaw * PI / 180;        // Convert degrees to radians 
    float roll = Current_Rotation.Roll * PI / 180;
    float pitch = Current_Rotation.Pitch * PI / 180;

    double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);
    double cr = cos(roll * 0.5);
    double sr = sin(roll * 0.5);
    double cp = cos(pitch * 0.5);
    double sp = sin(pitch * 0.5);

    q.W = cy * cr * cp + sy * sr * sp;
    q.X = cy * sr * cp - sy * cr * sp;
    q.Y = cy * cr * sp + sy * sr * cp;
    q.Z = sy * cr * cp - cy * sr * sp;

    return q;                                           // Return the quaternion of the input Euler rotation
}

// Set the scene component's world rotation to the input quaternion
void UQuaternionInput::SetWorldRotationQuat(USceneComponent* SceneComponent, const FQuat& Desired_Rotation)
{
    if (SceneComponent)
    {
        SceneComponent->SetWorldRotation(Desired_Rotation);
    }
}

// Set the scene component's relative rotation to the input quaternion
void UQuaternionInput::SetRelativeRotationQuat(USceneComponent* SceneComponent, const FQuat& Desired_Rotation)
{
    if (SceneComponent)
    {
        SceneComponent->SetRelativeRotation(Desired_Rotation);
    }
}

// Add the input delta rotation to the scene component's current local rotation
void UQuaternionInput::AddLocalRotationQuat(USceneComponent* SceneComponent, const FQuat& Delta_Rotation)
{
    if (SceneComponent)
    {
        SceneComponent->AddLocalRotation(Delta_Rotation);
    }
}

// Set the Actor's world rotation to the input quaternion
void UQuaternionInput::SetActorWorldRotationQuat(AActor* Actor, const FQuat& Desired_Rotation)
{
    if (Actor)
    {
        Actor->SetActorRotation(Desired_Rotation);
    }
}

// Set the Actor's relative rotation to the input quaternion
void UQuaternionInput::SetActorRelativeRotationQuat(AActor* Actor, const FQuat& Desired_Rotation)
{
    if (Actor)
    {
        Actor->SetActorRelativeRotation(Desired_Rotation);
    }
}

// Add the input delta rotation to the Actor's current local rotation
void UQuaternionInput::AddActorLocalRotationQuat(AActor* Actor, const FQuat& Delta_Rotation)
{
    if (Actor)
    {
        Actor->AddActorLocalRotation(Delta_Rotation);
    }
}

void UQuaternionInput::ClampRotation()
{
    //FRotator BirdRotation = GetOwner()->GetActorRotation();

    //float Pitch = FMath::Clamp(BirdRotation.Pitch, -90, 90);
    //float Roll = FMath::Clamp(BirdRotation.Roll, -90, 90);

    //BirdRotation = FRotator(BirdRotation.Pitch, BirdRotation.Yaw, Roll);

    //GetOwner()->SetActorRotation(BirdRotation);

    FQuat BirdRotationQuat = GetOwner()->GetActorQuat();
    FRotator BirdRotation = BirdRotationQuat.Rotator();

    float ClampedPitch = FMath::Clamp(BirdRotation.Pitch, -90.0f, 90.0f);
    float ClampedRoll = FMath::Clamp(BirdRotation.Roll, -90.0f, 90.0f);

    FRotator ClampedRotator(BirdRotation.Pitch, BirdRotation.Yaw, ClampedRoll);
    FQuat ClampedQuat = FQuat(ClampedRotator);

    GetOwner()->SetActorRotation(ClampedQuat);
}
