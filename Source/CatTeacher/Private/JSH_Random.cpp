#include "JSH_Random.h"

#include "EngineUtils.h"
#include "JSH_Cat.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "KMK_Player.h" // AJSH_Cat Ŭ������ �����ϴ� ��� ����
#include "JSH_CatFSM.h" // UJSH_CatFSM Ŭ������ �����ϴ� ��� ����

// Sets default values
AJSH_Random::AJSH_Random()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AJSH_Random::BeginPlay()
{
	Super::BeginPlay();
	
	// Set timer to call FindAndSelectRandomTag every 10 seconds
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AJSH_Random::FindAndSelectRandomTag, 5.0f, true);
}

// Called every frame
void AJSH_Random::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AJSH_Random::FindAndSelectRandomTag()
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
        
		UE_LOG(LogTemp, Log, TEXT("Random Tag: %s"), *RandomTag.ToString());  // �������� ���õ� �±׸� �α׷� ���

		// FSM ������Ʈ�� ���� �±� ����
		for (TActorIterator<AJSH_Cat> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			AJSH_Cat* CatActor = *ActorItr;
			UJSH_CatFSM* CatFSM = CatActor->FindComponentByClass<UJSH_CatFSM>();
			if (CatFSM)
			{
				CatFSM->SelectedTag = RandomTag;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Tags Found in Level"));
	}
}
