// Fill out your copyright notice in the Description page of Project Settings.

#include "Bridge.h"
#include "BallProject/BallProjectBall.h"

// Sets default values
ABridge::ABridge(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{ 
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = _rootComponent;

	_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	_collision->SetWorldScale3D(FVector(1.2f, 4.f, 1.f));
	_collision->SetRelativeLocation(FVector(0.f, 0.f, 0.5f));
	_collision->SetHiddenInGame(false);
	_collision->SetupAttachment(_rootComponent);

	_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BridgeMesh"));
	_mesh->SetupAttachment(_rootComponent);
}

// Called when the game starts or when spawned
void ABridge::BeginPlay()
{
	_collision->OnComponentBeginOverlap.AddDynamic(this, &ABridge::OnBridgeCollisionBeginOverlap);
	_collision->OnComponentEndOverlap.AddDynamic(this, &ABridge::OnBridgeCollisionEndOverlap);
	_mesh->SetMaterial(0, _material);

	Super::BeginPlay();
}

// Called every frame
void ABridge::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABridge::OnBridgeCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	FString actorName = *OtherActor->GetName();
	FString debugMessage = "Overlap started with: " + actorName;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, debugMessage);
}

void ABridge::OnBridgeCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	FString actorName = *OtherActor->GetName();
	FString debugMessage = "Overlap ended with: " + actorName;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, debugMessage);

	if (actorName.Equals("RollingBall")) {
		if (boostSpeed) {
			ApplySpeedBoostToPlayer(OtherActor);
		}
		if (boostJump) {
			ApplyJumpBoostToPlayer(OtherActor);
		}

		//_collision->OnComponentBeginOverlap.RemoveDynamic(this, &ABridge::OnBridgeCollisionBeginOverlap);
	}
}

void ABridge::ApplySpeedBoostToPlayer(AActor* player) {
	ABallProjectBall* BallActor = dynamic_cast<ABallProjectBall*>(player);
	BallActor->BoostSpeed(50000000.0f);
}

void ABridge::ApplyJumpBoostToPlayer(AActor* player) {
	ABallProjectBall* BallActor = dynamic_cast<ABallProjectBall*>(player);
	BallActor->BoostNextJump();
}