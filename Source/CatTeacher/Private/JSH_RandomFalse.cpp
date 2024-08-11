// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_RandomFalse.h"

#include "EngineUtils.h"
#include "JSH_Cat.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "KMK_Player.h" // AJSH_Cat Ŭ������ �����ϴ� ��� ����
#include "JSH_CatFSM.h" 

// Sets default values
AJSH_RandomFalse::AJSH_RandomFalse()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AJSH_RandomFalse::BeginPlay()
{
	Super::BeginPlay();
	
	// Set timer to call FindAndSelectRandomTag every 10 seconds
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AJSH_RandomFalse::FindAndSelectRandomTag, FalseRandomeTime, true);
	// test
}

// Called every frame
void AJSH_RandomFalse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AJSH_RandomFalse::FindAndSelectRandomTag()
{
	// ���͸��� �±׵��� �迭�� ����
	TArray<FName> FilteredTags = { FName(TEXT("FCat1")), FName(TEXT("FCat2")), FName(TEXT("FCat3")), FName(TEXT("FCat4")) };
	TArray<FName> TagsInLevel;

	// ������ �ִ� �±� �߿��� ���͸��� �±׵鸸 ã��


	
	
	for (TActorIterator<AJSH_Cat> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AJSH_Cat* CatActor = *ActorItr;

		for (const FName& Tag : CatActor->Tags)
		{
			if (FilteredTags.Contains(Tag))
			{
				if (!TagsInLevel.Contains(Tag))
				{
					TagsInLevel.Add(Tag);
				}
			}
		}
	}

	if (TagsInLevel.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, TagsInLevel.Num() - 1);
		FName RandomTag = TagsInLevel[RandomIndex];

		UE_LOG(LogTemp, Log, TEXT("False Tag: %s"), *RandomTag.ToString());  // �������� ���õ� �±׸� �α׷� ���

		// FSM ������Ʈ�� ���� �±� ����
		for (TActorIterator<AJSH_Cat> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			AJSH_Cat* CatActor = *ActorItr;
			UJSH_CatFSM* CatFSM = CatActor->FindComponentByClass<UJSH_CatFSM>();
			if (CatFSM)
			{
				CatFSM->SelectedTagFalse = RandomTag;
				GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Red, TEXT("False"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No False Tags Found in Level"));
	}
}