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
	void GetAllSelections();
	UFUNCTION(BlueprintCallable)
	static UClass* GetSelectedActorClass();
	UFUNCTION(BlueprintCallable)
	void SetEditableText(FString InputString);
	UFUNCTION(BlueprintCallable)
	static FString GetUClassName(UClass* PropertyOwnerClass);
	UFUNCTION(BlueprintCallable)
	static FString GetUStructName(UStruct* PropertyOwnerStruct);
public:
	// 触发FI界面事件
	UFUNCTION(BlueprintCallable)
	void TriggerEUWEvent(FString EventName);
public:
	// 有关Json
	UFUNCTION(BlueprintCallable)
	FString FIGetJsonPath();
	UFUNCTION(BlueprintCallable)
	void FIReadJson();
	UFUNCTION(BlueprintCallable)
	bool FISaveJson();
private:
	TSharedPtr<FJsonObject> FIMakeJson();

public:
	static UFastInputManager* self;
	TSharedPtr<SEditableText> EditableTextSharedPtr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastInput")
	FString PropertyName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastInput")
	UClass* PropertyOwnerClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastInput")
	UStruct* PropertyOwnerStruct;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastInput")
	UClass* PropertyActorClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastInput")
	FString DTRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastInput")
	FString ColumnName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastInput")
	TArray<FString> Selections;
};
