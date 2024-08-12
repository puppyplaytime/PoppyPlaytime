// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_Ending.h"

#include "JSH_CatDoor.h"
#include "JSH_EndingCat.h"
#include "KHH_RotateDoor.h"
#include "KMK_PlayerHand.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

class AKMK_PlayerHand;
// Sets default values
AJSH_Ending::AJSH_Ending()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FalseBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FalseBox"));
	FalseBox->SetupAttachment(RootComponent);
	// FalseBox->SetCollisionProfileName(TEXT("NoCollision"));
	FalseBox->SetCollisionProfileName(TEXT("OverlapAll"));
	
	// FalseBox->SetWorldScale3D(FVector(7.0f, 7.0f, 7.0f));
	// FalseBox->SetRelativeLocation(FVector(80.0f, 0.0f, 170.0f));
}

// Called when the game starts or when spawned
void AJSH_Ending::BeginPlay()
{
	Super::BeginPlay();


	// CatDoor 액터 가져오기
	CatDoor = Cast<AJSH_CatDoor>(UGameplayStatics::GetActorOfClass(GetWorld(), AJSH_CatDoor::StaticClass()));

	if (CatDoor)
	{
		// 'Door'라는 이름으로 액터의 참조를 저장합니다.
		FName DoorName = "Door";
		CatDoor->SetActorLabel(DoorName.ToString());
	}

	me = Cast<AJSH_EndingCat>(GetOwner());
	
}

// Called every frame
void AJSH_Ending::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

}

void AJSH_Ending::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);


	AKMK_PlayerHand* Hand = Cast<AKMK_PlayerHand>(OtherActor);

	if (Hand && OtherActor->ActorHasTag("Green"))
	{
		CatDoor->fsm->cnt++;
		CatDoor->fsm->isOpen = true;
		// Cat의 애니메이션 실행 되도록
	}

	
}

