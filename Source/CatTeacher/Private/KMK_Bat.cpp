// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_Bat.h"
#include "KMK_PlayerRay.h"
#include "KHH_BatteryOpenDoor.h"
#include "Engine/StaticMeshActor.h"

// Sets default values for this component's properties
UKMK_Bat::UKMK_Bat()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UKMK_Bat::BeginPlay()
{
	Super::BeginPlay();
	meshBat = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	player = GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<UKMK_PlayerRay>();
	// 머테리얼 변경
	myMatDynamic = UMaterialInstanceDynamic::Create(matFact, this);
	if (batProgress != nullptr)
	{
		// batProgress->SetMaterial(0, myMatDynamic);
        auto* b = batProgress->GetStaticMeshComponent();
        if (b)
        {
            b->SetMaterial(0, myMatDynamic);
        }
		myMatDynamic->SetScalarParameterValue("Gage", 0);
	}
	
}

// 배터리가 들어간 상태일때 문이 열리도록 하고 싶다. 
// IsPUsh 일때 shouldMove가 ture로 바꾸고 싶다. 
// 1. 문의 컴포넌트랑 bat를 연결 => 1-2 사용
// 방법 1-1. 코드로 가져오겠다
// 방법 1-2. 직접할당을 하겠다 :
// 필요요소 : TargetDoor, ispush
// 2. ISPUSH, shouldMove를 연동
// 
// Called every frame
void UKMK_Bat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 배터리 여부에 따라 on/off
	meshBat->SetVisibility(isHaveBat);
	
	if (isHaveBat == true)
	{
		// 딜라이트 맵용 단자들
		if(TargetDoor != nullptr)SetTargetDoor(TargetDoor);
		// 캣냅용 단자들 => 컴퓨터 근처에 있는 단자용
		if (batProgress != nullptr)
		{
			if (!isStart) return;
			spd += ChargeSpeed;
			ChargeGage(spd);
			if (spd > 100)
			{
				spd = 100;
			}
		}
	}

}

void UKMK_Bat::SetTargetDoor(AActor* NewTargetDoor)
{
	if (NewTargetDoor != nullptr)
	{	// 1. 문의 컴포넌트랑 bat를 연결 => 1-2 사용
		BatteryOpendoorComponent = NewTargetDoor->FindComponentByClass<UKHH_BatteryOpenDoor>();
		// 2. ISPUSH, shouldMove를 연동
		if (BatteryOpendoorComponent)
		{
			BatteryOpendoorComponent->ShouldMove = true;
		}
	}
}

void UKMK_Bat::ChargeGage(float DeltaTime)
{
	myMatDynamic->SetScalarParameterValue("Gage", DeltaTime);
}

