#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Editor.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Input/SEditableText.h"
#include "Editor/PropertyEditor/Private/SDetailSingleItemRow.h"
#include "Misc/Paths.h"
#include "Selection.h"
#include "FastInputManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnFastInputDetect, FString, PropertyName, UClass*, PropertyOwnerClass, UStruct*, PropertyOwnerStruct, UClass*, ObjectClass);

UCLASS()
class FASTINPUT_API UFastInputManager : public UObject
{
	GENERATED_BODY()
public:
	UFastInputManager();

public:
	UFUNCTION(BlueprintCallable)
	static UFastInputManager* Get();

	// 有关鼠标点击
public:
	void GetSDetailSingleItemRow(FSlateApplication& SlateApp, TSharedPtr<SDetailSingleItemRow>& OutDetailSingleItemRow);
	void GetAllPropertiesNameAndClass(TSharedPtr<SDetailSingleItemRow>& InDetailSingleItemRowPtr);

	// 有关FI界面提供函数
public:
	UFUNCTION(BlueprintCallable)
	TArray<FString> GetAllPropertyNames(UClass* InClass);
	UFUNCTION(BlueprintCallable)
	TArray<FString> GetAllSelections(FString PropertyName, UClass* PropertyOwnerClass, UStruct* PropertyOwnerStruct, UClass* ObjectClass);
	UFUNCTION(BlueprintCallable)
	UClass* GetSelectedActorClass();
	UFUNCTION(BlueprintCallable)
	void SetEditableText(FString InputString);
	UFUNCTION(BlueprintCallable)
	FString GetUClassName(UClass* PropertyOwnerClass);
	UFUNCTION(BlueprintCallable)
	FString GetUStructName(UStruct* PropertyOwnerStruct);
	// 有关Json
public:
	UFUNCTION(BlueprintCallable)
	FString FIGetJsonPath(FString PropertyName, UClass* Class, UStruct* Struct);
	TSharedPtr<FJsonObject> FIReadJson(const FString& FilePath);
	TSharedPtr<FJsonObject> FIMakeJson(UDataTable* DT);
	UFUNCTION(BlueprintCallable)
	bool FISaveJson(FString PropertyName, UClass* Class, UStruct* Struct, UClass* ObjectClass, UDataTable* DT);

public:
	UPROPERTY(BlueprintAssignable)
	FOnFastInputDetect OnFastInputDetect;

public:
	static UFastInputManager* self;
	SEditableText* EditableText;
};
