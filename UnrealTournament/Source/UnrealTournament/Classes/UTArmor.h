// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "UTArmor.generated.h"

UCLASS(Blueprintable, Abstract, notplaceable, meta = (ChildCanTick))
class AUTArmor : public AUTInventory
{
	GENERATED_UCLASS_BODY()

	/** armor amount remaining */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Armor)
	int32 ArmorAmount;
	/** percentage of incoming damage redirected to armor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Armor, Meta = (UIMin = 0.0, UIMax = 1.0))
	float AbsorptionPct;
	/** whether to also absorb momentum by the same percentage that damage is absorbed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Armor)
	bool bAbsorbMomentum;
	/** character overlay applied while this armor is equipped */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Effects)
	UMaterialInterface* OverlayMaterial;

	virtual void AddOverlayMaterials_Implementation(AUTGameState* GS) const OVERRIDE
	{
		GS->AddOverlayMaterial(OverlayMaterial);
	}

	virtual void GivenTo(AUTCharacter* NewOwner, bool bAutoActivate) OVERRIDE;
	virtual void Removed() OVERRIDE;

	virtual void ModifyDamageTaken_Implementation(int32& Damage, FVector& Momentum, const FDamageEvent& DamageEvent, AController* InstigatedBy, AActor* DamageCauser);

	virtual bool StackPickup_Implementation(AUTInventory* ContainedInv);
};