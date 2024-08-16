// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_FinalSwitch.h"

#include "JSH_Random.h"
#include "KMK_Bat.h"
#include "KMK_Player.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"

// Sets default values for this component's properties
UKMK_FinalSwitch::UKMK_FinalSwitch()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// begin Play�� �Ű澲�� ������ �⺻ ���õ� �س��� �� => �ֺ� ���ڶ� ������ ����ġ�� �����ϱ� ���� �͵��̿��� �Ű� �Ƚᵵ��
 
// UKMK_FinalSwitch�� ���� 5���� Actor�� �޾Ƽ� ó�����Դϴ�
// (�ε��� ����) 0������ 3�������� ���� Actor�� ������ �����̰� 4���� ��ǻ�� ��ܿ� �ִ� ����ġ ���õ� ģ���Դϴ�.

// ���� ����
// ���⿡ �ִ� bats = ������ �̾߱��� 5���� Actor�� �޾ƿ��� ���� => ���� �Ҵ�
// batsComps�� ���ڿ� component�� �޷��ִ� UKMK_Bat�� �޾ƿ��� ����
// UKMK_Bat ���ο� �ִ� isHaveBat(���͸��� ������ �ִ��� Ȯ���ϴ� ����)�� Ȯ���ϰ� ���� ������Ʈ�� �ִ� ChargeSpeed(���ڸ��� �޸� ���α׷�����) ������ ���� ��ǻ�� ���α׷����ٸ� ä��� �����Դϴ�

void UKMK_FinalSwitch::BeginPlay()
{
	Super::BeginPlay();
	bats[4]->SetActorEnableCollision(false);
	for(int i = 0; i < 4; i++)
	{
		batsComps.Add(bats[i]->FindComponentByClass<UKMK_Bat>());
	}
	meshComp = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	myMatDynamic = UMaterialInstanceDynamic::Create(matFact, this);
	if (matFact != nullptr && meshComp != nullptr) meshComp->SetMaterial(0, myMatDynamic);
	myMatDynamic->SetScalarParameterValue("Page", 0);
	myMatDynamic->SetScalarParameterValue("Gage", 100);

	// CatNab RandomTag 
	RandomTag = Cast<AJSH_Random>(UGameplayStatics::GetActorOfClass(GetWorld(), AJSH_Random::StaticClass()));
}


// Called every frame
void UKMK_FinalSwitch::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto* player = Cast<AKMK_Player>(GetWorld()->GetFirstPlayerController()->GetPawn());
	float d = FVector::Distance(player->GetActorLocation(), GetOwner()->GetTargetLocation());
	//GEngine->AddOnScreenDebugMessage(5, 1, FColor::White, FString::Printf(TEXT("%f"), d));
	// Ĺ�� ���� ��, ����� �ȳ� ���� ���� if �� => PlayerDist�� ���� ����Ϳ� �÷��̾��� ���� ���� ������ 
	// isTrue = true�� ��� Ĺ�� fsm�� ���۵Ǿ����
	if (!isTrue && d < playerDist)
	{
		// �����ð��� ������
		t += DeltaTime;
		if (t > 5)
		{
			// ��ũ�� �������� ����ǰ�
			myMatDynamic->SetScalarParameterValue("Page", 1);
			if (t > 3.f)
			{
				// ��ũ���� �������� 0���� �ʱ�ȭ��
				t = 0;
				isTrue = true;
				myMatDynamic->SetScalarParameterValue("Gage", 0);

				// CatNab ���� ���¸� �������ִ� ���� ����
				RandomTag->start = true;
				UE_LOG(LogTemp, Warning, TEXT("FSM Start"));
			}
		}
	}
	// ������ ����ġ�� ����� ���ڵ��� Ĺ�� ����� ���۵ɶ� �����ϱ� ���� ���� ������
	if (isTrue)
	{
		for (int i = 0; i < 4; i++)
		{
			batsComps[i]->isStart = true;
		}
	}
	if(!isTrue) return;
	// Ĺ�� ����� ���۵Ǵ� �κ�
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, FString::Printf(TEXT("%f"), allCharge));
	//GEngine->AddOnScreenDebugMessage(6, 1, FColor::Yellow, FString::Printf(TEXT("%d"), count));
	// 4�� ���ڿ��� ��� �������� ��� ���
	if (allCharge > 100 || (player != nullptr && player->isCheat2))
	{
		allCharge = 100;
		// ��ǻ�� ��� ����ġ�� �޸� collision�� ����
		bats[4]->SetActorEnableCollision(true);
	}
	// �������� ��� �ִٸ�
	else
	{
		/* Material instance�� ���, Page(������ ����)�� Gage(���α׷�����) 2������ ������ ������ 
			=> �ش� Material instance�� 8/12�� TA�ʿ��� ���� �÷����ٰ� ��
			=> ��ġ ��δ� ���� Ȯ���غ��� �˴ϴ� */
		// ��� ���α׷��� �ٿ� �ִ� material instance�� ���� �� gage �κ��� allCharge(���ڿ��� ������ ��)�� �����ϰڴ�
		myMatDynamic->SetScalarParameterValue("Gage", allCharge);
		
		// 4������ ���� ���¸� Ȯ���ϱ� ���� for��
		for (int i = 0; i < 4; i++)
		{
			// ���ڿ� ���� component(KMK_Bat) ������ isHaveBat�� ���� ���ڿ� ���͸��� �ִ��� Ȯ����
			// ���͸��� �ִ� ���
			if (batsComps[i]->isHaveBat && !isHave[i])
			{
				count++;
				isHave[i] = true;
				
			}
			// ���͸��� ���� ��� 
			if (!batsComps[i]->isHaveBat && isHave[i])
			{
				count--;
				isHave[i] = false;
			}

		}

		// ���͸��� 1���ﵵ �����ϸ�
		if (count != 0)
		{
			if (allCharge >= 25)
			{
				batsComps[1]->ChargeGage(batsComps[1]->ChargeSpeed, count + 1);
			}
			// ���͸��� ��� �������� ��ǻ�Ϳ� �����Ǹ� ���ڿ� ���� ���α׷����ٰ� �ö󰡰� ����� ����
			if (allCharge >= 50)
			{
				batsComps[2]->ChargeGage(batsComps[2]->ChargeSpeed, count + 2);
			}
			if (allCharge >= 75)
			{
				batsComps[3]->ChargeGage(batsComps[3]->ChargeSpeed, count + 3);
			}
			// ��ǻ�� ������ �ִ� ���α׷��� ���� ���� ��������
			allCharge += batsComps[0]->ChargeSpeed * 0.25f * count;
		}
	}
}

