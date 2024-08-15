// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_EleButton.h"

#include "JSH_Movebox.h"
#include "KMK_PlayerHand.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "SFX_Manager.h"
#include "Components/AudioComponent.h"





// Sets default values
AJSH_EleButton::AJSH_EleButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJSH_EleButton::BeginPlay()
{
	Super::BeginPlay();


	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AJSH_Movebox::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (Actor->ActorHasTag("ele"))
		{
			Elevator = Cast<AJSH_Movebox>(Actor);
		}
		else if (Actor->ActorHasTag("cage"))
		{
			cage = Cast<AJSH_Movebox>(Actor);
		}
	}


}

// Called every frame
void AJSH_EleButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (Ofen)
	{
		CageOfen();
	}

	if (close)
	{
		CageClose();
	}

	if (upOn)
	{
		EleUp();
	}

}



void AJSH_EleButton::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);


	AKMK_PlayerHand* Hand = Cast<AKMK_PlayerHand>(OtherActor);


	if (Hand->overActor->ActorHasTag(FName("Button1")))
	{
		Ofen = true;
	}


	if (Hand->overActor->ActorHasTag(FName("Button2")))
	{
		close = true;
		Ofen = false;
	}



}


// void AJSH_EleButton::EleUp()
// {
// 	if (Elevator) // Ensure ElevatorActor is valid
// 	{
// 		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("22"));
//
// 		FVector CurrentLocation = Elevator->GetActorLocation();
//
//
// 		CurrentLocation.Z += eleSpeed;
//
// 		// Set the new location
// 		Elevator->SetActorLocation(CurrentLocation);
// 	}
// }


void AJSH_EleButton::EleUp()
{
	if (Elevator) 
	{
		// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("EleUp"));
		FVector CurrentLocation = Elevator->GetActorLocation();
		CurrentLocation.Z += eleSpeed;
		Elevator->SetActorLocation(CurrentLocation);

		if (MoveElv)
		{
			MoveElv = false;
			static USoundWave* ElvSound = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Project/KHH/Sound/BatteryOpendoor/poppy-playtime-opening-gate-sound-effect.poppy-playtime-opening-gate-sound-effect'"));
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ElvSound, GetActorLocation());
		}
		
		t += GetWorld()->DeltaTimeSeconds;
		//static float TargetZ = Elevator->GetActorLocation().Z + distance;
		
		if (t > UpTime)
		{
			auto* EleW = CreateWidget(GetWorld(), widfact);
			EleW->AddToViewport(3);
			auto* a = Cast<ASFX_Manager>(audioManager);
			if(a) a->audio->Stop();
		}

	}
}


void AJSH_EleButton::CageOfen()
{
	
	if (cage)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("CageOfen"));

		float TargetZ = distance;

		FVector CurrentLocation = cage->GetActorLocation();

		// ���������Ͱ� ��ǥ Z ��ġ�� �����ߴ��� Ȯ��
		if (CurrentLocation.Z <= TargetZ)
		{
			// Z ���� �������� ���������͸� ���� �̵�
			CurrentLocation.Z += cageOftenSpeed;

			cage->SetActorLocation(CurrentLocation);
		}
		else
		{
			// ���������Ͱ� ��ǥ ��ġ�� ���������Ƿ� ����
			Ofen = false;
			click02 = true;
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Elevator reached the target height."));
		}

		if (MoveElv2)
		{
			MoveElv2 = false;
			static USoundWave* ElvSound = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Project/KHH/Sound/BatteryOpendoor/poppy-playtime-opening-gate-sound-effect.poppy-playtime-opening-gate-sound-effect'"));
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ElvSound, GetActorLocation());
		}
	}
}


void AJSH_EleButton::CageClose()
{
	if (cage)
	{
		Ofen = false;
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("c1"));

		// ��ǥ Z ��ġ�� ���� (��: �ʱ� ��ġ + 200)
		float TargetZ = distance;

		// ���� ������������ ��ġ�� ������
		FVector CurrentLocation = cage->GetActorLocation();

		// ���������Ͱ� ��ǥ Z ��ġ�� �����ߴ��� Ȯ��
		if (CurrentLocation.Z >= TargetZ)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("c2"));
			// Z ���� �������� ���������͸� ���� �̵�
			CurrentLocation.Z -= cageCloseSpeed; // �ӵ��� �ʿ信 ���� ����

			// ���ο� ��ġ�� ����
			cage->SetActorLocation(CurrentLocation);
		}
		else
		{
			// ���������Ͱ� ��ǥ ��ġ�� ���������Ƿ� ����
			close = false;
			upOn = true;
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Elevator reached the target height."));
		}
	}
}

