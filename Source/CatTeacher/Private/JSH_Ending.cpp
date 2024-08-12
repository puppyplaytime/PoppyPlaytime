// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_Ending.h"

#include "KMK_PlayerHand.h"
#include "Components/BoxComponent.h"

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
		// Cat의 애니메이션 실행 되도록
	}

	
}

