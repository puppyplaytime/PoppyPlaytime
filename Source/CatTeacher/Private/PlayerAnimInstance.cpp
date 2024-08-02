// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "KMK_Player.h"

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    AKMK_Player* player = Cast<AKMK_Player>(TryGetPawnOwner());

    if (player == nullptr) return;
    FVector v = player->GetVelocity();
    FVector forward = player->GetActorForwardVector();
    Speed = FVector::DotProduct(v, forward);
    Spd = FVector::DotProduct(v, player->GetActorRightVector());
}
