
#include "AutoCurtainController.h"


AAutoCurtainController::AAutoCurtainController()
{
	PrimaryActorTick.bCanEverTick = true;

	CurtainPole = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CurtainPole"));
	SetRootComponent(CurtainPole);
	CurtainPole->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	CurtainLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CurtainLeft"));
	CurtainLeft->SetupAttachment(CurtainPole);
	CurtainLeft->SetRelativeLocation(FVector(-120.f, 0.f, 0.f));

	CurtainRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CurtainRight"));
	CurtainRight->SetupAttachment(CurtainPole);
	CurtainRight->SetRelativeLocation(FVector(120.f, 0.f, 0.f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> Pole(TEXT("/Script/Engine.StaticMesh'/Game/Fab/Retro_Patterned_Curtain/retro_patterned_curtain/StaticMeshes/CurtainPole.CurtainPole'"));
	if (Pole.Succeeded())
	{
		CurtainPole->SetStaticMesh(Pole.Object);
	}
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> LeftFabric(TEXT("/Script/Engine.StaticMesh'/Game/Fab/Retro_Patterned_Curtain/retro_patterned_curtain/StaticMeshes/Curtain.Curtain'"));
	if (LeftFabric.Succeeded())
	{
		CurtainLeft->SetStaticMesh(LeftFabric.Object);
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> RightFabric(TEXT("/Script/Engine.StaticMesh'/Game/Fab/Retro_Patterned_Curtain/retro_patterned_curtain/StaticMeshes/Curtain.Curtain'"));
	if (RightFabric.Succeeded())
	{
		CurtainRight->SetStaticMesh(RightFabric.Object);
	}
	
}

void AAutoCurtainController::BeginPlay()
{
	Super::BeginPlay();

	float Celsius = 10.f;
	int WindSpeed = 1;

	float Ratio = GetCurtainOpenRatio(Celsius, WindSpeed);

	CurtainLeft->SetRelativeRotation(FRotator(0.f, -90.f*Ratio, 0.f ));
	CurtainRight->SetRelativeRotation(FRotator(0.f, 90.f*Ratio, 0.f));
	
	// 로그 찍기
	UE_LOG(LogTemp, Warning, TEXT("현재 온도: %.1f°C, 풍속: %d m/s → 개방 비율 %.1f 적용"), Celsius, WindSpeed, Ratio);
}

void AAutoCurtainController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AAutoCurtainController::GetCurtainOpenRatio(float Celsius, int WindSpeed)
{
	// 온도 25°C 이상 또는 풍속 10m/s 이상
	if (Celsius >= 25.f || WindSpeed >= 10)
	{
		return 0.2f;
	}
	// 온도 0~15°C, 풍속 0~5m/s
	if ((Celsius >= 0.f && Celsius <= 15.f) && (WindSpeed >= 0 && WindSpeed <= 5))
	{
		return 1.f;
	}
	// 그 외
	return 0.5f;
}

