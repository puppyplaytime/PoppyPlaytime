// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_Battery.h"
#include "KMK_Bat.h"
#include "JSH_0304Steam.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UJSH_Battery::UJSH_Battery()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UJSH_Battery::BeginPlay()
{
	Super::BeginPlay();

	
	FindTaggedActors();
}


// Called every frame
void UJSH_Battery::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	// 민경 배터리 isHaveBat == true 됬을떄
	// niagara 꺼지고 , 3번 문 fsm 활성화

	BatComponent03 = batsave03->FindComponentByClass<UKMK_Bat>();
	FSMOnOff03 = BatComponent03->isHaveBat;
	if (BatComponent03->isHaveBat == false)
	{
		FSMOnOff03 = false;
	}
	if (BatComponent03->isHaveBat == true)
	{
		FSMOnOff03 = true;
	}

	BatComponent04 = batsave04->FindComponentByClass<UKMK_Bat>();
	FSMOnOff04 = BatComponent04->isHaveBat;
	if (BatComponent04->isHaveBat == false)
	{
		FSMOnOff04 = false;
	}
	if (BatComponent04->isHaveBat == true)
	{
		FSMOnOff04 = true;
	}
	
	
}

void UJSH_Battery::FindTaggedActors()
{
	UWorld* World = GetWorld();
	if (World)
	{
		// Arrays to store found actors
		TArray<AActor*> FoundActors03;
		TArray<AActor*> FoundActors04;

		// Find actors with tag "bat03"
		UGameplayStatics::GetAllActorsWithTag(World, FName("bat03"), FoundActors03);
		if (FoundActors03.Num() > 0)
		{
			batsave03 = FoundActors03[0];
		}

		// Find actors with tag "bat04"
		UGameplayStatics::GetAllActorsWithTag(World, FName("bat04"), FoundActors04);
		if (FoundActors04.Num() > 0)
		{
			batsave04 = FoundActors04[0];
		}
	}
}

