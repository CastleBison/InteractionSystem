
#include "Elevator.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"



AElevator::AElevator()
{
	PrimaryActorTick.bCanEverTick = true;

	ElevatorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElevatorFrame"));
	SetRootComponent(ElevatorFrame);
	ElevatorFrame->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Frame(TEXT("/Script/Engine.StaticMesh'/Game/Mesh/SM_ElevatorBody.SM_ElevatorBody'"));
	if (Frame.Succeeded())
	{
		ElevatorFrame->SetStaticMesh(Frame.Object);
	}
	
	ElevatorBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElevatorBody"));
	ElevatorBody->SetupAttachment(ElevatorFrame);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Body(TEXT("/Script/Engine.StaticMesh'/Game/Mesh/ElecvatorInner.ElecvatorInner'"));
	if (Body.Succeeded())
	{
		ElevatorBody->SetStaticMesh(Body.Object);
	}

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	OverlapBox->SetupAttachment(ElevatorFrame);
	OverlapBox->SetBoxExtent(FVector(60.f, 60.f, 10.f));
	OverlapBox->SetRelativeLocation(FVector(0.f, 0.f, 3.f));
	
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapEvent);
	OverlapBox->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlapEvent);

	// 층별 위치값 배열 
	FloorLocationArray.Add(0.f); // 1층
	FloorLocationArray.Add(260.f); // 2층
	FloorLocationArray.Add(520.f); // 3층
	FloorLocationArray.Add(780.f); // 4층
	FloorLocationArray.Add(1040.f); // 5층
}

void AElevator::BeginPlay()
{
	Super::BeginPlay();

	// 엘레베이터 시작 위치 저장
	StartLocation = GetActorLocation();

	if (FloorLocationArray.IsValidIndex(0))
	{
		TargetFloorIndex = 0;
		TargetFloorZ = StartLocation.Z + FloorLocationArray[TargetFloorIndex];
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("없는 층입니다."))
	}
}

void AElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 실제로 Tick에서 함수 동작
	MoveElevator(DeltaTime);
}

void AElevator::OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,	const FHitResult & SweepResult)
{
	if (OtherActor && OtherActor ->ActorHasTag("Player"))
	{
		if (bIsActive)
		{
			return;
		}

		if (!FloorLocationArray.IsValidIndex(TargetFloorIndex))
		{
			UE_LOG(LogTemp, Warning, TEXT("잘못된 층: %d"), TargetFloorIndex);
			return;
		}

		TargetFloorZ = StartLocation.Z + FloorLocationArray[TargetFloorIndex];
		bIsActive = true;

		UE_LOG(LogTemp, Warning, TEXT("목표 층: %d"), TargetFloorZ);
	}
	
}

void AElevator::OnEndOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		if (!FloorLocationArray.IsValidIndex(0))
		{
			return;
		}

		TargetFloorIndex = 0;
		TargetFloorZ = StartLocation.Z + FloorLocationArray[0];
		bIsActive = true;

		UE_LOG(LogTemp, Warning, TEXT("1층으로 복귀: %d"),TargetFloorZ);
	}
}

void AElevator::MoveElevator(float DeltaTime)
{
	if (!bIsActive)
	{
		return;
	}

	FVector CurrentLocation = GetActorLocation();

	float NextFloor = FMath::FInterpTo(CurrentLocation.Z, TargetFloorZ, DeltaTime, MoveSpeed);

	CurrentLocation.Z = NextFloor;
	SetActorLocation(CurrentLocation);

	if (FMath::IsNearlyEqual(NextFloor, TargetFloorZ, 1.f))
	{
		bIsActive = false;
	}
}


