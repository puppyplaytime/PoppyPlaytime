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
	
	// 월드에서 ATPSPlayer 타깃 찾아오기
	//auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AKMK_Player::StaticClass());
	//auto actor = UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Target"), AJSH_Target::StaticClass());
	// ATPSPlayer 타입으로 캐스팅
	//target = Cast<AKMK_Player>(actor);
	// 소유 객체 가져오기
	me = Cast<AJSH_Cat>(GetOwner());
	
	// 특정 태그를 가진 모든 액터 찾기
	TArray<AActor*> FoundT2;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("T2"), FoundT2);
	// 찾아낸 액터 중 AJSH_Target 클래스로 캐스팅 가능한 액터를 target01에 할당
	for (AActor* Actor : FoundT2)
	{
		target02 = Cast<AJSH_Target>(Actor);
		if (target02)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found target01 with tag: %s"), *target02->GetName());
			break;  // 원하는 액터를 찾았으므로 루프를 종료합니다.
		}
	}

	TArray<AActor*> FoundT1;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("T1"), FoundT1);
	for (AActor* Actor : FoundT1)
	{
		target01 = Cast<AJSH_Target>(Actor);
		if (target01)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found target01 with tag: %s"), *target01->GetName());
			break;  // 원하는 액터를 찾았으므로 루프를 종료합니다.
		}
	}
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
	FVector destination1 = target01->GetActorLocation();
	FVector dir1 = destination1 - me->GetActorLocation();
	me->AddMovementInput(dir1.GetSafeNormal());

	float distance = FVector::Dist(me->GetActorLocation(), destination1);
	if (dir1.Size() < rr)  // 허용 오차 10.0f, 필요에 따라 조정
	{
		// 상태 전환
		cState = ECatState::Move02;
	}
}

void UJSH_CatFSM::Move02State()
{
	FVector destination2 = target02->GetActorLocation();
	FVector dir2 = destination2 - me->GetActorLocation();
	me->AddMovementInput(dir2.GetSafeNormal());

	float distance = FVector::Dist(me->GetActorLocation(), destination2);
	if (dir2.Size() < rr)  // 허용 오차 10.0f, 필요에 따라 조정
	{
		// 상태 전환
		cState = ECatState::Move01;
	}
}

void UJSH_CatFSM::StraightState()
{
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

