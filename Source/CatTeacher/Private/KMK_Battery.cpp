// Fill out your copyright notice in the Description page of Project Settings.
#include "KMK_Battery.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "UObject/UObjectBaseUtility.h"
#include "KMK_PlayerHand.h"
#include "KMK_Bat.h"

// �տ� ��� �ִ� ���͸�
// Sets default values
AKMK_Battery::AKMK_Battery()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(meshComp);

	meshComp->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AKMK_Battery::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AKMK_Battery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// ���͸��� visible���� �Լ�
void AKMK_Battery::SetVis(bool isPut)
{
	// ���͸��� ���ٸ�
	isPush = isPut;
	// ������ �ʰ� �����
	meshComp->SetRenderInMainPass(isPush);
}

void AKMK_Battery::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Other->GetActorLabel().Contains("Bool") || Other->GetActorLabel().Contains("Cube"))
	{
		meshComp->SetWorldScale3D(FVector(1));
	}
}


