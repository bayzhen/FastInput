// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "FastInputManager.h"
#include "FastInputEditorStyle.h"
#include "LevelEditor.h"

class FFastInputModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
public:
	void AddToolbarExtension(FToolBarBuilder& Builder);
	UObject* FastInputManager;
	void MyButtonClicked() {
		UFastInputManager::GetCppInstance()->RunEditorWidgetWindow();
	}
private:
	TSharedPtr<FExtender> ToolbarExtender;	
};
