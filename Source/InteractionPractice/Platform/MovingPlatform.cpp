
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
	

	// Overlap Begin, End 이벤트 추가 -> 이런건 대부분 생성자에서 해줍니다.
	// 객체가 만들어질 때 한 번만 설정하면 되기 때문입니다.
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

	if (!PlayerCharacter)
	{
		PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);	
	}
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 굳이 필요 없습니다.
	/*if (!CollisionBox)
	{
		return;
	}*/

	// 플레이어가 바뀌는 로직이 없는 이상은 BeginPlay 에서 1번만 해주는 것이 좋습니다.
	// ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0);

	FVector Target  = FVector();
	FVector CurrentLocation = GetActorLocation();

	{
		// 삼항 연산자
		//Target = bIsActive ? StartLocation : TargetLocation;
	

		// if-else 문
		if (bIsActive)
		{
			Target = TargetLocation;
		}
		else
		{
			Target = StartLocation;
		}
	}
	
	
	// 해당 로직은 오버랩이벤트와 겹침 
	/*if (PlayerCharacter && CollisionBox->IsOverlappingActor(PlayerCharacter))
	{
		Target = TargetLocation;
	}*/

	FVector NewLocation = FMath::VInterpTo(CurrentLocation, Target, DeltaTime, MoveSpeed);

	SetActorLocation(NewLocation);
}

void AMovingPlatform::OnDetectBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// other actor가 없거나 자기 자신이면 리턴
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	// "Player"라는 태그를 가진 액터인지 검사
	if (OtherActor->ActorHasTag("Player"))
	{
		bIsActive = true;
		UE_LOG(LogTemp, Warning, TEXT("플레이어 진입"));
	}

	// 또는 위에서 PlayerCharcter를 받아왔다면
	/*if (OtherActor == PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("플레이어 진입"));
	}*/
}

void AMovingPlatform::OnDetectEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// other actor가 없거나 자기 자신이면 리턴
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	// "Player"라는 태그를 가진 액터인지 검사
	if (OtherActor->ActorHasTag("Player"))
	{
		bIsActive = false;
		UE_LOG(LogTemp, Warning, TEXT("플레이어 이탈"));
	}

	// 또는 위에서 PlayerCharcter를 받아왔다면
	/*if (OtherActor == PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("플레이어 이탈"));
	}*/
}

