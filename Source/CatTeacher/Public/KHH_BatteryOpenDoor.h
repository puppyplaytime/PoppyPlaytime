// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
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
	float DistanceThreshold = 270.0f;

   UPROPERTY(EditAnywhere)
   //class APawn* Enemy;
   class APawn* Player;
   FVector CurrentLocation;
   FVector TargetLocation;
   float Speed;
   FVector NewLocation;

   void MoveDoor(float DeltaTime);   

   UPROPERTY(EditAnywhere)
   TArray<class AActor*> doors;

   class AKHH_Switch* SwitchComponent;
   class UKMK_Bat* BatComponent;
   class UKMK_Bat* BatComponent1;

};

