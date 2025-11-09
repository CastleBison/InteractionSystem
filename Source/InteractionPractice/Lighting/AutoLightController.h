
#pragma once

#include "GameFramework/Actor.h"
#include "AutoLightController.generated.h"

// 전방 선언 (헤더에 포함하는것을 최소한으로 하기 위해 사용)
class UPointLightComponent;

UCLASS()
class INTERACTIONPRACTICE_API AAutoLightController : public AActor
{
	GENERATED_BODY()


public:
	AAutoLightController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	// 변수 선언
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPointLightComponent>	PointLight;

	// 에디터에서 직접 시간을 수정할 수 있도록 설정
	UPROPERTY(EditAnywhere, Category = "Light Control", meta = (ClampMin = "0", ClampMax = "23", AllowPrivateAccess = "true"))
	int32 Hour = 12;

	// 블루프린트 호출 가능 설정
	UFUNCTION(BlueprintCallable)
	float GetBrightnessByTime(int32 InHour);

	
};
