// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_TopCatFSM.h"

#include "JSH_Cat.h"
#include "JSH_CatDoor.h"
#include "KHH_RotateDoor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UJSH_TopCatFSM::UJSH_TopCatFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UJSH_TopCatFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...

	// 체크용으로 top 해둔거임
	tState = TCatState::TopOpen;
	DoorOpen = false;
	topStart = true;
	// -----------------------------


	

	CatDoor = Cast<AJSH_CatDoor>(UGameplayStatics::GetActorOfClass(GetWorld(), AJSH_CatDoor::StaticClass()));

	if (CatDoor)
	{
		// 'Door'라는 이름으로 액터의 참조를 저장합니다.
		FName DoorName = "Door";
		CatDoor->SetActorLabel(DoorName.ToString());
	}

	if (AttackStart)
	{
		tState = TCatState::TopOpen;
	}
	
}


// Called every frame
void UJSH_TopCatFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (tState)
	{
	case TCatState::Idle:
		IdleState(DeltaTime);
		break;
		
	case TCatState::TopOpen:
		TopOpenState(DeltaTime);
		break;
		
	case TCatState::Attack:
		AttackState();
		break;
	}

	// if (DoorOpen)
	// {
	// 	tState = TCatState::TopOpen;
	// 	DoorOpen = false;
	// 	topStart = true;
	// }
	

	FString myState = UEnum::GetValueAsString(tState);
	DrawDebugString(GetWorld() , GetOwner()->GetActorLocation(), myState , nullptr , FColor::Blue , 0, true, 1);
}

void UJSH_TopCatFSM::IdleState(float DeltaTime)
{
}

void UJSH_TopCatFSM::TopOpenState(float DeltaTime)
{
	currtime += DeltaTime;

	if (topStart)
	{
		CatDoor->fsm->isOpen = true;
		topStart = false;
	}
	
	if (CatDoor->fsm->isOpen)
	{
		if (currtime >= toptime)
		{
			tState = TCatState::Attack;
			bHasAttacked = true;
			currtime = 0;
			CatDoor->fsm->isOpen = false;
		}
	}
	else if (CatDoor->fsm->isOpen == false)
	{
		tState = TCatState::Idle;
		currtime = 0;
	}
	// bHasAttacked = true;
	// tState = TCatState::Attack;
}

void UJSH_TopCatFSM::AttackState()
{
    if (bHasAttacked)
    {
    	bHasAttacked = false;
        // Get me's location and store it in tt
    	if (TopCat)
    	{
    		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(TopCat);
    		
    		if (SpawnedActor)
    		{
    			// Set the actor to be hidden in the game
    			SpawnedActor->SetActorHiddenInGame(true);

    			// Optionally, disable collision if you don't want the actor to interact with other objects
    			SpawnedActor->SetActorEnableCollision(false);

    			// Get the location if needed
    			tt = SpawnedActor->GetActorLocation();
    			// Now you can use 'tt' as the location of the spawned actor
    		}
    	}
    	
        // Destroy me
        //me->fsm->BatCatStop();
        
        // Get the player controller
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PlayerController)
        {
            // Get the player's pawn (character or any actor controlled by the player)
            APawn* PlayerPawn = PlayerController->GetPawn();
            if (PlayerPawn)
            {
                // Calculate the direction vector from the player to tt
                FVector Direction = tt - PlayerPawn->GetActorLocation();
                Direction.Z = 0; // Keep the camera level, ignore the height difference
                
                // Calculate the new rotation for the camera to look at tt
                FRotator NewControlRotation = Direction.Rotation();
                
                // Adjust the Pitch to make the camera look slightly upwards (e.g., +15 degrees)
                NewControlRotation.Pitch += 23.0f;

                // Set the player's control rotation to the new rotation
                PlayerController->SetControlRotation(NewControlRotation);
 
                // Disable the player's ability to control the camera
                PlayerController->SetIgnoreLookInput(true);

                // Disable the player's ability to move
                PlayerController->SetIgnoreMoveInput(true);

                // Find all actors with the tag "FcatAttack"
                TArray<AActor*> FoundActors;
                UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("FcatAttack"), FoundActors);

                for (AActor* Actor : FoundActors)
                {
                    // Calculate the position 1 cm in front of the player's eyes
                    FVector EyeLocation;
                    FRotator EyeRotation;
                    PlayerPawn->GetActorEyesViewPoint(EyeLocation, EyeRotation);
                    FVector NewLocation = EyeLocation + EyeRotation.Vector() * 1.0f;

                    // Set the actor's location
                    Actor->SetActorLocation(NewLocation);

                    // Calculate the new rotation to look in the opposite direction of the player's view
                    FRotator OppositeRotation = EyeRotation;
                    OppositeRotation.Yaw += 180.0f; // Rotate 180 degrees around the Z axis (Yaw)

                    // Set the actor's rotation
                    Actor->SetActorRotation(OppositeRotation);

                    AJSH_Cat* atcat = Cast<AJSH_Cat>(Actor);

                	
                    atcat->PlayAttackAnimation = true;  // --->> cat tick
                }
            }
            //me->PlayAttackAnimation = true;
        }
    }
}

