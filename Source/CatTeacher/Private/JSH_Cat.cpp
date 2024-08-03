// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_Cat.h"
#include "JSH_CatFSM.h"
#include "KMK_Bullet.h"
#include "Components/BoxComponent.h"

// Sets default values
AJSH_Cat::AJSH_Cat()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// EnemyFSM 컴포넌트 추가
	fsm = CreateDefaultSubobject<UJSH_CatFSM>(TEXT("FSM"));

	// FalseCat Collision 설정 전 , FSM에서 Collision 변경 예정
	FalseBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FalseBox"));
	FalseBox->SetupAttachment(RootComponent);
	FalseBox->SetCollisionProfileName(TEXT("NoCollision"));
	FalseBox->SetWorldScale3D(FVector(7.0f, 7.0f, 7.0f));
	FalseBox->SetRelativeLocation(FVector(80.0f, 0.0f, 170.0f));
}

// Called when the game starts or when spawned
void AJSH_Cat::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJSH_Cat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AJSH_Cat::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AJSH_Cat::NotifyActorBeginOverlap(AActor* otherActor)
{
	Super::NotifyActorBeginOverlap(otherActor);

	AKMK_Bullet* Bullet = Cast<AKMK_Bullet>(otherActor);
	if (Bullet)
	{
		//fsm->FalseDie = true;
		fsm->IdleTeleport = true;
		fsm->cState = ECatState::Idle;
		//Destroy(Bullet);
	}
}