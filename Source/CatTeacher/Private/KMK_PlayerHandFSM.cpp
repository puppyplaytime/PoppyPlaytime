// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_PlayerHandFSM.h"
#include "KMK_Player.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "KMK_Bullet.h"
#include "KMK_PlayerHand.h"
#include "PlayerAnimInstance.h"

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
	Player = Cast<AKMK_Player>(GetWorld()->GetFirstPlayerController()->GetPawn());
	movementComp = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();
	JumpPower = movementComp->JumpZVelocity;
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
	GEngine->AddOnScreenDebugMessage(4, 1, FColor::Blue, FString::Printf(TEXT("remove")));
}
#pragma endregion
#pragma region Jump
// 일정 트리거가 발견되면 점프력을 2배로
void UKMK_PlayerHandFSM::JumpHand()
{
	if(isJump)
	{
		if(movementComp->JumpZVelocity != JumpPower * 2)
		{
			movementComp->JumpZVelocity = JumpPower * 1.8f;
			Player->Jump();
			isJump = false;
		}
	}
	else
	{
		movementComp->JumpZVelocity = JumpPower;
	}
}
#pragma endregion
#pragma region Gun
void UKMK_PlayerHandFSM::GunHand()
{
	if (isFire && cnt < 1)
	{
		// 총알 효과 재생
		GetWorld()->SpawnActor<AKMK_Bullet>(bulletFact, bulletTrans);
		Player->anim->PlayFireMontage();
		cnt++;
	}
}
#pragma endregion
#pragma region Energy
void UKMK_PlayerHandFSM::EnergyHand()
{
	t += GetWorld()->DeltaTimeSeconds;
    GEngine->AddOnScreenDebugMessage(8, 1, FColor::Blue, FString::Printf(TEXT("charge")));
	if (t > chargeTime)
	{
		isCharge = false;
		PState = PlayerHandFSM::Normal;
		t = 0;
	}
}
#pragma endregion
