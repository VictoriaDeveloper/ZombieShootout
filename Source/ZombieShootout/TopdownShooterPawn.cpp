// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/Controller.h"
#include "Components/SceneComponent.h"
#include "PaperSpriteComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Engine/Engine.h"
#include "TopdownShooterGameMode.h"
#include "TopdownShooterPawn.h"

// Sets default values
ATopdownShooterPawn::ATopdownShooterPawn()
{
 	// Enable tick
	PrimaryActorTick.bCanEverTick = true;

	// Hold & create the capsule component
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));

	// Set the root component as the capsule component
	SetRootComponent(CapsuleComponent);

	// Hold & create the flipbook component
	PaperFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("PaperFlipbookComponent"));

	// Attach the flipbook to the root component
	PaperFlipbookComponent->SetupAttachment(RootComponent);

	// Hold & create the scene component for the gun parent
	GunParent = CreateDefaultSubobject<USceneComponent>(TEXT("GunParent"));

	// Attach the gun parent to the root component
	GunParent->SetupAttachment(RootComponent);

	// Hold & create the gun sprite for the gun
	GunSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("GunSprite"));

	// Attach the gun sprite to the gun parent
	GunSprite->SetupAttachment(GunParent);

	// Hold & create the scene component for the bullet spawn location
	BulletSpawnPosition = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawnPosition"));

	// Attach the bullet spawn to the gun sprite
	BulletSpawnPosition->SetupAttachment(GunSprite);
}

void ATopdownShooterPawn::OverlapBegin(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{

	// Hold the enemy character
	AEnemy* EnemyCharacter = Cast<AEnemy>(OtherActor);

	// Check if the enemy characater is not null & is alive
	if(EnemyCharacter && EnemyCharacter->IsAlive)
	{

		// Check if the player is alive
		if(IsAlive)
		{

			// Set IsAlive to false
			IsAlive = false;

			// Set CanMove to false
			CanMove = false;

			// Set CanShoot to false
			CanShoot = false;

			// Play the death sound
			UGameplayStatics::PlaySound2D(GetWorld(), DieSound);

			// Broadcast the player died delegate
			PlayerDiedDelegate.Broadcast();

			// Hold the game mode
			AGameModeBase *GameMode = UGameplayStatics::GetGameMode(GetWorld());
	
			// Cast the game mode to ATopdownShooterGameMode
			ATopdownShooterGameMode* CorrectGameMode = Cast<ATopdownShooterGameMode>(GameMode);

			// Restart the game
			CorrectGameMode->RestartGame();
		}
	}
}

// Called when the game starts or when spawned
void ATopdownShooterPawn::BeginPlay()
{
	Super::BeginPlay();
	
	// Call the parent class' BeginPlay() function
	Super::BeginPlay();
	
	// Hold the characters APlayerController
	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	// Set the controller to show the mouse cursor
	PlayerController->SetShowMouseCursor(true);

	// Check if the APlayerController is not null
	if(PlayerController != nullptr)
	{

		// Hold the APlayerController's UEnhancedInputLocalPlayerSubsystem
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	
		// Check if the UEnhancedInputLocalPlayerSubsystem is not nullif(Subsystem != nullptr)
		{

			// Add the UInputMapping context to the APlayerController's 
			// UEnhacnedInputLocalPlayerSubsystem
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ATopdownShooterPawn::OverlapBegin);
}

// Called every frame
void ATopdownShooterPawn::Tick(float DeltaTime)
{


	// Call the parent class' Tick() function
	Super::Tick(DeltaTime);

	// Check if the player can move
	if(CanMove)
	{

		// Check if there is movement 
		if(MovementDirection.Length() > 0.0f)
		{
			
			// Check if the movement is diagonal
			if(MovementDirection.Length() > 1.0f)
			{

				// Normalize the movement direction
				MovementDirection.Normalize();
			}

			// Hold the distance to move
			FVector2D DistanceToMove = MovementDirection * MovementSpeed * DeltaTime;

			// Hold the current location of the player
			FVector CurrentLocation = GetActorLocation();

			// Set the new horizontal location for the player
			FVector NewLocation = CurrentLocation + FVector(DistanceToMove.X, 0.0f, 0.0f);

			// Check if the new location is in the horizontal map bounds
			if(!IsInMapBoundsHorizontal(NewLocation.X))
			{

				// Set the new location with the movement subtracted
				NewLocation -=  FVector(DistanceToMove.X, 0.0f, 0.0f);
			}

			// Set the new vertical location for the player
			NewLocation += FVector(0.0f, 0.0f, DistanceToMove.Y);

			// Check if the new location is in the vertical map bounds
			if(!IsInMapBoundsVertical(NewLocation.Z))
			{

				// Set the new location with the movement subtracted
				NewLocation -= FVector(0.0f, 0.0f, DistanceToMove.Y);
			}

			// Set the new location for the player
			SetActorLocation(NewLocation); 
		}
	}

	// Hold the characters APlayerController
	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	// Check if the APlayerController is not null
	if(PlayerController != nullptr)
	{

		// Hold the mouse world location
		FVector MouseWorldLocation;

		// Hold the mouse world direction
		FVector MouseWorldDirection;

		// Deproject the mouse position and get the mouse world location and 
		// mouse world direction
		PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);

		// Hold the current location of the player
		FVector CurrentLocation = GetActorLocation();

		// Hold the current start location
		FVector Start = FVector(CurrentLocation.X, 0.0f, CurrentLocation.Z);

		// Hold the target location
		FVector Target = FVector(MouseWorldLocation.X, 0.0f, MouseWorldLocation.Z); 

		// Hold the rotator needed to get to the target location
		FRotator GunParentRotator = UKismetMathLibrary::FindLookAtRotation(Start, Target);

		// Set the relative rotation using the gun parent rotator
		GunParent->SetRelativeRotation(GunParentRotator);
	}

}

bool ATopdownShooterPawn::IsInMapBoundsHorizontal(float XPosition)
{

    // Hold the result boolean
    bool Result = true;

	// Set the result
	Result = XPosition > HorizontalLimits.X && XPosition < HorizontalLimits.Y;

	// Return the result
	return Result;
}

bool ATopdownShooterPawn::IsInMapBoundsVertical(float ZPosition)
{

	// Hold the result boolean
    bool Result = true;

	// Set the result
	Result = ZPosition > VerticalLimits.X && ZPosition < VerticalLimits.Y;

	// Return the result
	return Result;
}

// Called to bind functionality to input
void ATopdownShooterPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Call the parent class' SetupPlayerInputComponent
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Hold the UEnhancedInputComponent to bind the input actions
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	// Check if the UEnchancedInputComponent is not nullptr
	if(EnhancedInputComponent != nullptr)
	{

		// Bind the MoveAction & MoveTriggered to the triggered trigger event
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopdownShooterPawn::MoveTriggered);
		
		// Bind the MoveAction & MovementCompleted to the completed trigger event
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ATopdownShooterPawn::MoveCompleted);
		
		// Bind the MoveAction & MovementCompleted to the canceled trigger event
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &ATopdownShooterPawn::MoveCompleted);
		
		// Bind the ShootAction & Shoot to the started trigger event
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ATopdownShooterPawn::Shoot);
		
		// Bind the ShootAction & Shoot to the triggered trigger event
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ATopdownShooterPawn::Shoot);
	}

}

