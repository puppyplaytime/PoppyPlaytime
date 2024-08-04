	// Fill out your copyright notice in the Description page of Project Settings.


	#include "JSH_Switch.h"

	#include "JSH_Cat.h"
	#include "JSH_CatFSM.h"
	#include "KMK_PlayerHand.h"
	#include "KMK_PlayerHandFSM.h"
#include "Kismet/GameplayStatics.h"

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
		//AJSH_Cat* Cat = Cast<AJSH_Cat>(OtherActor);
		
		if (Hand && OtherActor->ActorHasTag("Green"))
		{
			FSM = Hand->FSM;
			
			if (FSM && FSM->isCharge == true)
			{
				GEngine->AddOnScreenDebugMessage(8, 3, FColor::Yellow, FString::Printf(TEXT("On Switch")));
				FSM->isCharge = false;

				if (Hand->SwitchName == "BP_Switch_C_1")
				{
					// 1번 통로 연기 -> cat01 destroy
					GEngine->AddOnScreenDebugMessage(31, 3, FColor::Red, FString::Printf(TEXT("C1")));

					TArray<AActor*> FoundActors;
					UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("S1"), FoundActors);

					// 찾은 액터들 순회
					for (AActor* Actor : FoundActors)
					{
						AJSH_Cat* Cat = Cast<AJSH_Cat>(Actor);
						if (Cat)
						{
							// tt 변수를 true로 설정
							Cat->fsm->SwSt1 = true;
							Cat->Tags.Remove("FCat1");
						}
					}

				}
				// 2번 통로
				else if (Hand->SwitchName == "BP_Switch_C_0")
				{
					// 2번 통로 연기 -> cat01 destroy
					GEngine->AddOnScreenDebugMessage(31, 3, FColor::Yellow, FString::Printf(TEXT("C2")));
				}
				// 3번 통로
				else if (Hand->SwitchName == "BP_Switch_C_0")
				{
					// 2번 통로 연기 -> cat01 destroy
					GEngine->AddOnScreenDebugMessage(31, 3, FColor::Yellow, FString::Printf(TEXT("C2")));
				}
				// 4번 통로
				else if (Hand->SwitchName == "BP_Switch_C_0")
				{
					// 2번 통로 연기 -> cat01 destroy
					GEngine->AddOnScreenDebugMessage(31, 3, FColor::Yellow, FString::Printf(TEXT("C2")));
				}
			}
		}
	}


