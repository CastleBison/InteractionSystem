
#include "MovingPlatform.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	MoveSpeed = 2.f;

	// 발판 세팅
	PlatformPanel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformPanel"));

	PlatformPanel->SetMobility(EComponentMobility::Movable);
	
	SetRootComponent(PlatformPanel);
	PlatformPanel->SetRelativeRotation(FRotator(0.f, -180.f, 0.f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> Panel(TEXT("/Script/Engine.StaticMesh'/Game/Mesh/PlatformPanel.PlatformPanel'"));

	if (Panel.Succeeded())
	{
		PlatformPanel->SetStaticMesh(Panel.Object);
	}

	// 콜리전 박스
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(PlatformPanel);

	CollisionBox->SetBoxExtent(FVector(32.f, 35.f, 30.f));
	CollisionBox->SetRelativeLocation(FVector(10.f, 0.f, 10.f));
	//CollisionBox->SetRelativeScale3D(FVector(2.5f, 3.0f, 0.1f));
	
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();

	float MoveOffset = 300.f;
	TargetLocation = StartLocation + FVector(0.f, 0.f, MoveOffset);

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMovingPlatform::OnDetectBeginOverlap);
		CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AMovingPlatform::OnDetectEndOverlap);
	}
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CollisionBox)
	{
		return;
	}

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0);

	FVector CurrentLocation = GetActorLocation();
	FVector Target = StartLocation;

	if (Player && CollisionBox->IsOverlappingActor(Player))
	{
		Target = TargetLocation;
	}

	FVector NewLocation = FMath::VInterpTo(CurrentLocation, Target, DeltaTime, MoveSpeed);

	SetActorLocation(NewLocation);
}

void AMovingPlatform::OnDetectBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	if (Cast<ACharacter>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("진입"));
	}
}

void AMovingPlatform::OnDetectEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	if (Cast<ACharacter>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("이탈"));
	}
}

