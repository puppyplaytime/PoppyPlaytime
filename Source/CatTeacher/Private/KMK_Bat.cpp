// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_Bat.h"
#include "KMK_PlayerRay.h"

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
}


// Called every frame
void UKMK_Bat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// 배터리 여부에 따라 on/off
	meshBat->SetVisibility(isHaveBat);

}

