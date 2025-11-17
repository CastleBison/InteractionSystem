
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

	// 엘리베이터가 움직이는 중인지 여부
	bool bIsActive = false;

	// 시작 위치
	FVector StartLocation;

	// 층 배열
	UPROPERTY(EditAnywhere)
	TArray<float>	FloorLocationArray; // 배열 타입과 넣는 값 타입 일치시키기

	// 실제로 이동해야 할 Z값
	float TargetFloorZ = 0;

	// 목표 층 인덱스
	UPROPERTY(EditAnywhere)
	int32 TargetFloorIndex = 0;

	// 엘레베이터 이동 속도
	float MoveSpeed = 2;
	
protected:
	UFUNCTION()
	void OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,	bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	// 층 이동 함수
	void MoveElevator(float DeltaTime);
};
