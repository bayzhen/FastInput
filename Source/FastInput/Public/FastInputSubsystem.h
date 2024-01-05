#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Editor.h"
#include "Editor/UnrealEdEngine.h"
#include "UnrealEdGlobals.h"

#include "Subsystems/Subsystem.h"
#include "Delegates/Delegate.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Input/SEditableText.h"
#include "Editor/PropertyEditor/Private/SDetailSingleItemRow.h"
#include "Misc/Paths.h"
#include "Selection.h"
#include "FastInputSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnFastInputDetect, FString, PropertyName, UClass*, PropertyOwnerClass, UStruct*, PropertyOwnerStruct);

UCLASS()
class FASTINPUT_API UFastInputSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	UFUNCTION(BlueprintCallable)
	static UFastInputSubsystem* Get();

// 有关鼠标点击
public:
	void GetSDetailSingleItemRow(FSlateApplication& SlateApp, TSharedPtr<SDetailSingleItemRow>& OutDetailSingleItemRow);
	void GetAllPropertiesNameAndClass(TSharedPtr<SDetailSingleItemRow>& InDetailSingleItemRowPtr);

// 有关FI界面提供函数
public:
	UFUNCTION(BlueprintCallable)
	TArray<FString> GetAllPropertyNames(UClass* InClass);
	UFUNCTION(BlueprintCallable)
	TArray<FString> GetAllSelections(FString PropertyName, UClass* Class, UStruct* Struct, UClass* ObjectClass);
	UFUNCTION(BlueprintCallable)
	UClass* GetSelectedActorClass();
	UFUNCTION(BlueprintCallable)
	void SetEditableText(FString InputString);
// 有关Json
public:
	FString FIGetJsonPath(FString PropertyName, UClass* Class, UStruct* Struct);
	TSharedPtr<FJsonObject> FIReadJson(const FString& FilePath);
	TSharedPtr<FJsonObject> FIMakeJson(UDataTable* DT);
	bool FISaveJson(FString PropertyName, UClass* Class, UStruct* Struct, UClass* ObjectClass, UDataTable* DT);

public:
	UPROPERTY(BlueprintAssignable, Category = "MLBridge")
	FOnFastInputDetect OnFastInputDetect;

private:
	static UFastInputSubsystem* self;
	TSharedPtr<SEditableText> EditingEditableTextSharedPtr;
};
