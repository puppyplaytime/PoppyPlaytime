// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JSH_Spawn.generated.h"

UCLASS()
class CATTEACHER_API AJSH_Spawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJSH_Spawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// �±׸� ã�� ������ �α׸� ����ϴ� �Լ�
	void CheckTagsAndLog();

	// Ÿ�̸� �ڵ�
	FTimerHandle TimerHandle1;
	FTimerHandle TimerHandle2;
	FTimerHandle TimerHandle3;
	FTimerHandle TimerHandle4;

	void DisplayMissingTag(FName MissingTag);

	// �±� üũ ����
	bool bCheckingFcat1 = false;
	bool bCheckingFcat2 = false;
	bool bCheckingFcat3 = false;
	bool bCheckingFcat4 = false;
};
