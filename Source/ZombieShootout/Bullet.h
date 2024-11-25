// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "PaperSpriteComponent.h"
#include "Bullet.generated.h"

UCLASS()
class ZOMBIESHOOTOUT_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	
	/**
     * @brief Constructs new ABullet by creating the UActorComponents
	 * & setting a movement direction
     * @param none
     * @return ABullet
     */
	ABullet();

	/**
	 * @brief Launch the bullet in the direction with the given speed
	 * @param FVector2D Direction the direction to shoot the bullet in
	 * float Speed the speed in which the bullet is going
	 * @return void
	 */
	void Launch(FVector2D Direction, float Speed);

	/**
	 * @brief Destroys the bullet 
	 * @param none
	 * @return void
	 */
	void OnDeleteTimerTimeout();

		/**
	 * @brief Handle the bullet overlapping with the enemy
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

	/**
	 * @brief Disable the bullet 
	 * @param none
	 * @return void
	 */
	void DisableBullet();
	
protected:
	
	/**
	 * @brief Configure BeginPlay to bind the overlap function to the sphere component
	 * @param none
	 * @return void
	 */
	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	// Hold the USphereComponent for collision detection
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* SphereComponent;

	// Hold the UPaperSpriteComponent to render the bullet sprite
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* BulletSprite;

	// Hold the movement direction of the bullet
	UPROPERTY(BlueprintReadWrite)
	FVector2D MovementDirection;

		// Hold the movement speed of the bullet
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 300.0f;
	
	// Hold if the bullet is lanched
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsLaunched = false;

	// Hold if the bullet is disabled
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsDisabled = false;

	// Hold a timer to delete the buller
	FTimerHandle DeleteTimer;
};
