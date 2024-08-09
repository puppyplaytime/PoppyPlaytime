// Fill out your copyright notice in the Description page of Project Settings.


#include "LeverComponent.h"
#include "KMK_Player.h"
#include "LeverAnimInstance.h"
#include "KHH_Enemy.h"

// Sets default values for this component's properties
ULeverComponent::ULeverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULeverComponent::BeginPlay()
{
	Super::BeginPlay();
	InitialRotation = GetOwner()->GetActorRotation();
	// ...
	

}


// Called every frame
//void ULeverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	if(me)GEngine->AddOnScreenDebugMessage(10, 1, FColor::Magenta, FString::Printf(TEXT("find!")));
//	//if (leverAnim->LeverMove)
//
//	
//	if (LeverMove)
//	{
//		CurrentRotation = GetOwner()->GetActorRotation();
//		TargetRotation = InitialRotation + OpenAngle;
//		Speed = FRotator::NormalizeAxis((TargetRotation - InitialRotation).Roll) / MoveTime;
//		if (Speed < 0)
//		{
//			Speed *= -1;
//		}
//		NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, Speed);
//		GetOwner()->SetActorRotation(NewRotation);
//	}
//}

void ULeverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    UAnimInstance* AnimInstance = nullptr;
    if (AActor* Owner = GetOwner())
    {
        if (USkeletalMeshComponent* MeshComp = Owner->FindComponentByClass<USkeletalMeshComponent>())
        {
            AnimInstance = MeshComp->GetAnimInstance();
        }
    }
    // 1. enemy 내부에 있는 ULevelAnimInstance 가져오기
    if (ULeverAnimInstance* LeverAnim = Cast<ULeverAnimInstance>(AnimInstance))
    {
        if(me)GEngine->AddOnScreenDebugMessage(10, 1, FColor::Magenta, FString::Printf(TEXT("find!")));
        if (LeverAnim->LeverMove)
        {
            CurrentRotation = GetOwner()->GetActorRotation();
            TargetRotation = InitialRotation + OpenAngle;
            Speed = FRotator::NormalizeAxis((TargetRotation - InitialRotation).Roll) / MoveTime;
            if (Speed < 0)
            {
                Speed *= -1;
            }
            NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, Speed);
            GetOwner()->SetActorRotation(NewRotation);
        }
    }
}


