// Fill out your copyright notice in the Description page of Project Settings.


#include "GameActor/Player/Sword.h"
#include "PlayerCharacter.h"

void USword::Init(TObjectPtr<APlayerCharacter> PlayerCharacter)
{
	Super::Init(PlayerCharacter);
	Energy = MaxEnergy;
}

void USword::OnSwitchedFrom(TObjectPtr<UWeapon> Other)
{
	Super::OnSwitchedFrom(Other);
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Magenta, TEXT("Sword"));
}

void USword::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Energy += DeltaTime * EnergyRegenSpeed;
	if (Energy > MaxEnergy) {
		Energy = MaxEnergy;
	}

}

void USword::LightAttackHandler(int Axis)
{
	Super::LightAttackHandler(Axis);
	if (!IsAfterDelaying())
	{
		SetAfterDelay(LightAttackAfterDelay);

		SendDamage(ScanHitbox(FVector2D(50, 0), FVector2D(150, 100)), LightAttackDamage);
	}
}

void USword::HeavyAttackHandler(int Axis)
{
	Super::HeavyAttackHandler(Axis);
	if (!IsAfterDelaying())
	{
		SetAfterDelay(HeavyAttackAfterDelay);

		SendDamage(ScanHitbox(FVector2D(50, 0), FVector2D(150, 150)), HeavyAttackDamage);
	}
}

void USword::Skill2Handler(int Axis)
{
	Super::Skill2Handler(Axis);
	if (!IsAfterDelaying()) {
		SetAfterDelay(RushAfterDelay);

		ScanHitbox(FVector2D(-100, 0), FVector2D(200 + RushLength * 100, 100));
		Dash(GetLastMoveInput() * RushLength);
		MoveAnimationLogic(GetLastMoveInput());
	}
}

void USword::Skill3Handler(int Axis)
{
	Super::Skill3Handler(Axis);
	if (!IsAfterDelaying()) {
		SetAfterDelay(BackStepAfterDelay);

		Dash(-GetLastMoveInput() * BackStepLength);
		MoveAnimationLogic(-GetLastMoveInput());
	}
}

float USword::GetEnergy()
{
	return Energy;
}
