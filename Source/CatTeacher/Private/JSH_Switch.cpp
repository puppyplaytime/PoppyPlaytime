	// Fill out your copyright notice in the Description page of Project Settings.


	#include "JSH_Switch.h"

	#include "KMK_PlayerHand.h"
	#include "KMK_PlayerHandFSM.h"

	// Sets default values
	AJSH_Switch::AJSH_Switch()
	{
 		// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
		PrimaryActorTick.bCanEverTick = true;

	}

	// Called when the game starts or when spawned
	void AJSH_Switch::BeginPlay()
	{
		Super::BeginPlay();
		
	}

	// Called every frame
	void AJSH_Switch::Tick(float DeltaTime)
	{
		Super::Tick(DeltaTime);
		
	}



	void AJSH_Switch::NotifyActorBeginOverlap(AActor* OtherActor)
	{
		Super::NotifyActorBeginOverlap(OtherActor);
		
		AKMK_PlayerHand* Hand = Cast<AKMK_PlayerHand>(OtherActor);
		
		if (Hand)
		{
			FSM = Hand->FSM;
			if (FSM && FSM->isCharge == true)
			{
				GEngine->AddOnScreenDebugMessage(8, 1, FColor::Yellow, FString::Printf(TEXT("On Switch")));
				FSM->isCharge = false;
			}
		}
	}


