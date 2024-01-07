// Copyright Epic Games, Inc. All Rights Reserved.

#include "FastInput.h"
#include "FastInputCustomInputListener.h"

#define LOCTEXT_NAMESPACE "FFastInputModule"

void FFastInputModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FSlateApplication& SlateApplication = FSlateApplication::Get();
	SlateApplication.RegisterInputPreProcessor(MakeShareable(new FFastInputCustomInputListener()));
	

	TSubclassOf<UObject> BlueprintClass;
	UClass* BlueprintAsset;


	// Specify the Blueprint asset path
	FString BlueprintPath = TEXT("Blueprint'/FastInput/BP_FastInputManager.BP_FastInputManager_C'");

	// Load the Blueprint asset using the LoadObject function
	BlueprintAsset = LoadObject<UClass>(nullptr, *BlueprintPath);

	if (BlueprintAsset)
	{
		BlueprintClass = BlueprintAsset;
		// You can now use the BlueprintClass to spawn objects or perform other operations
		FastInputManager = NewObject<UObject>(GetTransientPackage(), BlueprintClass);
		UFastInputManager::FastInputManagerCppInstance = Cast<UFastInputManager>(FastInputManager);
		FastInputManager->AddToRoot();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to load Blueprint asset '%s'"), *BlueprintPath);
	}

	FSlateStyleRegistry::RegisterSlateStyle(*FFastInputEditorStyle::Get());

	// Register the extension in the level editor toolbar
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension("Content", EExtensionHook::After, nullptr, FToolBarExtensionDelegate::CreateRaw(this, &FFastInputModule::AddToolbarExtension));
	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);

}

void FFastInputModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FSlateApplication& SlateApplication = FSlateApplication::Get();
	FastInputManager->RemoveFromRoot();
}

void FFastInputModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(
		FUIAction(FExecuteAction::CreateRaw(this, &FFastInputModule::MyButtonClicked)),
		NAME_None,
		FText::FromString("My Button"),
		FText::FromString("Click to perform an action"),
		FSlateIcon(FFastInputEditorStyle::Get()->GetStyleSetName(), "FastInputEditorStyle.FastInputIcon"),
		EUserInterfaceActionType::Button,
		NAME_None
	);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFastInputModule, FastInput)