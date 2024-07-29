// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_RandomFalse.h"

#include "EngineUtils.h"
#include "JSH_Cat.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "KMK_Player.h" // AJSH_Cat 클래스를 포함하는 헤더 파일
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
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AJSH_RandomFalse::FindAndSelectRandomTag, 10.0f, true);
	// test
}

// Called every frame
void AJSH_RandomFalse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AJSH_RandomFalse::FindAndSelectRandomTag()
{
	TArray<FName> TagsInLevel;
	for (TActorIterator<AJSH_Cat> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AJSH_Cat* CatActor = *ActorItr;
		TagsInLevel.Append(CatActor->Tags);
	}

	if (TagsInLevel.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, TagsInLevel.Num() - 1);
		FName RandomTag = TagsInLevel[RandomIndex];
        
		UE_LOG(LogTemp, Log, TEXT("False Tag: %s"), *RandomTag.ToString());  // 랜덤으로 선택된 태그를 로그로 출력

		// FSM 컴포넌트에 랜덤 태그 전달
		for (TActorIterator<AJSH_Cat> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			AJSH_Cat* CatActor = *ActorItr;
			UJSH_CatFSM* CatFSM = CatActor->FindComponentByClass<UJSH_CatFSM>();
			if (CatFSM)
			{
				CatFSM->SelectedTagFalse = RandomTag;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Flase Tags Found in Level"));
	}
}
