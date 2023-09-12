// Copyright 2023 Xsolla Inc. All Rights Reserved.

#include "XsollaStoreLibrary.h"
#include "XsollaStore.h"
#include "XsollaStoreCurrencyFormat.h"
#include "XsollaStoreDataModel.h"
#include "XsollaUtilsLibrary.h"
#include "XsollaStoreDefines.h"
#include "Engine/DataTable.h"
#include "Kismet/KismetTextLibrary.h"
#include "UObject/ConstructorHelpers.h"
#if 1 // WITH_DIRECTIVE
#include "Misc/EngineVersionComparison.h"
#endif

UDataTable* UXsollaStoreLibrary::CurrencyLibrary;

UXsollaStoreLibrary::UXsollaStoreLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> CurrencyLibraryObj(*FString::Printf(TEXT("DataTable'/%s/Misc/currency-format.currency-format'"),
		*UXsollaUtilsLibrary::GetPluginName(FXsollaStoreModule::ModuleName)));
	CurrencyLibrary = CurrencyLibraryObj.Object;
}

bool UXsollaStoreLibrary::Equal_StoreCartStoreCart(const FStoreCart& A, const FStoreCart& B)
{
	return A == B;
}

UDataTable* UXsollaStoreLibrary::GetCurrencyLibrary()
{
	return CurrencyLibrary;
}

FString UXsollaStoreLibrary::FormatPrice(float Amount, const FString& Currency /*= TEXT("USD")*/)
{
	if (Currency.IsEmpty())
	{
		return FString();
	}

	const FXsollaStoreCurrency* Row = GetCurrencyLibrary()->FindRow<FXsollaStoreCurrency>(FName(*Currency), FString());
	if (Row)
	{
#if UE_VERSION_OLDER_THAN(5, 1, 0) // WITH_DIRECTIVE
		const FString SanitizedAmount = UKismetTextLibrary::Conv_FloatToText(Amount, ERoundingMode::HalfToEven,
			false, true, 1, 324, Row->fractionSize, Row->fractionSize)
											.ToString();
#else
		const FString SanitizedAmount = UKismetTextLibrary::Conv_DoubleToText(Amount, ERoundingMode::HalfToEven,
			false, true, 1, 324, Row->fractionSize, Row->fractionSize)
											.ToString();
#endif
		return Row->symbol.format.Replace(TEXT("$"), *Row->symbol.grapheme).Replace(TEXT("1"), *SanitizedAmount);
	}

	UE_LOG(LogXsollaStore, Error, TEXT("%s: Failed to format price (%d %s)"), *VA_FUNC_LINE, Amount, *Currency);
	return FString();
}
