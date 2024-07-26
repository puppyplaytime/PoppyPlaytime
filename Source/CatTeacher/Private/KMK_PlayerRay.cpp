// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_PlayerRay.h"
#include "KMK_PlayerHandFSM.h"

// Sets default values for this component's properties
UKMK_PlayerRay::UKMK_PlayerRay()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKMK_PlayerRay::BeginPlay()
{
	Super::BeginPlay();
	FSM = GetOwner()->FindComponentByClass<UKMK_PlayerHandFSM>();
	
}


// Called every frame
void UKMK_PlayerRay::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());
	FHitResult hitInfo;
	// 클릭이 된다면
	if (isRay)
	{
		// 레이를 쏘고
		bool bhit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
		DrawDebugLine(GetWorld(), startPos, endPos, FColor::Blue, false, 1.0f);
		// 물체가 있다면
		if (bhit)
		{
			GEngine->AddOnScreenDebugMessage(1, 1, FColor::Blue, FString::Printf(TEXT("hi")));
			// jump패드를 감지했을때
			if (hitInfo.GetActor()->GetActorLabel().Contains("jump"))
			{
				// movementComp->JumpZVelocity = JumpPower * 2;
			}
			if (hitInfo.GetActor()->GetActorLabel().Contains("ElectricalPanel"))
			{
				FSM->PState = PlayerHandFSM::Energy;
			}
		}
	}

}
// 첫 위치값과 마지막 위치값
// 클릭됐을때, 레이의 위치 설정됨
void UKMK_PlayerRay::SetStartEndPos(FVector start, FVector dir)
{
	startPos = start;
	endPos = start + dir * rayDis;
	
	isRay = true;
}

