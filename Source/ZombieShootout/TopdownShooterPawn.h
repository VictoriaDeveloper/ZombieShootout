// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
#include "Bullet.h"
#include "Sound/SoundBase.h"
#include "TopdownShooterPawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDiedDelegate);

UCLASS()
class ZOMBIESHOOTOUT_API ATopdownShooterPawn : public APawn
{
	GENERATED_BODY()

public:
	
	/**
     * @brief Constructs new ATopdownPawn by creating the UActorComponents
     * @param none
     * @return ATopdownPawn
     */
	ATopdownShooterPawn();

	/**
	 * @brief Handle setting the actor location depending on the movement
	 * input
	 * @param float DeltaTime the frame rate 
	 * @return void
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief Check if the player is in the horizontal bounds of the map
	 * @param float XPostion the x position of the character
	 * @return bool
	 */
	bool IsInMapBoundsHorizontal(float XPosition);

	/**
	 * @brief Check if the player is in the vertical bounds of the map
	 * @param float ZPosition the z position of the character
	 * @return bool
	 */
	bool IsInMapBoundsVertical(float ZPosition);

	/**
	 * @brief Bind the Input Actions to the EnhancedInputComponent
	 * @param UInputComponent* PlayerInputComponent the input component for 
	 * the player
	 * @return void
	 */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		/**
	 * @brief Handle moving the pawn and changing the flipbook source to the
	 * jog animation
	 * @param FInputActionValue& Value a reference to the FInputActionValue for this
	 * InputAction
	 * @return void
	 */
	void MoveTriggered(const FInputActionValue& Value);

	/**
	 * @brief Handle stopping the pawn and changing the flipbook source to the
	 * idle animation
	 * @param FInputActionValue& Value a refernce to the FInputActionValue for this
	 * InputAction
	 * @return void
	 */
	void MoveCompleted(const FInputActionValue& Value);

		/**
	 * @brief Handle shooting the gun 
	 * @param FInputaction& Value a reference to the FInputActionValue for this
	 * InputAction 
	 * @return void
	 */
	void Shoot(const FInputActionValue& Value);

	/**
	 * @brief Allow the player to shoot again after the timer timesout
	 * @param none
	 * @return void
	 */
	void OnShootCooldownTimerTimeout();

	/**
	 * @brief Handle the player overlapping the enemy
	 * @param UPrimitiveComponent* OverlappedComponent the component that overlapped
	 * AActor* OtherActor the actor that was overlapped
	 * UPrimitiveComponent OtherComp the component of the actor that was overlapped
	 * int32 OtherBodyIndex the index of the body in a multi-body component 
	 * boool bFromSweep indicate if the overlap occured during a sweep 
	 * operation
	 * FHitResult& SweepResult the hit information of the overlap 
	 * @return void
	 */
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	
	/**
	 * @brief Configure BeginPlay to add the UInputMappingContext to the 
	 * player controller's EnhancedInputLocalPlayerSubsystem
	 * @param none
	 * @return void
	 */
	virtual void BeginPlay() override;

public:	


	// Hold the UCapsuleComponent for collision
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComponent;

	// Hold the UPaperFlipbookComponent for sprite animations
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* PaperFlipbookComponent;

	// Hold the USceneComponent for the gun sprite parent
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* GunParent;

	// Hold the UPaperSpriteComponent for the gun sprite
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* GunSprite;

	// Hold the USceneComponent for the bullet spawn location on the gun
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* BulletSpawnPosition;

	// Hold the UInputMappingContext for the input actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputMappingContext* InputMappingContext;

	// Hold if the player can move
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanMove = true;
	
	// Hold the movement direction for moving
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D MovementDirection;

	// Hold the movement speed for the character
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 100.0f;

	// Hold the FVector2D that holds the values of the horizontal limits
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D HorizontalLimits;

	// Hold the FVector2D that holds the values of the vertical limits
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D VerticalLimits;

	// Hold the UInputAction for moving
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* MoveAction;

	// Hold the UInputAction for shooting
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* ShootAction;

	// Hold if the player can move
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAlive = true;

	// Hold the UPaperFlipbook for the idle animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* IdleFlipbook;

	// Hold the UPaperFlipbook for the run animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* RunFlipbook;

	// Hold if the player can shoot
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CanShoot = true;

	// Hold the bullet spawn cooldown
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShootCooldownDurationInSeconds = 0.3f;

	// Hold the reference to the bullet class blueprint
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABullet> BulletActorToSpawn;

	// Hold the timer for the shoot cooldown
	FTimerHandle ShootCooldownTimer;

	// Hold the USoundBase for the bullet
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* BulletShootSound;

	// Hold the FPlayerDiedDelgate for the player's death
	FPlayerDiedDelegate PlayerDiedDelegate;

	// Hold the USoundBase for the death 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* DieSound;
};
