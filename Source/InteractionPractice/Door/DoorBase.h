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
	// Sets default values for this actor's properties
	ADoorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// 문 프레임
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,  meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent>	Frame;

	// 문 짝
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent>	Panel;

	// 화살표
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,  meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent>			Arrow;

	// 콜리전박스
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly , meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent>		OverlapBox;

	// 방향 (정면/후면)
	bool bCorrectDirection;

	UFUNCTION()
	void OverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	void CheckDir();

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
