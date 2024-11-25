// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "TopdownShooterGameMode.h"

ATopdownShooterGameMode::ATopdownShooterGameMode()
{
    
      // Allow the actor to tick
    PrimaryActorTick.bCanEverTick = true;
}

void ATopdownShooterGameMode::AddScore()
{

    Score += 10;
}

void ATopdownShooterGameMode::BeginPlay()
{
    // Call the parent class's BeginPlay()
    Super::BeginPlay();

    Score = 0;
}

void ATopdownShooterGameMode::OnRestartGameTimerTimeout()
{

    // Start the timer to restart the level
   GetWorldTimerManager().SetTimer(RestartGameTimer, this, &ATopdownShooterGameMode::OnRestartGameTimerTimeout, 1.0f, false, TimeBeforeRestart);
}

void ATopdownShooterGameMode::RestartGame()
{

    // Open the main level
    UGameplayStatics::OpenLevel(GetWorld(), FName("L_Main"));
}
