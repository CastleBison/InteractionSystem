
#pragma once

#include "GameFramework/Actor.h"
#include "Elevator.generated.h"

// 전방선언
class UBoxComponent;

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

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent>	ElevatorFrame;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent>	ElevatorBody;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent>			OverlapBox;

	bool bIsActive = false;

protected:
	UFUNCTION()
	void OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,	bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void UpMovement();

	// 층 배열
	UPROPERTY(EditAnywhere)
	TArray<int32>	FloorLocationArray;

	// 에디터에서 목표층을 설정할 수 있는 변수 선언
	UPROPERTY(EditAnywhere)
	int32 GoalFloor = 0;

	int32 MaxFloor = 100;
};
