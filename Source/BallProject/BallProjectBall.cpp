// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "BallProjectBall.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"

ABallProjectBall::ABallProjectBall()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMesh(TEXT("/Game/Rolling/Meshes/BallMesh.BallMesh"));

	// Create mesh component for the ball
	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball0"));
	Ball->SetStaticMesh(BallMesh.Object);
	Ball->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Ball->SetSimulatePhysics(true);
	Ball->SetAngularDamping(0.1f);
	Ball->SetLinearDamping(0.1f);
	Ball->BodyInstance.MassScale = 3.5f;
	Ball->BodyInstance.MaxAngularVelocity = 800.0f;
	Ball->SetNotifyRigidBodyCollision(true);
	RootComponent = Ball;

	// Create a camera boom attached to the root (ball)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bAbsoluteRotation = true; // Rotation of the ball should not affect rotation of boom
	SpringArm->RelativeRotation = FRotator(-45.f, 0.f, 0.f);
	SpringArm->TargetArmLength = 1200.f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 3.f;

	// Create a camera and attach to boom
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Set up forces
	RollTorque = 50000000.0f;
	JumpImpulse = 350000.0f;
	bCanJump = true; // Start being able to jump
}

//void Update:
void ABallProjectBall::Tick(float DeltaTime)
{
	APawn::Tick(DeltaTime);
	EvaluateBoosts(DeltaTime);
}

void ABallProjectBall::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAxis("MoveRight", this, &ABallProjectBall::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABallProjectBall::MoveForward);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABallProjectBall::Jump);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ABallProjectBall::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ABallProjectBall::TouchStopped);
}

void ABallProjectBall::MoveRight(float Val)
{
	float BoostedRollTorque = RollTorque + speedBoost;
	const FVector Torque = FVector(-1.f * Val * BoostedRollTorque, 0.f, 0.f);
	Ball->AddTorqueInRadians(Torque);
}

void ABallProjectBall::MoveForward(float Val)
{
	float BoostedRollTorque = RollTorque + speedBoost;
	const FVector Torque = FVector(0.f, Val * BoostedRollTorque, 0.f);
	Ball->AddTorqueInRadians(Torque);	
}

void ABallProjectBall::Jump()
{
	if(bCanJump)
	{
		float BoostedJumpImpulse = JumpImpulse;
		if (jumpBoost) {
			jumpBoost = false;

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Stop jumpboost");

			BoostedJumpImpulse *= 2;
		}

		const FVector Impulse = FVector(0.f, 0.f, BoostedJumpImpulse);
		Ball->AddImpulse(Impulse);
		bCanJump = false;
	}
}

void ABallProjectBall::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	bCanJump = true;
}

void ABallProjectBall::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (bCanJump)
	{
		const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
		Ball->AddImpulse(Impulse);
		bCanJump = false;
	}

}

void ABallProjectBall::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (bCanJump)
	{
		const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
		Ball->AddImpulse(Impulse);
		bCanJump = false;
	}
}

//self-made functions:
void ABallProjectBall::EvaluateBoosts(float tickTime) {
	if (remainingSpeedBoostTime > 0.f) {
		remainingSpeedBoostTime -= tickTime;
	}

	if (remainingSpeedBoostTime < 0.f) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Stop speedboost");
		remainingSpeedBoostTime = 0.f;
		speedBoost = 0.f;
	}
}

void ABallProjectBall::BoostSpeed(float speedBoost) {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Start speedboost");
	remainingSpeedBoostTime = 10.f;
	this->speedBoost = speedBoost;
}
void ABallProjectBall::BoostNextJump() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Start jump-boost");
	jumpBoost = true;
}