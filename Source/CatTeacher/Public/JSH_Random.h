// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JSH_Random.generated.h"

UCLASS()
class CATTEACHER_API AJSH_Random : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJSH_Random();

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
