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
	
	// ���忡�� Player ã�ƿ���
	//auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AJSH_Cat::StaticClass());
	//layer = Cast<AJSH_Cat>(actor);
	
	// ���� ��ü ��������
	me = Cast<AJSH_Cat>(GetOwner());
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
	if (dir1.Size() < rr)
	{
		// ���� ��ȯ
		cState = ECatState::Move02;
	}
}

void UJSH_CatFSM::Move02State()
{
	FVector destination2 = target02->GetActorLocation();
	FVector dir2 = destination2 - me->GetActorLocation();
	me->AddMovementInput(dir2.GetSafeNormal());

	float distance = FVector::Dist(me->GetActorLocation(), destination2);
	if (dir2.Size() < rr)
	{
		// ���� ��ȯ
		cState = ECatState::Move01;
	}
}

void UJSH_CatFSM::StraightState()
{
	// Ư�� �±׸� ���� ��� ���� ã��
	TArray<AActor*> Fcat1;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("FCat1"), Fcat1);

	// me�� FCat1 �±׸� ������ �ִ��� Ȯ��
	bool bIsTaggedCat = false;
	for (AActor* Actor : Fcat1)
	{
		if (Actor == me)
		{
			bIsTaggedCat = true;
			break;
		}
	}

	if (bIsTaggedCat)
	{
		// me�� FCat1 �±׸� ���� ��쿡�� �Ʒ� �ڵ带 ����
		FVector destination1 = target03->GetActorLocation();
		FVector dir1 = destination1 - me->GetActorLocation();
		me->AddMovementInput(dir1.GetSafeNormal());
		if (dir1.Size() < rr)
		{
			// ���� ��ȯ
			cState = ECatState::Move01;
		}
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("me does not have the FCat1 tag or target03 is null."));
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

