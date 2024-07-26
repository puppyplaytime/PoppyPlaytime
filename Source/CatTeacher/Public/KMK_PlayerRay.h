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
	UPROPERTY(EditAnywhere)
	float rayDis = 1000;

	UFUNCTION()
	void SetStartEndPos(FVector start, FVector end);

	class UKMK_PlayerHandFSM* FSM;

	bool isRay = false;
#pragma endregion

		
};
