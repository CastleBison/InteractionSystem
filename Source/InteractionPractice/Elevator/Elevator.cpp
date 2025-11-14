
#include "Elevator.h"


AElevator::AElevator()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AElevator::BeginPlay()
{
	Super::BeginPlay();
	
}

void AElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

