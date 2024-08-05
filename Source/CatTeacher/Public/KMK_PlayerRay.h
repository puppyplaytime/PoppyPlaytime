// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KMK_PlayerRay.generated.h"

// 3������ ���̸� ���鿹��
// 1. �޼�
// 2. ������
// 3. �� => �б� �߰ݽ��� ����
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATTEACHER_API UKMK_PlayerRay : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKMK_PlayerRay();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma region ray variables
	FVector startPos;
	FVector endPos;
	UFUNCTION()
	void SetRayPos(FVector start, FVector end);
	UPROPERTY(EditAnywhere)
	float rayDis = 1000;

	class UKMK_PlayerHandFSM* FSM;
	class AKMK_Player* playerComp;
	UPROPERTY()
	bool isRay = false;
	float RayDistance =0;
	class UKHH_EnemyFSM* teacher;
	class UKMK_Bat* b;

	TArray<class AKMK_PlayerHand*> Hands;
	TArray<class AKMK_Battery*> Bats;
	UPROPERTY(EditAnywhere)
	float PDis = 300;
	
	bool isCome = false;

	int startPoint = -1;
#pragma endregion
		
};
