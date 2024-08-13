// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_FinalSwitch.h"
#include "KMK_Bat.h"
#include "KMK_Player.h"

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
	bats[4]->SetActorEnableCollision(false);
	for(int i = 0; i < 4; i++)
	{
		batsComps.Add(bats[i]->FindComponentByClass<UKMK_Bat>());
	}
	meshComp = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	myMatDynamic = UMaterialInstanceDynamic::Create(matFact, this);
	if (matFact != nullptr && meshComp != nullptr) meshComp->SetMaterial(0, myMatDynamic);
	myMatDynamic->SetScalarParameterValue("Page", 0);
	myMatDynamic->SetScalarParameterValue("Gage", 100);
}


// Called every frame
void UKMK_FinalSwitch::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto* player = Cast<AKMK_Player>(GetWorld()->GetFirstPlayerController()->GetPawn());
	float d = FVector::Distance(player->GetActorLocation(), GetOwner()->GetTargetLocation());
	GEngine->AddOnScreenDebugMessage(5, 1, FColor::White, FString::Printf(TEXT("%f"), d));
	if (!isTrue && d < playerDist)
	{
		t += DeltaTime;
		if (t > 1)
		{
			myMatDynamic->SetScalarParameterValue("Page", 1);
			if (t > 3.f)
			{
				t = 0;
				isTrue = true;
				myMatDynamic->SetScalarParameterValue("Gage", 0);
			}
		}
	}

	if(!isTrue) return;
	
	GEngine->AddOnScreenDebugMessage(5, 1, FColor::White, FString::Printf(TEXT("%f"), allCharge));
	GEngine->AddOnScreenDebugMessage(6, 1, FColor::Yellow, FString::Printf(TEXT("%d"), count));
	if (allCharge > 100 || (player != nullptr && player->isCheat2))
	{
		allCharge = 100;
		bats[4]->SetActorEnableCollision(true);
	}
	else
	{
		myMatDynamic->SetScalarParameterValue("Gage", allCharge);
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

		if(count != 0) allCharge += batsComps[0]->ChargeSpeed / 4 * count;
	}
}

