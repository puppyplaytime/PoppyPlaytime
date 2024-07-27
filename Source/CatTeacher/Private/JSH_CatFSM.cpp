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
    static bool bMovingToTarget01_FCat1 = true; // FCat1 ��� ���� �̵� �÷���
    static bool bMovingToTarget01_FCat2 = true; // FCat2 ��� ���� �̵� �÷���
    static bool bMovingToTarget01_FCat3 = true; // FCat3 ��� ���� �̵� �÷���

    TArray<AActor*> TaggedActors_FCat1;
    TArray<AActor*> TaggedActors_FCat2;
    TArray<AActor*> TaggedActors_FCat3;

    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("FCat1"), TaggedActors_FCat1);
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("FCat2"), TaggedActors_FCat2);
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("FCat3"), TaggedActors_FCat3);

    // me�� FCat1 �±׸� ������ �ִ��� Ȯ��
    bool bIsTagged_FCat1 = TaggedActors_FCat1.Contains(me);

    // me�� FCat2 �±׸� ������ �ִ��� Ȯ��
    bool bIsTagged_FCat2 = TaggedActors_FCat2.Contains(me);

    // me�� FCat3 �±׸� ������ �ִ��� Ȯ��
    bool bIsTagged_FCat3 = TaggedActors_FCat3.Contains(me);

    if (bIsTagged_FCat1)
    {
        // me�� FCat1 �±׸� ���� ��쿡�� �Ʒ� �ڵ带 ����
        FVector destination = bMovingToTarget01_FCat1 ? target01->GetActorLocation() : target02->GetActorLocation();

        FVector dir = destination - me->GetActorLocation();
        me->AddMovementInput(dir.GetSafeNormal());

        if (FVector::Dist(me->GetActorLocation(), destination) < ReachDistance)
        {
            // ��ǥ ���� ����, �̵� ��ǥ�� ��ȯ
            bMovingToTarget01_FCat1 = !bMovingToTarget01_FCat1;
        }
    }
    else if (bIsTagged_FCat2)
    {
        // me�� FCat2 �±׸� ���� ��쿡�� �Ʒ� �ڵ带 ����
        FVector destination = bMovingToTarget01_FCat2 ? target01->GetActorLocation() : target02->GetActorLocation();

        FVector dir = destination - me->GetActorLocation();
        me->AddMovementInput(dir.GetSafeNormal());

        if (FVector::Dist(me->GetActorLocation(), destination) < ReachDistance)
        {
            // ��ǥ ���� ����, �̵� ��ǥ�� ��ȯ
            bMovingToTarget01_FCat2 = !bMovingToTarget01_FCat2;
        }
    }
    else if (bIsTagged_FCat3)
    {
        // me�� FCat3 �±׸� ���� ��쿡�� �Ʒ� �ڵ带 ����
        FVector destination = bMovingToTarget01_FCat3 ? target01->GetActorLocation() : target02->GetActorLocation();

        FVector dir = destination - me->GetActorLocation();
        me->AddMovementInput(dir.GetSafeNormal());

        if (FVector::Dist(me->GetActorLocation(), destination) < ReachDistance)
        {
            // ��ǥ ���� ����, �̵� ��ǥ�� ��ȯ
            bMovingToTarget01_FCat3 = !bMovingToTarget01_FCat3;
        }
    }

    // ---------------------------------------------
    currentTime += GetWorld()->DeltaTimeSeconds;

    if (currentTime >= stTime)
    {
        // �±� ���
        TArray<FName> Tags = { FName("FCat1"), FName("FCat2"), FName("FCat3")};

        // �������� �±� ����
        int32 RandomIndex = FMath::RandRange(0, Tags.Num() - 1);
        SelectedTag = Tags[RandomIndex]; // ���õ� �±׸� ����

        // ���õ� �±׸� ���� ��� ���� ã��
        TArray<AActor*> TaggedCats;
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), SelectedTag, TaggedCats);
        currentTime = 0;

        // me�� ���õ� �±׸� ������ �ִ��� Ȯ��
        bool bIsTaggedCat = TaggedCats.Contains(me);

        if (bIsTaggedCat)
        {
            UE_LOG(LogTemp, Warning, TEXT("%s is moving to Straight - State with tag %s"), *me->GetName(), *SelectedTag.ToString());
            cState = ECatState::MoveWait;
        }
    }
}

void UJSH_CatFSM::MoveWaitState()
{
    // me�� ���õ� �±׸� ������ �ִ��� Ȯ��
    TArray<AActor*> TaggedCats;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), SelectedTag, TaggedCats);

    bool bIsTaggedCat = TaggedCats.Contains(me);

    if (bIsTaggedCat)
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
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("%s does not have the %s tag."), *me->GetName(), *SelectedTag.ToString());
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