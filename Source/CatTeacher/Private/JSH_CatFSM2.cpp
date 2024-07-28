// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_CatFSM2.h"

#include "JSH_Cat2.h"
#include "KMK_Player.h"
#include "Kismet/GameplayStatics.h"
#include "JSH_Target.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UJSH_CatFSM2::UJSH_CatFSM2()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UJSH_CatFSM2::BeginPlay()
{
	Super::BeginPlay();
	
	// ���忡�� Player ã�ƿ���
	//auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AJSH_Cat::StaticClass());
	//layer = Cast<AJSH_Cat>(actor);
	
	// ���� ��ü ��������
	me = Cast<AJSH_Cat2>(GetOwner());

	cState2 = ECatState2::RoundMove;

	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(me->GetCharacterMovement());
	if (MovementComponent)
	{
		MovementComponent->MaxWalkSpeed = 300.0f;  // ���ϴ� �ִ� �ӵ��� ����
	}


}


// Called every frame
void UJSH_CatFSM2::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (cState2)
	{
	case ECatState2::RoundMove:
		RoundMoveState();
		break;
		
	case ECatState2::MoveWait:
		MoveWaitState();
		break;
		
	case ECatState2::TrueMove:
		TrueMoveState();
		break;

	case ECatState2::FalseMove:
		FalseMoveState();
		break;

	case ECatState2::Ceiling:
		CeilingState();
		break;
		
	case ECatState2::Discovery:
		DiscoveryState();
		break;
		
	case ECatState2::Attack:
		AttackState();
		break;

	case ECatState2::Blocked:
		BlockedState();
		break;
		
	case ECatState2::Die:
		DieState();
		break;
	}

	FString logMsg = UEnum::GetValueAsString(cState2);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, logMsg);
}

void UJSH_CatFSM2::RoundMoveState()
{
    static bool bMovingToTarget01_FCat2 = true; // FCat2 ��� ���� �̵� �÷���
	
    TArray<AActor*> TaggedActors_FCat2;

    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("FCat2"), TaggedActors_FCat2);
	
    // me�� FCat2 �±׸� ������ �ִ��� Ȯ��
    bool bIsTagged_FCat2 = TaggedActors_FCat2.Contains(me);

    if (bIsTagged_FCat2)
    {
        // me�� FCat2 �±׸� ���� ��쿡�� �Ʒ� �ڵ带 ����
        FVector destination = bMovingToTarget01_FCat2 ? target01->GetActorLocation() : target02->GetActorLocation();

        FVector dir = destination - me->GetActorLocation();
        me->AddMovementInput(dir.GetSafeNormal());

        if (FVector::Dist(me->GetActorLocation(), destination) < ReachDistance)
        {
            // ��ǥ ���� ����, �̵� ��ǥ�� ��ȯ
            bMovingToTarget01_FCat2 = !bMovingToTarget01_FCat2;
        	//cState2 = ECatState2::MoveWait;
        }
    }
}

void UJSH_CatFSM2::MoveWaitState()
{
	// �±� ����
	if (me && me->Tags.Contains(FName("FCat2"))) 
	{
		me->Tags.Remove(FName("FCat2"));
	}
	
	FVector destinationM = targetMiddle->GetActorLocation();
	FVector dirM = destinationM - me->GetActorLocation();
	me->AddMovementInput(dirM.GetSafeNormal());
	if (dirM.Size() < ReachDistance)
	{
		// ���� ��ȯ
		cState2 = ECatState2::TrueMove;
	}
}



void UJSH_CatFSM2::TrueMoveState()
{
	FVector destinationPlayer = targetPlayer->GetActorLocation();
	FVector dirPlayer = destinationPlayer - me->GetActorLocation();
	me->AddMovementInput(dirPlayer.GetSafeNormal());


	if (dirPlayer.Size() < ReachDistance)
	{
		// ���� ��ȯ
		cState2 = ECatState2::Attack;
	}
	// Ư�� ��ġ ���� -> AttackState()  ___ Game Over
	// (ȯ��) �����ΰ��ٰ� , ��ȣź ���� -> DiscoveryState()  ___ õõ�� �����
	// (��¥)  �����ΰ��ٰ� player�� ��� ���� ->  DieState() ___ destroy �� ���� �պ� ��ġ spawn 
}


void UJSH_CatFSM2::FalseMoveState()
{

}

void UJSH_CatFSM2::CeilingState()
{
}

void UJSH_CatFSM2::DiscoveryState()
{
}

void UJSH_CatFSM2::AttackState()
{
}

void UJSH_CatFSM2::BlockedState()
{
}

void UJSH_CatFSM2::DieState()
{
}

void UJSH_CatFSM2::OnDamgaeProcess()
{
}