
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
	OverlapBox->SetupAttachment(ElevatorBody);
	OverlapBox->SetBoxExtent(FVector(100.f, 100.f, 250.f));
	OverlapBox->SetRelativeLocation(FVector(0.f, 0.f, 3.f));
	
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapEvent);
	OverlapBox->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlapEvent);

	// 층별 위치값 배열 
	FloorLocationArray.Add(0); // 1층
	FloorLocationArray.Add(260); // 2층
	FloorLocationArray.Add(520); // 3층
	FloorLocationArray.Add(780); // 4층
	FloorLocationArray.Add(1040); // 5층
}

void AElevator::BeginPlay()
{
	Super::BeginPlay();

	// 엘레베이터 시작 위치 저장
	StartLocation = GetActorLocation();

	/*// 배열에 있는 0번째 인덱스가 있으면
	if (FloorLocationArray.IsValidIndex(0))
	{
		// TargetFloorIndex 가 0임
		// TargetFloorIndex = 0; // 이게 바뀌여야함.
		// 움직일 Z값 = 초기 Z위치 + 배열에 있는 TargetFloorIndex값.
		TargetFloorZ = StartLocation.Z + FloorLocationArray[TargetFloorIndex];
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("없는 층입니다."))
	}*/
}

void AElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 실제로 Tick에서 함수 동작
	MoveElevator(DeltaTime);
}

void AElevator::OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,	const FHitResult & SweepResult)
{
	// OtherActor가 있고, 그 OtherActor가 Player라는 태그를 가지고 있으면?
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		// bIsActive가 참이면 리턴 
		if (bIsActive)
		{
			return;
		}

		// TargetFloorIndex가 배열의 범위를 벗어나면
		if (!FloorLocationArray.IsValidIndex(TargetFloorIndex))
		{
			// 잘못된 층 로그 출력
			UE_LOG(LogTemp, Warning, TEXT("잘못된 층: %d"), TargetFloorIndex);
			return;
		}

		// 목표로할 값 = 처음 Z위치 + 배열에서 TargetFloorIndex에 대한하는 인덱스에서 값 얻기 
		TargetFloorZ = StartLocation.Z + FloorLocationArray[TargetFloorIndex];

		
		UE_LOG(LogTemp, Warning, TEXT("FloorLocationArray: %f"), FloorLocationArray[TargetFloorIndex]);
		UE_LOG(LogTemp, Warning, TEXT("TargetFloorIndex: %d"), TargetFloorIndex);
		
		// 활성상태
		bIsActive = true;

		
		// 로그 출력 (목표로 할 값)
		UE_LOG(LogTemp, Warning, TEXT("목표 층: %f"), TargetFloorZ);
	}
}

void AElevator::OnEndOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		UE_LOG(LogTemp, Warning, TEXT("OverlapEnd"));
		if (!FloorLocationArray.IsValidIndex(0))
		{
			return;
		}
		
		TargetFloorZ = StartLocation.Z + FloorLocationArray[0];
		bIsActive = true;

		UE_LOG(LogTemp, Warning, TEXT("1층으로 복귀: %f"),TargetFloorZ);
	}
}

void AElevator::MoveElevator(float DeltaTime)
{
	if (!bIsActive)
	{
		return;
	}

	FVector CurrentLocation = GetActorLocation();
	
	UE_LOG(LogTemp, Warning, TEXT("NextFloor: %f"), CurrentLocation.Z);

	UE_LOG(LogTemp, Warning, TEXT("TargetFloorZ: %f"), TargetFloorZ);
	
	float NextFloorZ = FMath::FInterpTo(CurrentLocation.Z, TargetFloorZ, DeltaTime, MoveSpeed);

	CurrentLocation.Z = NextFloorZ;
	SetActorLocation(CurrentLocation);

	// A값, B값 같아지는 오차? > 0.000001 >>>>1.f 
	if (FMath::IsNearlyEqual(NextFloorZ, TargetFloorZ,1.f))
	{

		bIsActive = false;
	}

	
}


