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
	
}

void ADoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoorBase::OverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(),0))
	{
		CheckDir();
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

