// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_FinalSwitch.h"
#include "KMK_Bat.h"

// Sets default values for this component's properties
UKMK_FinalSwitch::UKMK_FinalSwitch()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKMK_FinalSwitch::BeginPlay()
{
	Super::BeginPlay();
	GetOwner()->SetActorEnableCollision(false);
	for(int i = 0; i < 4; i++)
	{
		batsComps.Add(bats[i]->FindComponentByClass<UKMK_Bat>());
	}
}


// Called every frame
void UKMK_FinalSwitch::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GEngine->AddOnScreenDebugMessage(5, 1, FColor::White, FString::Printf(TEXT("%f"), allCharge));
	GEngine->AddOnScreenDebugMessage(6, 1, FColor::Yellow, FString::Printf(TEXT("%d"), count));
	if (allCharge > 400)
	{
		allCharge = 400;
		GetOwner()->SetActorEnableCollision(true);
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			if (batsComps[i]->isHaveBat && !isHave[i])
			{
				count++;
				isHave[i] = true;
				
			}
			if (!batsComps[i]->isHaveBat && isHave[i])
			{
				count--;
				isHave[i] = false;
			}
			
		}

		if(count != 0) allCharge += batsComps[0]->ChargeSpeed * count;
	}
}

