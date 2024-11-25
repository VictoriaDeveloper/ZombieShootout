// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "EnemySpawner.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemySpawner::StartSpawning()
{
	
	// Set the timer to spawn an enemy
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AEnemySpawner::OnSpawnTimerTimeout, SpawnTime, true, SpawnTime);
}

void AEnemySpawner::OnSpawnTimerTimeout()
{

	// Spawn an enemy
	SpawnEnemy();
}

void AEnemySpawner::SpawnEnemy()
{

	// Hold the actor location to spawn the enemy
	FVector EnemyLocation = GetActorLocation() ;

	// Randomize Z component between -20 and 50
	EnemyLocation.Z = FMath::RandRange(-20.0f, 50.0f);

	// Randomize the sign of X component
	EnemyLocation.X *= (FMath::RandBool() ? 1 : -1); // Flip the sign randomly

	// Spawn the AEnemy actor
	AEnemy* EnemySpawned = GetWorld()->SpawnActor<AEnemy>(EnemyActorToSpawn, EnemyLocation, 
	FRotator::ZeroRotator);

	// Setup the enemy spawned
	SetupEnemy(EnemySpawned);

	// Increment the total enemy count by 1
	TotalEnemyCount++;

	// Check if we reached a difficulty spike interval
	if(TotalEnemyCount % DifficultySpikeInterval == 0)
	{

		// Check if we can decrease the spawn time
		if(SpawnTime > SpawnTimeMinimumLimit)
		{

			// Decrease the spawn time
			SpawnTime -= DecreaseSpawnTimerByEveryInterval;
			
			// Check if the spawn time went below the minimum spawn time
			if(SpawnTime < SpawnTimeMinimumLimit)
			{

				// Set the new spawn time
				SpawnTime = SpawnTimeMinimumLimit;
			}

			// Stop to reset the spawning
			StopSpawning();

			// Reset the the spawning
			StartSpawning();
		}
	}
}

void AEnemySpawner::SetupEnemy(AEnemy *EnemyCharacter)
{

	// Check if there is an enemy character
	if (EnemyCharacter)
	{

		// Set the player character for the enemy
		EnemyCharacter->PlayerCharacter = PlayerCharacter;

		// Allow the enemy to follow
		EnemyCharacter->CanFollow = true;

		// Add this function the enemy died delgate to be fired when it broadcasts
		//EnemyCharacter->EnemyDiedDelegate.AddDynamic(this, &AEnemySpawner::OnEnemyDied);
	}
}

void AEnemySpawner::StopSpawning()
{
	
	// Clear the timer 
	GetWorldTimerManager().ClearTimer(SpawnTimer);
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{

	// Call the parent class' BeginPlay()
	Super::BeginPlay();
	
	// Hold the game mode
	AGameModeBase *GameMode = UGameplayStatics::GetGameMode(GetWorld());
	
	// Check if the game mode is not null
	if (GameMode != nullptr)
	{

		// Cast the game mode to ATopdownShooterGameMode
		MyGameMode = Cast<ATopdownShooterGameMode>(GameMode);

		// Check if the game mode was casted correctly
		check(MyGameMode);
	}


	// Get the player actor
	AActor* PlayerActor = UGameplayStatics::GetActorOfClass(GetWorld(), ATopdownShooterPawn::StaticClass());
	if (PlayerActor)
	{

		// Cast the actor to ATopdownShooterPawn
		PlayerCharacter = Cast<ATopdownShooterPawn>(PlayerActor);

		// Bind the delegate to the player when they die
		PlayerCharacter->PlayerDiedDelegate.AddDynamic(this, &AEnemySpawner::OnPlayerDied);
	}
	
	// Start spawning
	StartSpawning();
}

void AEnemySpawner::OnEnemyDied()
{

	 GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Add score?"));
	// Add the score to the game mode
	MyGameMode->AddScore();
}

void AEnemySpawner::OnPlayerDied()
{
		// Stop enemies from spawning
	StopSpawning();

	// Hold all the enemies
	TArray<AActor*> EnemyArray;

	// Set all the all actors of the Enemy class
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), EnemyArray);

	// Loop through the Enemy actors
	for (AActor *EnemyActor : EnemyArray)
	{

		// Hold the enemy 
		AEnemy* EnemyCharacter = Cast<AEnemy>(EnemyActor);
		
		// Check if the enemy is not nullptr and is alive
		if (EnemyCharacter && EnemyCharacter->IsAlive)
		{

			// Set CanFollow to false
			EnemyCharacter->CanFollow = false;
		}
	}


	// Restart the game
	MyGameMode->RestartGame();
}
