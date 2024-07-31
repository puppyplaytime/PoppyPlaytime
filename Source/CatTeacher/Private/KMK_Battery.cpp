// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_Battery.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "UObject/UObjectBaseUtility.h"

// Sets default values
AKMK_Battery::AKMK_Battery()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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

void AKMK_Battery::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!Other->GetName().Contains("hand"))
	{
		meshComp->SetWorldScale3D(FVector(1));
	}
}

void AKMK_Battery::NotifyActorBeginOverlap(AActor* OtherActor) //otheractor BatteryFrame
{
	if (OtherActor->GetActorLabel().Contains("BatteryFrame"))
	{
		this->Destroy();
	} // 배터리 채워신 상태로 에셋 변경  // actor blueprint로 직접 수정 
}


