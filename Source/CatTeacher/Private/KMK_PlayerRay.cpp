// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_PlayerRay.h"
#include "KMK_PlayerHandFSM.h"
#include "KMK_Player.h"
#include "KMK_PlayerHand.h"
#include "../../../../Plugins/Runtime/CableComponent/Source/CableComponent/Classes/CableComponent.h"
#include "KHH_EnemyFSM.h"
#include "Components/ArrowComponent.h"
#include "KMK_Bat.h"
#include "KMK_Battery.h"

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
	playerComp = Cast<AKMK_Player>(GetOwner());
}


// Called every frame
void UKMK_PlayerRay::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// 레이 셋팅
	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());
	params.AddIgnoredActor(Hands[0]);
	params.AddIgnoredActor(Hands[1]);
	// 레이 쏘는 마지막 부분
	endPos = playerComp->endPos;

	// hit된 물체 정보 들고오기
	FHitResult hitInfo;
	GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
	// 레이를 쏘고
	bool bhit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

	if (hitInfo.GetActor() != nullptr && hitInfo.GetActor()->ActorHasTag("E_Bat"))
	{
		// 벽에 있는 단자에 배터리가 들어있지 않은 경우
		auto* bat = hitInfo.GetActor()->FindComponentByClass<UKMK_Bat>();
		FVector dir = hitInfo.GetActor()->GetTargetLocation() - playerComp->startPos;
		// 일정거리 내부라면, 배터리를 넣을 수 있는 상태가 됨
		// GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Orange, FString::Printf(TEXT("%f, %s"), dir.Length(), *hitInfo.GetActor()->GetActorLabel()));
		if (!bat->isCome)
		{
			// 일정거리 내부라면, 배터리를 넣을 수 있는 상태가 됨
			if (dir.Length() < PDis)
			{
				Hands[0]->isCome = true;
				Hands[1]->isCome = true;
			}
			// 클릭이 되고
			if (isRay)
			{	
				
				// 배터리를 들고 있는 손을 파악해 배터리를 넣어줌
				for (int i = 0; i < 2; i++)
				{
					if (playerComp->isDir[i] && Hands[i]->isGrab)
					{
						playerComp->Hands[i]->isBatCom = true;
						bat->isCome = true;
						Hands[i]->isGrab = false;
						Hands[i]->isCome = false;
						Bats[i]->SetVis(false);
					}
				}
			}
			
		}
		// 배터리가 단자에 들어있는 경우
		else
		{
			
			// 거리를 확인한후, 인터렉션이 가능하게 만듦
			if (playerComp->isDir[0])
			{
				bat->isCome = false;
				Bats[0]->SetVis(true);
				Hands[0]->isCome = false;
			}
			if(playerComp->isDir[1])
			{
				bat->isCome = false;
				Bats[1]->SetVis(true);
				Hands[1]->isCome = false;
			}
		}

	}
	// 입력값이 들어오는 경우
	if (isRay)
	{
		// 레이를 그리고
		DrawDebugLine(GetWorld(), startPos, endPos, FColor::Blue, false, 1.f);
		//  오른손 이라면
		if(playerComp->isDir[0])
		{
			
			// 레이에 물체가 맞은 경우
			if(bhit)
			{
				// 총을 들고 있다면
				if (playerComp->RMeshComp->GetStaticMesh() == Hands[0]->HandMesh[1])
				{
					// 총을 쏜다
					FSM->isFire	= true;
					// FSM을 gunpack으로 변경하고
					FSM->PState = PlayerHandFSM::GunPack;
				}
				// 총을 들고있지 않다면
				else
				{
					// 오른손에 물체의 component를 넘겨주고
					Hands[0]->hitinfo = hitInfo.GetComponent();
					// 손을 뻗게 만든다
					// 손 초기값
					Hands[0]->handPos = 27;
					// 손의 도착지점
					Hands[0]->endPos = hitInfo.ImpactPoint;
					// 손을 뻗게 만듦
					if (Hands[0]->isGrab || Hands[0]->isPick)
					{
						Hands[0]->isGrab = false;
						Hands[0]->isPick = false;
					}
					else Hands[0]->isGo = true;
				}
			}
			// 물체가 맞지 않은 경우
			else
			{
				if (Hands[0]->isGrab)
				{
					Hands[0]->isGrab = false;
					return;
				}

				// 1. 총을 들고 있지 않다면
				if (playerComp->RMeshComp->GetStaticMesh() != Hands[0]->HandMesh[1])
				{
					// 손을 뻗는다
					Hands[0]->handPos = 27;
					Hands[0]->endPos = playerComp->endPos;
					Hands[0]->isGo = true;
				}
				// 아니라면
				else
				{
					// 총쏘기
					FSM->isFire = true;
				}
			}

		}
		// 왼손이라면
		if (playerComp->isDir[1])
		{

			if (Hands[1]->isGrab)
			{
				Hands[1]->isGrab = false;
				return;
			}
			// 물체가 있다면
			if (bhit)
			{
				Hands[1]->endPos = hitInfo.ImpactPoint;
				Hands[1]->hitinfo = hitInfo.GetComponent();
			}
			else Hands[1]->endPos = playerComp->endPos;
			// 공통적으로 수행
			Hands[1]->handPos = -27;
			Hands[1]->isRay = true;
			if (Hands[1]->isGrab || Hands[1]->isPick)
			{
				Hands[1]->isGrab = false;
				Hands[1]->isPick = false;
			}
			else Hands[1]->isGo = true;
		}
	}
}

void UKMK_PlayerRay::SetRayPos(FVector start, FVector end)
{
	startPos = start;
	endPos = end;

	isRay = true;
}
