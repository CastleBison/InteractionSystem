
#include "Elevator.h"

#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

AElevator::AElevator()
{
	PrimaryActorTick.bCanEverTick = true;

	ElevatorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElevatorFrame"));
	SetRootComponent(ElevatorFrame);
	ElevatorFrame->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Frame(TEXT("/Script/Engine.StaticMesh'/Game/Mesh/Body.Body'"));
	if (Frame.Succeeded())
	{
		ElevatorFrame->SetStaticMesh(Frame.Object);
	}
	
	ElevatorBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElevatorBody"));
	ElevatorBody->SetupAttachment(ElevatorFrame);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Body(TEXT("/Script/Engine.StaticMesh'/Game/Mesh/ElevatorInner.ElevatorInner'"));
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

	FloorLocationArray.Add(0);
	FloorLocationArray.Add(260);
	FloorLocationArray.Add(520);
	FloorLocationArray.Add(780);
	FloorLocationArray.Add(1040);

	// 위젯 경로 지정
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/Widget/WBP_FloorUI.WBP_FloorUI_C"));

	if (WidgetClassFinder.Succeeded())
	{
		ElevatorWidgetClass = WidgetClassFinder.Class;
	}
}

void AElevator::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();

	if (FloorLocationArray.IsValidIndex(TargetFloorIndex))
	{
		TargetFloorZ = StartLocation.Z + FloorLocationArray[TargetFloorIndex];
		UE_LOG(LogTemp, Warning, TEXT("BeginPlay 초기 목표 층 Z: %f"), TargetFloorZ);
	}
}

void AElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveElevator(DeltaTime);
}

void AElevator::OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,	const FHitResult & SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		bIsInPlayer = true;

		ShowElevatorWidget();

		if (!FloorLocationArray.IsValidIndex(TargetFloorIndex))
		{
			UE_LOG(LogTemp, Warning, TEXT("잘못된 층: %d"), TargetFloorIndex);
			return;
		}

		TargetFloorZ = StartLocation.Z + FloorLocationArray[TargetFloorIndex];

		if (!bIsActive)
		{
			bIsActive = true;
			UE_LOG(LogTemp, Warning, TEXT("목표 층: %f"), TargetFloorZ);
		}
	}
}

void AElevator::OnEndOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		bIsInPlayer = false;
		UE_LOG(LogTemp, Warning, TEXT("엘레베이터 이탈"));

		HideElevatorWidget();
		
		if (!FloorLocationArray.IsValidIndex(0))
		{
			return;
		}
		
		TargetFloorZ = StartLocation.Z;
		bIsActive = true;
	}
}

void AElevator::MoveElevator(float DeltaTime)
{
	if (!bIsActive)
	{
		return;
	}

	FVector CurrentLocation = GetActorLocation();
	
	float NextFloorZ = FMath::FInterpTo(CurrentLocation.Z, TargetFloorZ, DeltaTime, MoveSpeed);

	CurrentLocation.Z = NextFloorZ;
	SetActorLocation(CurrentLocation);

	
	if (FMath::IsNearlyEqual(NextFloorZ, TargetFloorZ,1.f))
	{
		bIsActive = false;
	}
}

bool AElevator::GetIsActive()
{
	return bIsActive;
}

void AElevator::SetIsActive(const bool NewActive)
{
	bIsActive = NewActive;
}

int32 AElevator::GetTargetFloorIndex()
{
	return TargetFloorIndex;
}


void AElevator::SetTargetFloorIndex(int32 NewTargetFloorIndex)
{
	TargetFloorIndex = NewTargetFloorIndex;
	TargetFloorZ = StartLocation.Z + FloorLocationArray[TargetFloorIndex];

	SetIsActive(true);
}

bool AElevator::GetIsInPlayer()
{
	return bIsInPlayer;
}

void AElevator::SetIsInPlayer(bool NewIsInPlayer)
{
	bIsInPlayer = NewIsInPlayer;
}

void AElevator::ShowElevatorWidget()
{
	// 이미 켜져 있거나, 클래스가 없으면 바로 종료
	if (bWidgetVisible || !ElevatorWidgetClass)
	{
		return;
	}

	// 플레이어 컨트롤러 가져오기
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		// 위젯 인스턴스가 없으면 생성
		if (!ElevatorWidgetInstance)
		{
			ElevatorWidgetInstance = CreateWidget<UUserWidget>(PC, ElevatorWidgetClass);
		}

		// 생성이 잘 됐으면 화면에 추가
		if (ElevatorWidgetInstance)
		{
			ElevatorWidgetInstance->AddToViewport();

			// 마우스 커서 보이게 + 게임+UI 입력 모드
			PC->bShowMouseCursor = true;

			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PC->SetInputMode(InputMode);

			bWidgetVisible = true;
		}
	}
}

void AElevator::HideElevatorWidget()
{
	if (!bWidgetVisible)
	{
		return;
	}

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (ElevatorWidgetInstance)
		{
			ElevatorWidgetInstance->RemoveFromParent();
		}

		PC->bShowMouseCursor = false;

		FInputModeGameOnly GameOnly;
		PC->SetInputMode(GameOnly);
	}

	bWidgetVisible = false;
}



