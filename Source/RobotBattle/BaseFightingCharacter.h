// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <functional>

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/PlayerCameraManager.h"
#include "HealthManagerComponent.h"
#include "Katana.h"
#include "Engine/SkeletalMeshSocket.h"
#include "BaseFightingCharacter.generated.h"



UCLASS()
class ROBOTBATTLE_API ABaseFightingCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseFightingCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintPure, Category="Health")
	float GetHealthPoints();
	

	UFUNCTION()
	void SetIsAttacking(bool attacking);

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	UHealthManagerComponent* healthManager;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	// The Katana weapon to spawn.
	TSubclassOf <class AKatana> KatanaClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool IsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool AttackAnimActive = false;

	std::function<void(ABaseFightingCharacter*)> DeadCallback;

	UFUNCTION()
	void DestroyCharacter();

	UFUNCTION(BlueprintCallable)
	bool GetCanAttack();

	UFUNCTION(BlueprintCallable)
	void ResetAttack();

	UFUNCTION(BlueprintCallable)
	void StartAttackAnim();

	UFUNCTION()
	// Deal damage and return true if owner had died.
	virtual bool ApplyDamage(float damage);

	virtual void DamageDealt(float Damage, bool EnemyKilled);

protected:
	UFUNCTION()
	void Die();
	
	UPROPERTY(VisibleAnywhere)
	const USkeletalMeshSocket* rHandSocket;
	
	UPROPERTY(VisibleAnywhere)
	AKatana* katana;

	bool ShouldTriggerDeath;
};
