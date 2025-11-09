#pragma once

#include "GameFramework/Actor.h"
#include "DoorBase.generated.h"

class UArrowComponent;
class UBoxComponent;

UCLASS()
class INTERACTIONPRACTICE_API ADoorBase : public AActor
{
	GENERATED_BODY()

public:
	ADoorBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	// 문 프레임
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,  meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent>	Frame;

	// 문 짝
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent>	Panel;

	// 화살표
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,  meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent>			Arrow;

	// 콜리전박스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent>		OverlapBox;

	// 방향 (정면/후면)
	bool bCorrectDirection;

	// 문이 현재 움직이는 중인지
	bool bIsMoving = false;

	// 문이 열린 상태인지
	bool bIsOpen = false;

	// 닫힌 생태의 기준 Yaw
	float CloseYaw = 0.f;

	// 목표 Yaw
	float TargetYaw = 0.f;

	// 문이 열릴 각도
	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;

	// 열리는 속도
	UPROPERTY(EditAnywhere)
	float RotateSpeed = 3.f;

	// 문닫기 타이머
	FTimerHandle CloseTimerHandle;

	UFUNCTION()
	void OverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
					UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
protected:
	void CheckDir();

	// 문 여닫기 함수
	void OpenDoor();
	void CloseDoor();

	// 타이머 함수
	void CloseDoorByTimer();

public:
	bool GetCorrectDirection() const
	{
		return bCorrectDirection;
	}
	
	void SetCorrectDirection(bool Dir)
	{
		bCorrectDirection = Dir;
	}
};
