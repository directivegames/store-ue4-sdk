// Copyright 2023 Xsolla Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "XsollaWorldObject.generated.h"

class UUserWidget;

UCLASS(Blueprintable)
class XSOLLAUTILS_API UXsollaWorldObject : public UObject
{
	GENERATED_BODY()

public:
	virtual UWorld* GetWorld() const override;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"), Category = "Xsolla|World object")
	UUserWidget* Owner;
};
