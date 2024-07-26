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
	UE_LOG(LogTemp, Warning, TEXT("Move01 - State"));
	FVector destination1 = target01->GetActorLocation();
	FVector dir1 = destination1 - me->GetActorLocation();
	me->AddMovementInput(dir1.GetSafeNormal());

	float distance = FVector::Dist(me->GetActorLocation(), destination1);
	if (dir1.Size() < rr)
	{
		// 상태 전환
		UE_LOG(LogTemp, Warning, TEXT("Move01 to Move02 - State"));
		cState = ECatState::Move02;
	}
}

void UJSH_CatFSM::Move02State()
{
	UE_LOG(LogTemp, Warning, TEXT("Move02 - State"));
	FVector destination2 = target02->GetActorLocation();
	FVector dir2 = destination2 - me->GetActorLocation();
	me->AddMovementInput(dir2.GetSafeNormal());

	float distance = FVector::Dist(me->GetActorLocation(), destination2);
	if (dir2.Size() < rr)
	{
		// 상태 전환
		UE_LOG(LogTemp, Warning, TEXT("Move01 to Move02 - State"));
		cState = ECatState::Move01;
		return;
	}

	currentTime += GetWorld()->DeltaTimeSeconds;
	
	if (currentTime >= stTime)
	{
		// 태그 목록
		TArray<FName> Tags = { FName("FCat1"), FName("FCat2")};

		// 무작위로 태그 선택
		int32 RandomIndex = FMath::RandRange(0, Tags.Num() - 1);
		FName RandomTag = Tags[RandomIndex];

		// 선택된 태그를 가진 모든 액터 찾기
		TArray<AActor*> TaggedCats;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), RandomTag, TaggedCats);

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
			UE_LOG(LogTemp, Warning, TEXT("Straight - State"));
			cState = ECatState::Straight;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("me does not have the %s tag or target03 is null."), *RandomTag.ToString());
		}
	}
	
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
		cState = ECatState::Move01;
		UE_LOG(LogTemp, Warning, TEXT("Straight to Move01 - State"));
	}
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

