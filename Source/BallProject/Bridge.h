// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "Components/BoxComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bridge.generated.h"

UCLASS()
class BALLPROJECT_API ABridge : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABridge(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBridgeCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBridgeCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
	bool boostSpeed;
	UPROPERTY(EditAnywhere)
	bool boostJump;

	UFUNCTION(BluePrintCallable)
	void ApplySpeedBoostToPlayer(AActor* player);
	UFUNCTION(BluePrintCallable)
	void ApplyJumpBoostToPlayer(AActor* player);

private:
	UPROPERTY(EditAnywhere)
	USceneComponent* _rootComponent;
	UPROPERTY(EditAnywhere, Category="Collision")
	UBoxComponent* _collision;
	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* _mesh;
	UPROPERTY(EditAnywhere, Category = "Material")
	UMaterialInterface* _material;
};
