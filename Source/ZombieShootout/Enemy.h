// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "TopdownShooterPawn.h"
#include "Sound/SoundBase.h"
#include "Enemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyDiedDelegate);

UCLASS()
class ZOMBIESHOOTOUT_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	
	/**
     * @brief Constructs new AEnemy by creating the UActorComponents
     * @param none
     * @return AEnemy
     */
	AEnemy();

	/**
     * @brief Handles following the player
     * @param none
     * @return void
     */
	virtual void Tick(float DeltaTime) override;

		/**
	 * @brief Handle the death of the enemy character
	 * @param none
	 * @return void
	 */
	void Die();

		/**
	 * @brief Handle the destruction of the enemy when the timer times out 
	 * @param none
	 * @return void
	 */
	void OnDestroyTimerTimeout();
public:	

	// Hold the UCapsuleComponent for collision
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComponent;

	// Hold the UPaperFlipbookComponent for the animations
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* PaperFlipbookComponent;

	// Hold the ATopdownShooterPawn that is the player character
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ATopdownShooterPawn* PlayerCharacter;

	// Hold the bool that checks if the enemy is alive
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsAlive = true;

	// Hold the bool the checks if the enemy can follow the player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CanFollow = false;

	// Hold the movement speed for the enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 50.0f;

	// Hold the stop distance for the enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StopDistance = 20.0f;

	// Hold an FTimerHandle to countdown the destruction of the enemy
	FTimerHandle DestroyTimer;

	// Hold the FEnemyDiedDelegate for the enemy
	FEnemyDiedDelegate EnemyDiedDelegate;

	// Hold the USoundBase die sound
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* DieSound;
};
