
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AutoCurtainController.generated.h"

UCLASS()
class INTERACTIONPRACTICE_API AAutoCurtainController : public AActor
{
	GENERATED_BODY()

public:
	AAutoCurtainController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent>	CurtainPole;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent>	CurtainLeft;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent>	CurtainRight;

protected:
	UFUNCTION(BlueprintCallable)
	float GetCurtainOpenRatio(float Celsius, int WindSpeed); // 반환값 float
};
