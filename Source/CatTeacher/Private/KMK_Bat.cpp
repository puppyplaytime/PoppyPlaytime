// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_Bat.h"

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
}


// Called every frame
void UKMK_Bat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	meshBat->SetVisibility(isCome);

}

