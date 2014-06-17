// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "UTTimedPowerup.generated.h"

UCLASS(Blueprintable, Abstract)
class AUTTimedPowerup : public AUTInventory
{
	GENERATED_UCLASS_BODY()

	/** time remaining (in defaults, initial lifetime on pickup)
	 * note: only ticks while held (not dropped on the ground)
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Powerup)
	float TimeRemaining;

	/** sound played on the current owner when the duration runs out */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Effects)
	USoundBase* PowerupOverSound;
	/** overlay material added to the player's weapon while this powerup is in effect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Effects)
	UMaterialInterface* OverlayMaterial;

	virtual void AddOverlayMaterials_Implementation(AUTGameState* GS) const OVERRIDE
	{
		GS->AddOverlayMaterial(OverlayMaterial);
	}

	UFUNCTION(Reliable, Client)
	void ClientSetTimeRemaining(float InTimeRemaining);

	UFUNCTION(BlueprintNativeEvent, BlueprintAuthorityOnly)
	void TimeExpired();

	virtual void Tick(float DeltaTime);

	virtual bool StackPickup_Implementation(AUTInventory* ContainedInv);

	virtual void GivenTo(AUTCharacter* NewOwner, bool bAutoActivate) OVERRIDE;
	virtual void Removed() OVERRIDE;
};