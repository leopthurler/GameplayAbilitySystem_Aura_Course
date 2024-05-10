// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/AuraPlayerController.h"

#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(LocalPlayerSubsystem)
	{
		LocalPlayerSubsystem->AddMappingContext(AuraContext, 0);
	}
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if(!CursorHit.bBlockingHit)
		return;
	
	LastCursorActor = ThisCursorActor;
	ThisCursorActor = CursorHit.GetActor();

	/*
	* Line trace from cursor. There are several scenarios:
	* A. LastCursorActor is null && ThisCursorActor is null
	*		- Do nothing
	* B. LastCursorActor is null && ThisCursorActor is valid
	*		- Highlight ThisCursorActor
	* C. LastCursorActor is valid && ThisCursorActor is null
	*		- UnHighlight LastActor
	* D. Both actors are valid, but LastCursorActor != ThisCursorActor
	*		- UnHighlight LastCursorActor and Highlight ThisCursorActor
	* E. Both actors are valid, and are the same actor
	*		- Do nothing
	*/

	if(LastCursorActor == nullptr)
	{
		if(ThisCursorActor != nullptr)
		{
			//Case B
			ThisCursorActor->HighlightActor();
		}
		else
		{
			//Case A
		}
	}
	else // LastCursorActor is valid
	{
		if(ThisCursorActor == nullptr)
		{
			//Case C
			LastCursorActor->UnHighlightActor();
		}
		else
		{
			if(LastCursorActor != ThisCursorActor)
			{
				//Case D
				LastCursorActor->UnHighlightActor();
				ThisCursorActor->HighlightActor();
			}
			else
			{
				//Case E
			}
		}
	}
}