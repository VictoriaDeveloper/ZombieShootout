// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/TimerHandle.h"
#include "TopdownShooterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTOUT_API ATopdownShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

		/**
	 * @brief Constructor for the game mode 
	 * @param none
	 * @return AGunSurvivorsGameMode
	 */
	ATopdownShooterGameMode();

public: 

	/**
	 * @brief Add score
	 * @param none
	 * @return void
	 */
	void AddScore();

		/**
	 * @brief Handle restarting the game by starting the timer
	 * @param none
	 * @return void
	 */
	void RestartGame();

	/**
	 * @brief Restart the game once the timer timesout
	 * @param none
	 * @return void
	 */
	void OnRestartGameTimerTimeout();

public:

	// Hold the score
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Score = 0;

	// Hold the float that holds the time before restart
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeBeforeRestart = 0.3f;

	// Hold the restart game timer
	FTimerHandle RestartGameTimer;

protected:

		/**
	 * @brief Handle the start of the game mode by setting the score to 0
	 * @param none
	 * @return void
	 */
	virtual void BeginPlay() override;

};
