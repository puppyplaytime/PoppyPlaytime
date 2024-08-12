// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JSH_Ending.generated.h"

UCLASS()
class CATTEACHER_API AJSH_Ending : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJSH_Ending();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* FalseBox;


	class AJSH_CatDoor* CatDoor;

	
	UPROPERTY()
	class AJSH_EndingCat* me;



	class AJSH_Cat* cat1;
	
	class AJSH_Cat* cat2;

	class AJSH_Cat* cat3;
	
	class AJSH_Cat* cat4;

	class AJSH_Cat* cat5;
};
