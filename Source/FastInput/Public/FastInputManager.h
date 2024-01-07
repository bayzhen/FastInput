#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Editor.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Input/SEditableText.h"
#include "Editor/PropertyEditor/Private/SDetailSingleItemRow.h"
#include "Misc/Paths.h"
#include "Selection.h"
#include "Kismet/GameplayStatics.h"
#include "EditorUtilityWidget.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "Editor.h"
#include "InputCoreTypes.h"
#include "LevelEditor.h"
#include "FastInputManager.generated.h"

UCLASS(Blueprintable)
class FASTINPUT_API UFastInputManager : public UObject
{
	GENERATED_BODY()
public:
	UFastInputManager();

public:
	UFUNCTION(BlueprintCallable)
	static UFastInputManager* GetCppInstance();

public:
	// 有关鼠标点击
	void GetSDetailSingleItemRow(FSlateApplication& SlateApp, TSharedPtr<SDetailSingleItemRow>& OutDetailSingleItemRow);
	void GetAllPropertiesNameAndClass(TSharedPtr<SDetailSingleItemRow>& InDetailSingleItemRowPtr);

public:
	// 有关FI界面提供函数
	UFUNCTION(BlueprintCallable)
	TArray<FString> GetAllPropertyNames(UClass* InClass);
	UFUNCTION(BlueprintCallable)
	void GetSelections();
	UFUNCTION(BlueprintCallable)
	static UClass* GetSelectedActorClass();
	UFUNCTION(BlueprintCallable)
	void SetEditableText(FString InputString);
	UFUNCTION(BlueprintCallable)
	static FString GetUClassName(UClass* PropertyOwnerClass);
	UFUNCTION(BlueprintCallable)
	static FString GetUStructName(UStruct* PropertyOwnerStruct);
public:
	// 用户覆写函数
	// 属性名 + 类名 + 对象类名 → 其它属性名 → 其它属性选项 → 最终选项
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FastInput")
	void UpdateCustomSelections();
public:
	// 触发FI界面事件
	void SpawnEditorWidgetWindow();
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
	static UFastInputManager* FastInputManagerCppInstance;
	static UObject* FastInputManagerBPInstance;
	static TWeakPtr<SWindow> EditorWidgetWindow;

	TSharedPtr<SEditableText> EditableTextSharedPtr;
	FProperty* PropertyPtr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastInput")
	FString PropertyName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastInput")
	UClass* PropertyOwnerClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastInput")
	UStruct* PropertyOwnerStruct;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastInput")
	FString PropertyOwnerStructName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastInput")
	UClass* PropertyActorClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastInput")
	FString DTRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastInput")
	FString ColumnName;
	// 从DT等资源中读出的选项
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastInput")
	TArray<FString> Selections;
	// 由自定义函数得出的选项
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FastInput")
	TArray<FString> CustomSelections;

};
