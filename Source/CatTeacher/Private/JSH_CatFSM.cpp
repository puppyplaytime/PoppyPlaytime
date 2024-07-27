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

	cState = ECatState::RoundMove;
}


// Called every frame
void UJSH_CatFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (cState)
	{
	case ECatState::RoundMove:
		RoundMoveState();
		break;
		
	case ECatState::MoveWait:
		MoveWaitState();
		break;
		
	case ECatState::TrueMove:
		TrueMoveState();
		break;

	case ECatState::FalseMove:
		FalseMoveState();
		break;

	case ECatState::Ceiling:
		CeilingState();
		break;
		
	case ECatState::Discovery:
		DiscoveryState();
		break;
		
	case ECatState::Attack:
		AttackState();
		break;

	case ECatState::Blocked:
		BlockedState();
		break;
		
	case ECatState::Die:
		DieState();
		break;
	}

	

	FString logMsg = UEnum::GetValueAsString(cState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, logMsg);
}

void UJSH_CatFSM::RoundMoveState()
{
	static bool bMovingToTarget01 = true; // target01���� �̵� ������ ���θ� ��Ÿ���� �÷���

	TArray<AActor*> TaggedActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("FCat1"), TaggedActors);

	// me�� tag1 �±׸� ������ �ִ��� Ȯ��
	bool bIsTagged = false;
	for (AActor* Actor : TaggedActors)
	{
		if (Actor == me)
		{
			bIsTagged = true;
			break;
		}
	}
	if (bIsTagged)
	{
		// me�� tag1 �±׸� ���� ��쿡�� �Ʒ� �ڵ带 ����
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
		if (distance < ReachDistance)
		{
			// ��ǥ ���� ����, �̵� ��ǥ�� ��ȯ
			bMovingToTarget01 = !bMovingToTarget01;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("me does not have the tag1 tag."));
	}
	


	
	// ---------------------------------------------
	currentTime += GetWorld()->DeltaTimeSeconds;
	
	if (currentTime >= stTime)
	{
		// �±� ���
		TArray<FName> Tags = { FName("F")};

		// �������� �±� ����
		int32 RandomIndex = FMath::RandRange(0, Tags.Num() - 1);
		FName RandomTag = Tags[RandomIndex];

		// ���õ� �±׸� ���� ��� ���� ã��
		TArray<AActor*> TaggedCats;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), RandomTag, TaggedCats);
		currentTime = 0;

		// me�� ���õ� �±׸� ������ �ִ��� Ȯ��
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
			cState = ECatState::MoveWait;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s does not have the %s tag or target03 is null."), *me->GetName(), *RandomTag.ToString());
		}
	}
}

void UJSH_CatFSM::MoveWaitState()
{
	FVector destinationM = targetMiddle->GetActorLocation();
	FVector dirM = destinationM - me->GetActorLocation();
	me->AddMovementInput(dirM.GetSafeNormal());
	if (dirM.Size() < ReachDistance)
	{
		// ���� ��ȯ
		cState = ECatState::TrueMove;
	}
	
}


void UJSH_CatFSM::TrueMoveState()
{
	FVector destination = targetPlayer->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();
	me->AddMovementInput(dir.GetSafeNormal());
	// Ư�� ��ġ ���� -> AttackState()  ___ Game Over
	// (ȯ��) �����ΰ��ٰ� , ��ȣź ���� -> DiscoveryState()  ___ õõ�� �����
	// (��¥)  �����ΰ��ٰ� player�� ��� ���� ->  DieState() ___ destroy �� ���� �պ� ��ġ spawn 
}


void UJSH_CatFSM::FalseMoveState()
{

}

void UJSH_CatFSM::CeilingState()
{
}

void UJSH_CatFSM::DiscoveryState()
{
}

void UJSH_CatFSM::AttackState()
{
}

void UJSH_CatFSM::BlockedState()
{
}

void UJSH_CatFSM::DieState()
{
}

void UJSH_CatFSM::OnDamgaeProcess()
{
}

