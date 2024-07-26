// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_Cat.h"

#include "JSH_CatFSM.h"

// Sets default values
AJSH_Cat::AJSH_Cat()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// EnemyFSM 컴포넌트 추가
	fsm = CreateDefaultSubobject<UJSH_CatFSM>(TEXT("FSM"));
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

