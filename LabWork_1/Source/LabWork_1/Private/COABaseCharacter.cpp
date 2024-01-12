// Fill out your copyright notice in the Description page of Project Settings.


#include "COABaseCharacter.h"

// Sets default values
ACOABaseCharacter::ACOABaseCharacter():
	bDeath(false),
	MaxHealth(0.0f),
	HealingRate(10.0f),
	WalkingSpeed(300.0f),
	Health(100.0f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACOABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
	Health = MaxHealth;
}

void ACOABaseCharacter::OnConstruction(const FTransform& Transform)
{
	Update = false;
}

// Called every frame
void ACOABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bDeath)
	{
		Health = FMath::Min(MaxHealth, Health + HealingRate * DeltaTime);
	}
	
}

void ACOABaseCharacter::SetHealth(float NewHealth)
{
	Health = NewHealth;
}

float ACOABaseCharacter::GetHealth()
{
	return Health;
}

float ACOABaseCharacter::ModifyDamage_Implementation(float IncomingDamage)
{
	return IncomingDamage;
}


float ACOABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bDeath) return 0.0f;
	DamageAmount = ModifyDamage(DamageAmount);
	Health -= DamageAmount;
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, FString::Printf(TEXT("Health: %f"), Health));
	if (Health < 0.0f)
	{
		bDeath = true;
		auto Player = Cast<APlayerController>(GetController());
		if (Player != nullptr) DisableInput(Player);
		OnPlayerDied();
	}
	return DamageAmount;
}

