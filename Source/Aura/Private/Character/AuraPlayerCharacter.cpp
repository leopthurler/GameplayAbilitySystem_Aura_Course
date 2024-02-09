// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraPlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

AAuraPlayerCharacter::AAuraPlayerCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0,400,0);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}
