// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_PlayerHand.h"
#include "Components/ArrowComponent.h"


// Sets default values
AKMK_PlayerHand::AKMK_PlayerHand()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//// 왼손
	hand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandComp"));
	SetRootComponent(hand);
	hand->SetRelativeRotation(FRotator(0, -90, 0));
	hand->SetRelativeScale3D(FVector(1.5f));
	// 발사 위치만들기
	arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePos"));
	arrow->SetupAttachment(hand);
	arrow->SetRelativeLocationAndRotation(FVector(0, 10, 2.2f), FRotator(0, 90, 0));
	arrow->SetRelativeScale3D(FVector(0.2f));
}

// Called when the game starts or when spawned
void AKMK_PlayerHand::BeginPlay()
{
	Super::BeginPlay();

	hand->SetStaticMesh(HandMesh[0]);
}

// Called every frame
void AKMK_PlayerHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isGo)
	{
		dir = endPos - GetActorLocation();
		if (dir.Length() < 10)
		{
			isGo = false;
			isReverse = true;
		}
		dir.Normalize();
		SetActorLocation(GetActorLocation() + dir * speed * DeltaTime);
		
	}
	if (isReverse)
	{
		dir = startPos - GetActorLocation();
		dir.Normalize();
		SetActorLocation(GetActorLocation() + dir * speed * DeltaTime);
		if ((startPos - GetActorLocation()).Length() < 4)
		{
			isReverse = false;
			SetActorRelativeLocation(handPos);
		}
	}
}

