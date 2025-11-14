
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Elevator.generated.h"

UCLASS()
class INTERACTIONPRACTICE_API AElevator : public AActor
{
	GENERATED_BODY()

public:
	AElevator();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
