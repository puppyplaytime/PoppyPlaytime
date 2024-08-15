// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "JSH_Cat.generated.h"

UCLASS()
class CATTEACHER_API AJSH_Cat : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJSH_Cat();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// �� AI���� ������Ʈ Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=FSMcomponent)
	class UJSH_CatFSM* fsm;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* FalseBox;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	

	// �ִϸ��̼� �������� �����ϱ� ���� ����
	bool PlayAttackAnimation = false;

	bool timestart = false;
	float currtime = 0;
	float aniendtime = 1.2;


	// widget ������ ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WidgetPlay = false;

	UPROPERTY()
	class AJSH_Ending* EndingHelper;


	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> ccamera;


	// JumpScare���� Player�� ���� ���ֱ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HandDel = false;
};
