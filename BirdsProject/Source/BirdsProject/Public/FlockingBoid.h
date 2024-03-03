#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "FlockingBoid.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BIRDSPROJECT_API UFlockingBoid : public UActorComponent
{
    GENERATED_BODY()

public:
    UFlockingBoid();

    UFUNCTION(BlueprintCallable, Category = "Flocking")
        void StartFlocking(AActor* leader, float rowDistance, float rowSpacing, float verticalSpacing, TArray<AActor*> flockMembers);

    UFUNCTION(BlueprintCallable, Category = "Flocking")
        void Flock(float DeltaTime);

private:
    bool _isFlocking;

    AActor* _leader;
    float _rowDistance;
    float _rowSpacing;
    float _verticalSpacing;
    TArray<AActor*> _flockMembers;
    class USkeletalMeshComponent* _skeletalMeshComponent;
    UFloatingPawnMovement* _movementComponent;
};
