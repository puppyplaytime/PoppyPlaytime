// Fill out your copyright notice in the Description page of Project Settings.


#include "KHH_Enemy.h"
#include "KHH_EnemyFSM.h"


// Sets default values
AKHH_Enemy::AKHH_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));

	if (tempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(tempMesh.Object);

		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
	}
	fsm = CreateDefaultSubobject<UKHH_EnemyFSM>(TEXT("FSM"));

}

// Called when the game starts or when spawned
void AKHH_Enemy::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AKHH_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isRun == true)
	{
		fsm->isRun = true;
	}

	//float SFXSpd = GetVeloFunc();
	//if (SFXSpd > 0 && SFXSpd < 800)
	//{
	//	isSFX[0] = true;
	//	isSFX[1] = false;
	//}
	//else if (SFXSpd >= 800)
	//{
	//	isSFX[0] = false;
	//	isSFX[1] = true;
	//}
	//else
	//{
	//	isSFX[0] = false;
	//	isSFX[1] = false;
	//}
}

// Called to bind functionality to input
void AKHH_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//void AKHH_Enemy::PlayEnemySound(int index)
//{
//	if (audioComps[index] && !audioComps[index]->IsPlaying())
//	{
//		audioComps[index]->Play();
//	}
//}
//
//void AKHH_Enemy::StopEnemySound(int index)
//{
//	if (audioComps[index] && audioComps[index]->IsPlaying())
//	{
//		audioComps[index]->Stop();
//	}
//}
//
//float AKHH_Enemy::GetVeloFunc()
//{
//	return GetVelocity().Size();
//}

