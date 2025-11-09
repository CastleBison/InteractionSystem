#include "DoorBase.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ADoorBase::ADoorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Frame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame"));
	SetRootComponent(Frame);
	
	Panel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Panel"));
	Panel->SetupAttachment(Frame);
	Panel->SetRelativeLocation(FVector(-55.f, 0.f, 3.f));
	
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Frame);
	Arrow->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	Arrow->SetRelativeLocation(FVector(0.f, 0.f, 130.f));
	
	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	OverlapBox->SetupAttachment(Frame);
	OverlapBox->SetRelativeScale3D(FVector(2.f, 5.f, 4.f));
	OverlapBox->SetRelativeLocation(FVector(0.f, 0.f, 130.f));
	
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OverlapEvent);
	OverlapBox->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OverlapEnd);

	ConstructorHelpers::FObjectFinder<UStaticMesh> DoorFrame(TEXT("/Script/Engine.StaticMesh'/Game/Mesh/Door/Door_frame.Door_frame'"));
	if (DoorFrame.Succeeded())
	{
		Frame->SetStaticMesh(DoorFrame.Object);
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> DoorPanel(TEXT("/Script/Engine.StaticMesh'/Game/Mesh/Door/Door_panel.Door_panel'"));
	if (DoorPanel.Succeeded())
	{
		Panel->SetStaticMesh(DoorPanel.Object);
	}
}

void ADoorBase::BeginPlay()
{
	Super::BeginPlay();

	if (!Panel)
	{
		UE_LOG(LogTemp, Warning, TEXT("DoorBase: Panel is nullptr!"));
		return;
	}

	// 시작시 닫힌 Yaw상태 저장
	CloseYaw = Panel->GetRelativeRotation().Yaw;
	TargetYaw = CloseYaw;
	
}

void ADoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Panel) return;

	if (bIsMoving)
	{
		FRotator CurrentRot = Panel->GetRelativeRotation();
		float NewYaw = FMath::FInterpTo(CurrentRot.Yaw, TargetYaw, DeltaTime, RotateSpeed);
		CurrentRot.Yaw = NewYaw;
		Panel->SetRelativeRotation(CurrentRot);

		if (FMath::IsNearlyEqual(NewYaw, TargetYaw, 0.1f))
		{
			CurrentRot.Yaw = TargetYaw;
			Panel->SetRelativeRotation(CurrentRot);
			bIsMoving = false;
		}
	}
}

void ADoorBase::OverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(),0))
	{
		CheckDir();

		if (!bIsOpen)
		{
			OpenDoor();
		}
	}
}

void ADoorBase::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(),0))
	{
		GetWorldTimerManager().ClearTimer(CloseTimerHandle);
		GetWorldTimerManager().SetTimer(CloseTimerHandle, this, &ADoorBase::CloseDoorByTimer, 3.f, false);
	}
}

void ADoorBase::CheckDir()
{
	FVector ArrowForwardVector = Arrow->GetForwardVector();
	FVector ArrowWorldLocation = Arrow->GetComponentLocation();
	FVector CharacterLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(),0)->GetActorLocation();
	FVector BetweenLength = ArrowWorldLocation - CharacterLocation;
	
	FVector NormalizeLength = BetweenLength.GetSafeNormal();
	

	float InnerProduct = ArrowForwardVector.Dot(NormalizeLength);

	if (InnerProduct > 0)
	{
		bCorrectDirection = true;
		UE_LOG(LogTemp, Warning, TEXT("후면"));
	}
	else
	{
		bCorrectDirection = false;
		UE_LOG(LogTemp, Warning, TEXT("정면"));
	}
}

void ADoorBase::OpenDoor()
{
	if (bIsMoving)
	{
		return;
	}

	float Sign = bCorrectDirection ? 1.f : -1.f;

	TargetYaw = CloseYaw + Sign * OpenAngle;

	bIsOpen = true;
	bIsMoving = true;

	GetWorldTimerManager().ClearTimer(CloseTimerHandle);
}

void ADoorBase::CloseDoor()
{
	if (bIsMoving)
	{
		return;
	}

	TargetYaw = CloseYaw;
	bIsOpen = false;
	bIsMoving = true;
}

void ADoorBase::CloseDoorByTimer()
{
	CloseDoor();
}

