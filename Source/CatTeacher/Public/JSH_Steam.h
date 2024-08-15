// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "JSH_Steam.generated.h"


UCLASS()
class CATTEACHER_API AJSH_Steam : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJSH_Steam();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// ����
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* NSteam;

	// ���� on , off
	UPROPERTY(EditAnywhere)
	bool SteamON = false;

	// �ð�
	float currtime = 0;
	float offtime = 3;

	// sound ���� �ð�
	bool soundstart = true;
	float SoundOfftime = 10;
};
