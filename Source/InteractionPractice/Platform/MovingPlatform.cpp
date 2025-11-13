
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
	

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMovingPlatform::OnDetectBeginOverlap);
		CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AMovingPlatform::OnDetectEndOverlap);
	}
	
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	MoveOffset = 300.f;
	
	TargetLocation = StartLocation + FVector(0.f, 0.f, MoveOffset);
	
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Target  = FVector();
	FVector CurrentLocation = GetActorLocation();

	
	if (bIsActive)
	{
		Target = TargetLocation;
	}
	else
	{
		Target = StartLocation;
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

	if (OtherActor->ActorHasTag("TestPlayer"))
	{
		bIsActive = true;
		UE_LOG(LogTemp, Warning, TEXT("플레이어 진입"));
	}
	// 로그 찍어보기
	UE_LOG(LogTemp, Warning, TEXT("%s") ,*OverlappedComp->GetName());
	UE_LOG(LogTemp, Warning, TEXT("%s") ,*OtherActor->GetName());;
	UE_LOG(LogTemp, Warning, TEXT("%s") ,*OtherComp->GetName());
	UE_LOG(LogTemp, Display, TEXT("%d") ,OtherBodyIndex);
	UE_LOG(LogTemp, Verbose, TEXT("%d") ,bFromSweep);
	UE_LOG(LogTemp, VeryVerbose, TEXT("%d") ,SweepResult.IsValidBlockingHit());
	
	
}

void AMovingPlatform::OnDetectEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	if (OtherActor->ActorHasTag("TestPlayer"))
	{
		bIsActive = false;
		UE_LOG(LogTemp, Warning, TEXT("플레이어 이탈"));
	}
}

