// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_PlayerHandFSM.h"

// Sets default values for this component's properties
UKMK_PlayerHandFSM::UKMK_PlayerHandFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UKMK_PlayerHandFSM::BeginPlay()
{
	Super::BeginPlay();
	// 플레이어 객체 들고오기
	// player = Cast<AKMK_Player>(GetWorld()->GetFirstPlayerController()->GetPawn());
	// movementComp = player->GetCharacterMovement();
}


// Called every frame
void UKMK_PlayerHandFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// 스테이트 변경
	switch (PState)
	{
		case PlayerHandFSM::Normal:
			NormalHand();
		break;
		case PlayerHandFSM::JumpPack:
			JumpHand();
		break;
		case PlayerHandFSM::GunPack:
			GunHand();
		break;
		case PlayerHandFSM::Energy:
			EnergyHand();
		break;
	}
}

#pragma region Normal
void UKMK_PlayerHandFSM::NormalHand()
{

}
#pragma endregion
#pragma region Jump
void UKMK_PlayerHandFSM::JumpHand()
{

}
#pragma endregion

#pragma region Gun
void UKMK_PlayerHandFSM::GunHand()
{

}
#pragma endregion

#pragma region Energy
void UKMK_PlayerHandFSM::EnergyHand()
{

}
#pragma endregion
