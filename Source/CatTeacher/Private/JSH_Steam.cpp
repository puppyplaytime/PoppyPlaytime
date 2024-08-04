// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_Steam.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
AJSH_Steam::AJSH_Steam()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Create a Niagara Component
	UNiagaraComponent* NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	SetRootComponent(NiagaraComponent);
	
	// Load the Niagara System
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSystem(TEXT("/Script/Niagara.NiagaraSystem'/Game/Effect/NG_Steam.NG_Steam'"));

	if (NiagaraSystem.Succeeded())
	{
		// Set the Niagara System to the Niagara Component
		NiagaraComponent->SetAsset(NiagaraSystem.Object);
	}

	// Set the Niagara Component as the Root Component
	RootComponent = NiagaraComponent;
	
}

// Called when the game starts or when spawned
void AJSH_Steam::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJSH_Steam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

