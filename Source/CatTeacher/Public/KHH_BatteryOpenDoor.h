// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "KHH_Enemy.h"
#include "Templates/SubclassOf.h"
#include "KHH_BatteryOpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATTEACHER_API UKHH_BatteryOpenDoor : public UActorComponent
{
   GENERATED_BODY()

public:   
   // Sets default values for this component's properties
   UKHH_BatteryOpenDoor();

protected:
   // Called when the game starts
   virtual void BeginPlay() override;

public:   
   // Called every frame
   virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
   
   UPROPERTY(EditAnywhere)
   bool ShouldMove = false;

   UPROPERTY(EditAnywhere)
   class AKHH_Enemy* enemy;

    UPROPERTY(EditAnywhere)
   class AActor* lever;
private:
   UPROPERTY(EditAnywhere)
   FVector MoveOffset = FVector(0,0,270);
   // ������ ���� ���� �Ʒ��� �������� ������ 
   // FVector MoveOffset = FVector(0, 0, -270); // �𸮾󿡼� ���� ��������� �� 
   // Mobility => Movable


   FVector OriginalLocation;

   UPROPERTY(EditAnywhere)
   float MoveTime = 3;

   UPROPERTY(EditAnywhere)
	float DistanceThreshold = 300.0f;


   UPROPERTY(EditAnywhere)
   //class APawn* Enemy;
   class AActor* Player;
   FVector CurrentLocation;
   FVector TargetLocation;
   float Speed;
   FVector NewLocation;

   float CurrentTime = 0;

   void MoveDoor(float DeltaTime);   

   UPROPERTY(EditAnywhere)
   TArray<class AActor*> doors;

   class AKHH_Switch* SwitchComponent;
   class UKMK_Bat* BatComponent;
   class UKMK_Bat* BatComponent1;

   class UKHH_EnemyFSM* destroycomponent;
   class UKHH_EnemyFSM* destroycomponent1;
    UPROPERTY(EditAnywhere)
	TSubclassOf<class AKHH_Enemy> del;

   int count =0;

   UFUNCTION(BlueprintCallable)
   void SpawnEnemy();

   
	UPROPERTY(EditAnywhere)
	class ULeverAnimInstance* Anim;

	UPROPERTY(EditAnywhere)
	class USoundCue* BatOpenDoor;

	bool BOD = false;
};

