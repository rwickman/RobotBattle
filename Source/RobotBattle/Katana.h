// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <functional>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Katana.generated.h"

UCLASS()
class ROBOTBATTLE_API AKatana : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKatana();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
		void OnOverlapBegin(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
	void SetIsOwnerAttacking(bool attacking);

	UFUNCTION()
	bool CanAttack();

	UFUNCTION()
	void ResetAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float damagePoints = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DPS = 0.43f;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* katanaMesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* boxCollision;

	UPROPERTY()
	float TimeSinceAttack = 0.0f;

	std::function<void(float, bool)> DamageDealtCallback; 

protected:
	bool IsOwnerAttacking = false;
	TSet<uint32> HurtEnemies;
	
};
