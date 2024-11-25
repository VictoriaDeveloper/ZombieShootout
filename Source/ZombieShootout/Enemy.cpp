// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "TopdownShooterGameMode.h"
#include "Enemy.h"

AEnemy::AEnemy()
{

 	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Set and create the capsule component
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));

	// Set the capsule component as the root component
	SetRootComponent(CapsuleComponent);

	// Set and create the paper flipbook component
	PaperFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("PaperFlipbookComponent"));

	// Attach the paper flipbook component to the root component
	PaperFlipbookComponent->SetupAttachment(RootComponent);
}

void AEnemy::Tick(float DeltaTime)
{

	// Call the parent class's Tick() function
	Super::Tick(DeltaTime);

	//  Check if the enemy is alive, the enemy can follow, & the player is set
	if(IsAlive && CanFollow && PlayerCharacter != nullptr)
	{

		// Hold the current location
		FVector CurrentLocation = GetActorLocation();

		// Hold the target location
		FVector TargetLocation = PlayerCharacter->GetActorLocation();

		// Hold the direction to the player
		FVector DirectionToPlayer = TargetLocation - CurrentLocation;

		// Hold the distance to the player
		float DistanceToPlayer = DirectionToPlayer.Length();

		// Check if the distance is greater than the stop distance
		if(DistanceToPlayer >= StopDistance)
		{

			// Normalize the direction
			DirectionToPlayer.Normalize();

			// Hold the new location
			FVector NewLocation = CurrentLocation + (DirectionToPlayer * MovementSpeed * DeltaTime);
			
			// Set the actor location using the new location
			SetActorLocation(NewLocation);
		}

		// Hold the new current location
		CurrentLocation = GetActorLocation();

		// Hold the flipbook scale
		float FlipbookScale = PaperFlipbookComponent->GetComponentScale().X;

		// Check if the enemy is on the left side of the player
		if((TargetLocation.X - CurrentLocation.X) >= 0.0f)
		{

			// Check if the enemy is facing left
			if(FlipbookScale < 0.0f)
			{

				// Set enemy to face right
				PaperFlipbookComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
			}
		}
		
		// Else the enemy is on the right of the player
		else
		{

			// Check if the enemy is facing right
			if(FlipbookScale > 0.0f)
			{

				// Set the enemy to face left
				PaperFlipbookComponent->SetWorldScale3D(FVector(-1.0f, 1.0f, 1.0f));
			}
		}
	}
}

void AEnemy::Die()
{
	// Check if the enemy is not alive
	if(!IsAlive) return;

	// Set IsAlive to false
	IsAlive = false;

	// Set CanFollow to false
	CanFollow = false;

	// Broadcast the enemy died delgate
	EnemyDiedDelegate.Broadcast();

	// Play the sound
	UGameplayStatics::PlaySound2D(GetWorld(), DieSound);

	// Hold the destroy time
	float DestroyTime = .5f;

	// Hold the game mode
	AGameModeBase *GameMode = UGameplayStatics::GetGameMode(GetWorld());

	// Cast the game mode to ATopdownShooterGameMode
	ATopdownShooterGameMode* CorrectGameMode = Cast<ATopdownShooterGameMode>(GameMode);

	// Restart the game
	CorrectGameMode->AddScore();

	// Set a timer to destroy the actor
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AEnemy::OnDestroyTimerTimeout, 1.0f, false, DestroyTime);
}

void AEnemy::OnDestroyTimerTimeout()
{
	


	// Destroy the actor
	Destroy();
}
