// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "Engine/TimerHandle.h"
#include "TopdownShooterPawn.h"
#include "TopdownShooterGameMode.h"
#include "EnemySpawner.generated.h"

UCLASS()
class ZOMBIESHOOTOUT_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	
	/**
	 * @brief Default constructor
	 * @param none
	 * @return AEnemySpawner
	 */
	AEnemySpawner();

	/**
	 * @brief Handle the start of spawning
	 * @param none
	 * @return void
	 */
	void StartSpawning();

	/**
	 * @brief Handle the spawn timer timing out
	 * @param none
	 * @return void
	 */
	void OnSpawnTimerTimeout();

	/**
	 * @brief  Spawn an enemy actor
	 * @param none
	 * @return void
	 */
	void SpawnEnemy();

	/**
	 * @brief Setup the enemy knowing the player location
	 * @param AEnemy* Enemy
	 * @return void
	 */
	void SetupEnemy(AEnemy* EnemyCharacter);

	/**
	 * @brief Stop spawning enemies
	 * @param none
	 * @return void
	 */
	void StopSpawning();

protected:
	
	/**
	 * @brief Handle setting the player character and game mode for level progression
	 * @param none
	 * @return void
	 */
	virtual void BeginPlay() override;

		/**
	 * @brief Handles the enemy broadcasting it's death
	 * @param none
	 * @return void
	 */
	UFUNCTION()
	void OnEnemyDied();

	/**
	 * @brief Handles the player death broadcasted
	 * @param none
	 * @return void
	 */
	UFUNCTION()
	void OnPlayerDied();
public:	
	
	// Hold the enemy actor class to spawn
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemy> EnemyActorToSpawn;

	// Hold the float for the spawn time
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnTime = 1.0f;

	// Hold the float for the spawn distance
	float SpawnDistance= 400.0f;

	// Hold the integer for the total enemies 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int TotalEnemyCount = 0;

	// Hold the integer for the difficulty spike interval
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DifficultySpikeInterval = 10;

	// Hold the float for the minimum spawn time
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnTimeMinimumLimit = 0.5;

	// Hold the float for the time to decrease the interval by
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DecreaseSpawnTimerByEveryInterval = .05;

	// Hold the FTimerHandle for the spawn time
	FTimerHandle SpawnTimer;

	// Hold the ATopdownShooterPawn that is the player's character
	ATopdownShooterPawn* PlayerCharacter;

	// Hold the AGunSurvivorsGameMode for score managing
	ATopdownShooterGameMode* MyGameMode;

};
