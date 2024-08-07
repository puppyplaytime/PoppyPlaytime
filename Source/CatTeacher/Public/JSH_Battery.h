// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JSH_Battery.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATTEACHER_API UJSH_Battery : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJSH_Battery();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	TArray<class AActor*> Steam;
	
	class UKMK_Bat* BatComponent03;
	class UKMK_Bat* BatComponent04;
	

	AActor* batsave03;
	AActor* batsave04;

	bool FSMOnOff03 = true;
	bool FSMOnOff04 = true;

	void FindTaggedActors();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool BPNiagaraOnOff = true;


	
	
};
