// Copyright 2023 Xsolla Inc. All Rights Reserved.

#pragma once

#include "Components/CheckBox.h"
#include "Http.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XsollaUtilsDataModel.h"
#include "XsollaUtilsLibrary.generated.h"

class UXsollaUtilsImageLoader;
class FJsonObject;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCustomRequestSuccess, const FString&, Data);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnCustomRequestError, const FString&, Code, const FString&, Description);

UCLASS()
class XSOLLAUTILS_API UXsollaUtilsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	static void Internal_AddParametersToJsonObject(TSharedPtr<FJsonObject> JsonObject,
		const FXsollaParameters& CustomParameters, const FString& FieldName = "");

public:
	/** Direct access to the image loader object. */
	UFUNCTION(BlueprintPure, Category = "Xsolla|Utils")
	static UXsollaUtilsImageLoader* GetImageLoader();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Xsolla|Utils")
	static void GetDefaultObject(TSubclassOf<UObject> ObjectClass, UObject*& DefaultObj);

	/** Make FDateTime structure based on a given timestamp. */
	UFUNCTION(BlueprintPure, Category = "Xsolla|Utils")
	static FDateTime MakeDateTimeFromTimestamp(const int64 Time);

	/** Get seconds from DateTime to UnixTimestamp. */
	UFUNCTION(BlueprintPure, Category = "Xsolla|Utils")
	static int64 GetSecondsFromUnixTimestamp(const FDateTime& DateTime);


	template <typename TEnum>
	static FString EnumToString(TEnum EnumValue)
	{
		return StaticEnum<TEnum>()->GetNameStringByIndex((int32)EnumValue);
	}

	template <typename TEnum>
	static FString GetEnumValueAsDisplayNameString(TEnum EnumValue)
	{
		return StaticEnum<TEnum>()->GetDisplayNameTextByIndex((int32)EnumValue).ToString();
	}

	template <typename TEnum>
	static TEnum GetEnumValueFromString(const FString& String)
	{
		return static_cast<TEnum>(StaticEnum<TEnum>()->GetValueByName(FName(*String)));
	}

	/** Add parameters to json object root */
	static void AddParametersToJsonObject(TSharedPtr<FJsonObject> JsonObject, const FXsollaParameters& CustomParameters);

	/** Add parameters to json object by FieldName */
	static void AddParametersToJsonObjectByFieldName(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, const FXsollaParameters& CustomParameters);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToXsollaJsonVariant (int)", CompactNodeTitle = "->", BlueprintAutocast), Category = "Xsolla|Utils")
	static FXsollaJsonVariant Conv_IntToXsollaJsonVariant(int Value);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToXsollaJsonVariant (float)", CompactNodeTitle = "->", BlueprintAutocast), Category = "Xsolla|Utils")
	static FXsollaJsonVariant Conv_FloatToXsollaJsonVariant(float Value);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToXsollaJsonVariant (string)", CompactNodeTitle = "->", BlueprintAutocast), Category = "Xsolla|Utils")
	static FXsollaJsonVariant Conv_StringToXsollaJsonVariant(FString Value);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToXsollaJsonVariant (bool)", CompactNodeTitle = "->", BlueprintAutocast), Category = "Xsolla|Utils")
	static FXsollaJsonVariant Conv_BoolToXsollaJsonVariant(bool Value);

	/** Set additional information for web request. */
	UFUNCTION(BlueprintCallable, Category = "Xsolla|Utils")
	static void SetPartnerInfo(const FString& Referral, const FString& ReferralVersion);

	static void GetPartnerInfo(FString& Referral, FString& ReferralVersion);

	/** Encodes the request body to match x-www-form-urlencoded data format. */
	static FString EncodeFormData(TSharedPtr<FJsonObject> FormDataJson);

	static FString GetPluginName(const FName& ModuleName);

	/** Returns URL query parameter with specified name. */
	static FString GetUrlParameter(const FString& Url, const FString& ParamName);

	UFUNCTION(BlueprintCallable, Category = "Xsolla|Utils", meta = (AutoCreateRefTerm = "SuccessCallback, ErrorCallback"))
	static void CreateCustomRequest(const FString& Url, const FString& Verb, const FOnCustomRequestSuccess& SuccessCallback, const FOnCustomRequestError& ErrorCallback);

	static void CreateCustomRequest_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded,
		FOnCustomRequestSuccess SuccessCallback, FOnCustomRequestError ErrorCallback);

	UFUNCTION(BlueprintCallable, Category = "Xsolla|Utils")
	static bool GetJsonStringField(const FString& data, const FString& key, FString& iValue);

	UFUNCTION(BlueprintCallable, Category = "Xsolla|Utils")
	static bool GetJsonIntField(const FString& data, const FString& key, int& iValue);

private:
	static FString XReferral;
	static FString XReferralVersion;
};
