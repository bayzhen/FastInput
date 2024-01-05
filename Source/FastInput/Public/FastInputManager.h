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

UCLASS()
class FASTINPUT_API UFastInputManager : public UObject
{
	GENERATED_BODY()
public:
	UFastInputManager();

public:
	UFUNCTION(BlueprintCallable)
	static UFastInputManager* Get();

public:
	// 有关鼠标点击
	void GetSDetailSingleItemRow(FSlateApplication& SlateApp, TSharedPtr<SDetailSingleItemRow>& OutDetailSingleItemRow);
	void GetAllPropertiesNameAndClass(TSharedPtr<SDetailSingleItemRow>& InDetailSingleItemRowPtr);

public:
	// 有关FI界面提供函数
	UFUNCTION(BlueprintCallable)
	TArray<FString> GetAllPropertyNames(UClass* InClass);
	UFUNCTION(BlueprintCallable)
	TArray<FString> GetAllSelections(FString PropertyName, UClass* PropertyOwnerClass, UStruct* PropertyOwnerStruct, UClass* ObjectClass);
	UFUNCTION(BlueprintCallable)
	static UClass* GetSelectedActorClass();
	UFUNCTION(BlueprintCallable)
	void SetEditableText(FString InputString);
	UFUNCTION(BlueprintCallable)
	static FString GetUClassName(UClass* PropertyOwnerClass);
	UFUNCTION(BlueprintCallable)
	static FString GetUStructName(UStruct* PropertyOwnerStruct);
public:
	// 有关Json
	UFUNCTION(BlueprintCallable)
	FString FIGetJsonPath(FString PropertyName, UClass* Class, UStruct* Struct);
	TSharedPtr<FJsonObject> FIReadJson(const FString& FilePath);
	TSharedPtr<FJsonObject> FIMakeJson(UDataTable* DT);
	UFUNCTION(BlueprintCallable)
	bool FISaveJson(FString PropertyName, UClass* Class, UStruct* Struct, UClass* ObjectClass, UDataTable* DT);

public:
	static UFastInputManager* self;
	TSharedPtr<SEditableText> EditableTextSharedPtr;
};
