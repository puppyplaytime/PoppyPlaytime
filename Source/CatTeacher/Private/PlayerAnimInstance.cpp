// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "KMK_Player.h"
#include "KMK_PlayerHandFSM.h"
#include "KMK_PlayerHand.h"

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    player = Cast<AKMK_Player>(TryGetPawnOwner());

    if (player == nullptr) return;
    FVector v = player->GetVelocity();
    FVector forward = player->GetActorForwardVector();
    Speed = FVector::DotProduct(v, forward);
    Spd = FVector::DotProduct(v, player->GetActorRightVector());
}

void UPlayerAnimInstance::PlayFireMontage()
{
    Montage_Play(Monts[0]);
}

void UPlayerAnimInstance::PlayHandMontage()
{
    Montage_Play(Monts[1]);
}

void UPlayerAnimInstance::PlayChangeMontage()
{
    Montage_Play(Monts[2]);
}

void UPlayerAnimInstance::PlayHandInMontage()
{
    Montage_JumpToSection("HandIn");
}

void UPlayerAnimInstance::AnimNotify_FireEnd()
{
    player->FireBulletEnd();
    auto* fsm = TryGetPawnOwner()->GetComponentByClass<UKMK_PlayerHandFSM>();
    fsm->isFire = false;
    fsm->cnt = 0;
    clickCount++;
}

void UPlayerAnimInstance::AnimNotify_ChangeHand()
{
    player->ChangeHandEnd();

    if (player->FSM->PState == PlayerHandFSM::JumpPack)
    {
        player->Hands[0]->hand->SetMaterial(0, player->FSM->matArrays[0]);
        player->RMeshComp->SetStaticMesh(player->Hands[0]->HandMesh[2]);
    }
    else if (player->FSM->PState == PlayerHandFSM::GunPack)
    {
        player->Hands[0]->hand->SetMaterial(0, player->FSM->matArrays[1]);
        player->RMeshComp->SetStaticMesh(player->Hands[0]->HandMesh[1]);
    }
    else
    {
        player->Hands[0]->hand->SetMaterial(0, player->Hands[0]->myMatDynamic);
        player->RMeshComp->SetStaticMesh(player->Hands[0]->HandMesh[0]);
    }
}

