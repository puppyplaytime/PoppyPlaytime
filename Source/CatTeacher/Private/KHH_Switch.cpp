// Fill out your copyright notice in the Description page of Project Settings.


#include "KHH_Switch.h"
#include "KMK_PlayerHand.h"
#include "GameFramework/Actor.h"
#include "KHH_BossOpendoor.h"
#include "KMK_PlayerHandFSM.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AKHH_Switch::AKHH_Switch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AKHH_Switch::BeginPlay()
{
	Super::BeginPlay();
	BoxComp->OnComponentBeginOverlap.AddDynamic(this,&AKHH_Switch::OnMyBoxBeginOverlap);
}

// Called every frame
void AKHH_Switch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void AKHH_Switch::NotifyActorBeginOverlap(AActor* OtherActor)
//{
//	Super::NotifyActorBeginOverlap(OtherActor);
//
//	UE_LOG(LogTemp,Warning,TEXT("switch1"));
//	
//
//	auto* FSM = Cast<UKMK_PlayerHandFSM>(OtherActor);
//	auto* Hand = Cast<AKMK_PlayerHand>(OtherActor);
//
//	if (FSM && OtherActor->ActorHasTag("Green"))
//	{
//		if (FSM->isCharge == true)
//		{
//			//FSM -> isCharge = false;
//
//			if (Hand->SwitchName == "BP_Switch_C_1")
//			{
//				Opendoor();
//			}
//			else if (Hand->SwitchName == "BP_Switch_C_7")
//			{
//				Opendoor();
//			}
//			else if (Hand->SwitchName == "BP_Switch_C_9")
//			{
//				Opendoor();
//			}
//
//		}
//
//	}
//}

void AKHH_Switch::Opendoor()
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		BossOpendoorComponent = OwnerActor->FindComponentByClass<UKHH_BossOpendoor>();
		if (BossOpendoorComponent)
		{
			BossOpendoorComponent->ShouldMove = true;
		}
	}
}

void AKHH_Switch::OnMyBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, 
AActor* OtherActor, 
UPrimitiveComponent* OtherComp, 
int32 OtherBodyIndex, 
bool bFromSweep, 
const FHitResult& SweepResult)
{

	auto* FSM = Cast<UKMK_PlayerHandFSM>(OtherActor);
	auto* Hand = Cast<AKMK_PlayerHand>(OtherActor);
	
	UE_LOG(LogTemp, Warning, TEXT("switch1"));
	if (OtherActor->ActorHasTag("Green"))
	{
		if (FSM->isCharge == true && FSM)
		{
			FSM->isCharge = false;

			if (Hand->SwitchName == "BP_Switch_C_1")
			{
				Opendoor();
			}
			else if (Hand->SwitchName == "BP_Switch_C_7")
			{
				Opendoor();
			}
			else if (Hand->SwitchName == "BP_Switch_C_9")
			{
				Opendoor();
			}

		}

	}
}

