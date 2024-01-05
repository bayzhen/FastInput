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

UCLASS(Blueprintable)
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
	TArray<FString> GetAllSelections(FString InPropertyName, UClass* InPropertyOwnerClass, UStruct* InPropertyOwnerStruct, UClass* InObjectClass);
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
	bool FISaveJson(FString InPropertyName, UClass* InClass, UStruct* InStruct, UClass* InObjectClass, UDataTable* DT);

public:
	static UFastInputManager* self;
	TWeakPtr<SEditableText> EditableTextWeakPtr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastInput")
	FString PropertyName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastInput")
	FString PropertyOwnerClassName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastInput")
	FString PropertyOwnerStructName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastInput")
	FString PropertyActorClassName;
};
