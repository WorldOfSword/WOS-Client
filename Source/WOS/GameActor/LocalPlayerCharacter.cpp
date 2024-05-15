// Fill out your copyright notice in the Description page of Project Settings.


#include "GameActor/LocalPlayerCharacter.h"	
#include "GameFramework/PlayerController.h"
#include "PaperFlipbookComponent.h"
#include "Engine/LocalPlayer.h"
#include "Managers/Manager.h"
#include "Managers/Network.h"

void ALocalPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALocalPlayerCharacter::MoveHandler);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ALocalPlayerCharacter::MoveHandler);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ALocalPlayerCharacter::JumpHandler);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ALocalPlayerCharacter::AttackHandler);

	auto ControllerPtr = Cast<APlayerController>(GetController());
	if (ControllerPtr != nullptr) {
		if (auto SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(ControllerPtr->GetLocalPlayer())) {
			SubSystem->AddMappingContext(InputMappingContext, 0);
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("LocalPlayerCharacter Can't Found PlayerController"));
	}

	PrimaryActorTick.bCanEverTick = true;
}

void ALocalPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	SpriteOriginScale = GetSprite()->GetComponentScale();

	auto testRpcId = BIND_RPC(ALocalPlayerCharacter, TestRPC);
	testRpcId->Call<ALocalPlayerCharacter>(RpcTarget::All);
}

void ALocalPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float time = GetWorld()->GetTimeSeconds();

	auto p = GetActorLocation();
	SetActorLocation(p + FVector(DeltaTime * LastMoveInput * MoveSpeed / 0.2f, 0, 0));

	if (time > LastSendPositionTime + sendPositionInterval) {
		LastSendPositionTime = time;

		SendMovePacket(LastMoveInput, 0);
	}
}

void ALocalPlayerCharacter::MoveHandler(const FInputActionValue& Value) {

	float Axis = Value.Get<float>();
	if (Axis != LastMoveInput) {
		LastMoveInput = Axis;

		LastSendPositionTime = GetWorld()->GetTimeSeconds();

		if (LastMoveInput != 0) {
			GetSprite()->SetWorldScale3D(FVector(SpriteOriginScale.X * LastMoveInput, SpriteOriginScale.Y, SpriteOriginScale.Z));
			GetSprite()->SetFlipbook(WalkAnimation);
		}
		else {
			GetSprite()->SetFlipbook(IdleAnimation);
		}

		SendMovePacket(Axis, 0);
	}
}

void ALocalPlayerCharacter::TestRPC()
{
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, TEXT("RPC"));
}

void ALocalPlayerCharacter::SendMovePacket(float X, float Y) {
	gen::mmo::Move MovePacket;
	MovePacket.dir.x = X;
	MovePacket.dir.y = Y;
	UManager::Net()->Send(ServerType::MMO, &MovePacket);
}

void ALocalPlayerCharacter::JumpHandler() {

}

void ALocalPlayerCharacter::AttackHandler() {

}