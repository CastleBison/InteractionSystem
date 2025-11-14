
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

	// 층별 위치값 배열 
	FloorLocationArray.Add(0.f);
	FloorLocationArray.Add(260.f);
	FloorLocationArray.Add(520.f);
	FloorLocationArray.Add(780.f);
	FloorLocationArray.Add(1040.f);
}

void AElevator::BeginPlay()
{
	Super::BeginPlay();
}

void AElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AElevator::OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,	const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		if (!bIsActive)
		{
			// 오버랩이 시작되면, 지정된 목표층(GoalFloor) 인덱스를 이용해 배열에서 해당 층의 위치를 가져온 뒤 그 위치로 엘리베이터를 이동
			bIsActive = true;
			UE_LOG(LogTemp, Warning, TEXT("플레이어 진입")); // 오버랩 시작

			if (GoalFloor > FloorLocationArray.Num()) return; // 예외처리
		
			
			float SelectFloorLocation = FloorLocationArray[GoalFloor]; // 인덱스를 이용해서 배열에 접근 후 출력
			
			UE_LOG(LogTemp,Warning,TEXT("select floor location : %f"), SelectFloorLocation);
		}
	}
}

void AElevator::UpMovement()
{
	
}

