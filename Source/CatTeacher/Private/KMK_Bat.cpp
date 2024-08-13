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
	// ���׸��� ����
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

// �̰� ȭ���̶� �ϴ� �κ��̿��� �Ű�  X
// ���͸��� �� �����϶� ���� �������� �ϰ� �ʹ�. 
// IsPUsh �϶� shouldMove�� ture�� �ٲٰ� �ʹ�. 
// 1. ���� ������Ʈ�� bat�� ���� => 1-2 ���
// ��� 1-1. �ڵ�� �������ڴ�
// ��� 1-2. �����Ҵ��� �ϰڴ� :
// �ʿ��� : TargetDoor, ispush
// 2. ISPUSH, shouldMove�� ����

// Ĺ���� ���� ���α׷����� ���� �̾߱�
// isHaveBat�� ���� �������� ������ ���͸� ���� �������Դϴ�.
// ChargeSpeed�� ���� ���α׷����� �ӵ� ���� �����մϴ�.
void UKMK_Bat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ���͸� ���ο� ���� on/off
	meshBat->SetVisibility(isHaveBat);
	// ���͸��� ����������
	if (isHaveBat == true)
	{
		// ������Ʈ �ʿ� ���ڵ�
		if(TargetDoor != nullptr)SetTargetDoor(TargetDoor);
		// Ĺ���� ���ڵ� => ��ǻ�� ��ó�� �ִ� ���ڿ�
		if (batProgress != nullptr)
		{
			// Ĺ�� ��� ���������� ���α׷��� �ٰ� �������� �ʰ� ����� ����
			if (!isStart) return;

			// Material ���α׷����� ä������ �� ���� �Լ�
			ChargeGage(ChargeSpeed, 1);
		}
	}

}

void UKMK_Bat::SetTargetDoor(AActor* NewTargetDoor)
{
	if (NewTargetDoor != nullptr)
	{	// 1. ���� ������Ʈ�� bat�� ���� => 1-2 ���
		BatteryOpendoorComponent = NewTargetDoor->FindComponentByClass<UKHH_BatteryOpenDoor>();
		// 2. ISPUSH, shouldMove�� ����
		if (BatteryOpendoorComponent)
		{
			BatteryOpendoorComponent->ShouldMove = true;
		}
	}
}

void UKMK_Bat::ChargeGage(float speed, int count)
{
	// ���۵ǰ� ���͸��� �ִٸ� õõ�� �����Ǳ� ����
	spd += speed * count;
	myMatDynamic->SetScalarParameterValue("Gage", spd );
	if (spd >= 100)
	{
		spd = 100;
	}
}

