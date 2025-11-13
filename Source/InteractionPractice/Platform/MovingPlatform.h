
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

// 전방 선언
class UBoxComponent;

UCLASS()
class INTERACTIONPRACTICE_API AMovingPlatform : public AActor
{
	GENERATED_BODY()

public:
	AMovingPlatform();

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

private:
	// 발판 메시
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent>	PlatformPanel;

	// 박스 콜리전
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent>			CollisionBox;

	// 시작 위치
	FVector StartLocation;

	// 목표 위치
	FVector TargetLocation;

	// 이동 속도
	UPROPERTY(EditAnywhere)
	float MoveSpeed;

	// 이동 오프셋
	UPROPERTY(EditAnywhere)
	float MoveOffset;

	UPROPERTY()
	TObjectPtr<ACharacter>			PlayerCharacter;

	// 감지 플래그
	bool bIsActive = false;

	// 오버랩 시작
	UFUNCTION()
	void OnDetectBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 오버랩 종료
	UFUNCTION()
	void OnDetectEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
