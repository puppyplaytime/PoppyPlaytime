// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_SteamFSM.h"

#include "JSH_Steam.h"

// Sets default values for this component's properties
UJSH_SteamFSM::UJSH_SteamFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UJSH_SteamFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UJSH_SteamFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//me->RootComponent->IsVisible();
}

