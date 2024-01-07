// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "FastInputManager.h"
#include "Framework/Application/IInputProcessor.h"


// Custom input listener class
class FFastInputCustomInputListener : public IInputProcessor
{
public:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApplication, TSharedRef<ICursor> Cursor) override {}
	virtual bool HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) { 
		UE_LOG(LogTemp, Warning, TEXT("markov HandleMouseButtonUpEvent"));
		UFastInputManager* FastInputSubsystem = UFastInputManager::GetCppInstance();
		TSharedPtr<SDetailSingleItemRow> DetailSingleItemRowPtr;
		FastInputSubsystem->GetSDetailSingleItemRow(SlateApp, DetailSingleItemRowPtr);
		FastInputSubsystem->GetAllPropertiesNameAndClass(DetailSingleItemRowPtr);
		return false; 
	}
};