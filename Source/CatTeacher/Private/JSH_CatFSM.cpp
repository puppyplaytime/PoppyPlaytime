// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_CatFSM.h"

#include "JSH_Cat.h"
#include "KMK_Player.h"
#include "Kismet/GameplayStatics.h"
#include "JSH_Target.h"

// Sets default values for this component's properties
UJSH_CatFSM::UJSH_CatFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UJSH_CatFSM::BeginPlay()
{
	Super::BeginPlay();
	
	// 월드에서 Player 찾아오기
	//auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AJSH_Cat::StaticClass());
	//layer = Cast<AJSH_Cat>(actor);
	
	// 소유 객체 가져오기
	me = Cast<AJSH_Cat>(GetOwner());

	cState = ECatState::Move01;
}


// Called every frame
void UJSH_CatFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (cState)
	{
	case ECatState::Move01:
		Move01State();
		break;
		
	case ECatState::Move02:
		Move02State();
		break;

	case ECatState::Straight:
		StraightState();
		break;

	case ECatState::Attack:
		AttackState();
		break;

	case ECatState::Discovery:
		DiscoveryState();
		break;

	case ECatState::Damage:
		DamageState();
		break;
		
	case ECatState::Die:
		DieState();
		break;
	}

	

	FString logMsg = UEnum::GetValueAsString(cState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, logMsg);
}

void UJSH_CatFSM::Move01State()
{
	static bool bMovingToTarget01 = true; // target01으로 이동 중인지 여부를 나타내는 플래그

	FVector destination;
	if (bMovingToTarget01)
	{
		destination = target01->GetActorLocation();
	}
	else
	{
		destination = target02->GetActorLocation();
	}

	FVector dir = destination - me->GetActorLocation();
	me->AddMovementInput(dir.GetSafeNormal());

	float distance = FVector::Dist(me->GetActorLocation(), destination);
	if (distance < rr)
	{
		// 목표 지점 도착, 이동 목표를 전환
		bMovingToTarget01 = !bMovingToTarget01;
	}
	// ---------------------------------------------
	currentTime += GetWorld()->DeltaTimeSeconds;
	
	if (currentTime >= stTime)
	{
		// 태그 목록
		TArray<FName> Tags = { FName("FCat1"), FName("FCat2"), FName("FCat3"), FName("FCat4")};

		// 무작위로 태그 선택
		int32 RandomIndex = FMath::RandRange(0, Tags.Num() - 1);
		FName RandomTag = Tags[RandomIndex];

		// 선택된 태그를 가진 모든 액터 찾기
		TArray<AActor*> TaggedCats;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), RandomTag, TaggedCats);
		currentTime = 0;

		// me가 선택된 태그를 가지고 있는지 확인
		bool bIsTaggedCat = false;
		for (AActor* Actor : TaggedCats)
		{
			if (Actor == me)
			{
				bIsTaggedCat = true;
				break;
			}
		}

		if (bIsTaggedCat)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s is moving to Straight - State with tag %s"), *me->GetName(), *RandomTag.ToString());
			cState = ECatState::Straight;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s does not have the %s tag or target03 is null."), *me->GetName(), *RandomTag.ToString());
		}
	}
}

void UJSH_CatFSM::Move02State()
{
}

void UJSH_CatFSM::StraightState()
{
	UE_LOG(LogTemp, Warning, TEXT("Straight - State"));
	FVector destination1 = target03->GetActorLocation();
	FVector dir1 = destination1 - me->GetActorLocation();
	me->AddMovementInput(dir1.GetSafeNormal());
	
	if (dir1.Size() < rr)
	{
		// 상태 전환
                                               
		return;
	}

	// 특정 위치 도달 -> AttackState()  ___ Game Over
	// (환영) 앞으로가다가 , 신호탄 맞음 -> DiscoveryState()  ___ 천천히 사라짐
	// (진짜)  앞으로가다가 player가 기믹 실행 ->  DieState() ___ destroy 후 본래 왕복 위치 spawn 
}

void UJSH_CatFSM::DiscoveryState()
{
}

void UJSH_CatFSM::AttackState()
{
}

void UJSH_CatFSM::DamageState()
{
}

void UJSH_CatFSM::DieState()
{
}

void UJSH_CatFSM::OnDamgaeProcess()
{
}

