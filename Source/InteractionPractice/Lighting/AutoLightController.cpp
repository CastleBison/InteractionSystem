
#include "AutoLightController.h"
#include "Components/PointLightComponent.h"


AAutoLightController::AAutoLightController()
{
	PrimaryActorTick.bCanEverTick = true;

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	SetRootComponent(PointLight);

	PointLight->AttenuationRadius = 500.f;
	PointLight->CastShadows =  true;
}

void AAutoLightController::BeginPlay()
{
	Super::BeginPlay();

	int32 CurrentHour = Hour;

	float Brightness = GetBrightnessByTime(CurrentHour);
	PointLight->SetIntensity(Brightness);
}

void AAutoLightController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AAutoLightController::GetBrightnessByTime(int32 InHour)
{
	float Brightness = 0.f;

	if (InHour >= 0 && InHour < 6)
	{
		Brightness = 200.f;
	}
	else if (InHour >= 6 && InHour < 12)
	{
		Brightness = 1500.f;
	}
	else if (InHour >= 12 && InHour < 18)
	{
		Brightness = 5000.f;
	}
	else
	{
		Brightness = 800.f;
	}

	// 현재시간과 밝기값 콘솔에 출력
	UE_LOG(LogTemp, Warning, TEXT("현재 시간: %d시 → 조명 밝기 %.0f 루멘 적용"), InHour, Brightness);

	return Brightness;
}