void ATopdownShooterPawn::MoveTriggered(const FInputActionValue &Value)
{
	// Hold the move action value from the UInputAction
	FVector2D MoveActionValue = Value.Get<FVector2D>();

	// Check if the player can move
	if(CanMove)
	{

		// Set the movement direction using the move action value
		MovementDirection = MoveActionValue;

		// Set the character's flipbook to be the run flipbook
		PaperFlipbookComponent->SetFlipbook(RunFlipbook);

		// Hold the flipbook's scale 
		FVector FlipbookScale = PaperFlipbookComponent->GetComponentScale();

		// Check if the movement direction is left
		if(MovementDirection.X < 0.0f)
		{

			// Check if the character is facing right
			if(FlipbookScale.X > 0.0f)
			{

				// Set the new scale of the flipbook to change the direction
				PaperFlipbookComponent->SetWorldScale3D(FVector(-1.0f, 1.0f, 1.0f));
			}
		}

		// Else check if the movement direction is right
		else if(MovementDirection.X > 0.0f)
		{

			// Check if the character is facing left
			if(FlipbookScale.X < 0.0f)
			{

				// Set the new scale of the flipbook to change the direction
				PaperFlipbookComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
			}
		}
	}
}

void ATopdownShooterPawn::MoveCompleted(const FInputActionValue &Value)
{

	// Zero out the movement direction
	MovementDirection = FVector2D(0.0f, 0.0f);

	// Check if the player is alive
	if(IsAlive)
	{

		// Set the character's flipbook to be the idle flipbook
		PaperFlipbookComponent->SetFlipbook(IdleFlipbook);
	}
}

void ATopdownShooterPawn::Shoot(const FInputActionValue &Value)
{

	// Check if the player can shooter
	if(CanShoot)
	{

		// Set CanShoot to false
		CanShoot = false;

		// Spawn the bullet 
		ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletActorToSpawn, BulletSpawnPosition->GetComponentLocation(), 
		FRotator(0.0f, 0.0f, 0.0f));

		// Check if the bullet has spawned
		check(Bullet);

		// Hold the characters APlayerController
		APlayerController* PlayerController = Cast<APlayerController>(Controller);

		// Check if there is player controller
		check(PlayerController);

		// Hold the mouse world location
		FVector MouseWorldLocation;

		// Hold the mouse world direction
		FVector MouseWorldDirection;

		// Deproject the mouse position and get the mouse world location and 
		// mouse world direction
		PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);

		// Hold the current location of the character
		FVector CurrentLocation = GetActorLocation();

		// Holdthe bullet direction
		FVector2D BulletDirection = FVector2D(MouseWorldLocation.X - CurrentLocation.X, MouseWorldLocation.Z-CurrentLocation.Z);

		// Normalize the direction
		BulletDirection.Normalize();

		// Hold the bullet speed
		float BulletSpeed = 300.0f;

		// Launch the bullet
		Bullet->Launch(BulletDirection, BulletSpeed);

		// Set a timer for the bullet spawn cooldown
		GetWorldTimerManager().SetTimer(ShootCooldownTimer, this, &ATopdownShooterPawn::OnShootCooldownTimerTimeout, 1.0f, false, ShootCooldownDurationInSeconds);

		// Play the bullet shoot sound
		UGameplayStatics::PlaySound2D(GetWorld(), BulletShootSound);
	}
}

void ATopdownShooterPawn::OnShootCooldownTimerTimeout()
{

	// Check if the player is alive
	if(IsAlive)
	{
		
		// Set CanShoot to true
		CanShoot = true;
	}
}
