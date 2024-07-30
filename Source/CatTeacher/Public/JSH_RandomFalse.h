// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JSH_RandomFalse.generated.h"

UCLASS()
class CATTEACHER_API AJSH_RandomFalse : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJSH_RandomFalse();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Ÿ�̸� �ڵ�
	FTimerHandle TimerHandle;

	// �±׸� ã�� �������� �����ϴ� �Լ�
	void FindAndSelectRandomTag();

	
};
