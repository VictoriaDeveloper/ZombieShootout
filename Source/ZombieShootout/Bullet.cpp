// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SphereComponent.h"
#include "PaperSpriteComponent.h"
#include "Enemy.h"
#include "Bullet.h"

ABullet::ABullet()
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Hold & create the sphere component
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

	// Set the sphere component as the root component
	SetRootComponent(SphereComponent);

	// Hold & create the bullet sprite
	BulletSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BulletSprite"));

	// Attach the bullet sprite to the root component
	BulletSprite->SetupAttachment(RootComponent);

	// Set the default movement direction
	MovementDirection = FVector2D(1.0f, 0.0f);
}

void ABullet::Launch(FVector2D Direction, float Speed)
{

	// Check if the bullet is launched
	if(IsLaunched) return;
	
	// Set IsLaunched to true
	IsLaunched = true;

	// Set the movement direction
	MovementDirection = Direction;

	// Set the movement speed
	MovementSpeed = Speed;

	// Hold the initial time delay for the timer
	float DeleteTime = 10.0f;
	
	// Set a timer for the delete bullet method
	GetWorldTimerManager().SetTimer(DeleteTimer, this, &ABullet::OnDeleteTimerTimeout, DeleteTime, false);
}

void ABullet::OnDeleteTimerTimeout()
{

	// Destroy the bullet
	Destroy();
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{

	// Call the parent class' BeginPlay()
	Super::BeginPlay();
	
	// Bind the OnOverlapBegin function to the sphere component's 
	// OnComponentBeginOverlap event
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OverlapBegin);
}

void ABullet::OverlapBegin(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	// Hold the enemy character
	AEnemy* EnemyCharacter = Cast<AEnemy>(OtherActor);

	// Check if there an enemy character and it is alive
	if(EnemyCharacter && EnemyCharacter->IsAlive)
	{

		// Disable the bullet
		DisableBullet();

		// Kill the enemy
		EnemyCharacter->Die();
	}
}

void ABullet::DisableBullet()
{
	
	// Return if the bullet is already disabled
	if(IsDisabled) return;

	// Set IsDisabled to true
	IsDisabled = true;

	// Remove the collision from the sphere component
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Destroy the sprite component
	BulletSprite->DestroyComponent();
}

void ABullet::Tick(float DeltaTime)
{
	
	// Call the parent class' Tick()
	Super::Tick(DeltaTime);

	// Check if bullet has been launched
	if(IsLaunched)
	{
		// Hold the distance to move
		FVector2D DistanceToMove = MovementDirection * MovementSpeed * DeltaTime;

		// Hold the current location
		FVector CurrentLocation = GetActorLocation();

		// Hold the new location
		FVector NewLocation = CurrentLocation + FVector(DistanceToMove.X, 0.0f, DistanceToMove.Y); 

		// Set the new actor location
		SetActorLocation(NewLocation);
	}
}

