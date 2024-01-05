// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "FastInputSubsystem.h"
#include "Framework/Application/IInputProcessor.h"


// Custom input listener class
class FFastInputCustomInputListener : public IInputProcessor
{
public:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApplication, TSharedRef<ICursor> Cursor) override {}

	//virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	//{
	//	TSharedPtr<SWidget> FocusedWidget = SlateApp.GetUserFocusedWidget(0);
	//	
	//	if (FocusedWidget.IsValid())
	//	{
	//		TSharedPtr<SEditableText> EditableTextWidget = StaticCastSharedPtr<SEditableText>(FocusedWidget);
	//		auto p = EditableTextWidget->GetParentWidget();
	//		if (EditableTextWidget.IsValid())
	//		{
	//			// Process input event for the focused SEditableText widget
	//			// You can replace the input text or add any other desired behavior here
	//			UE_LOG(LogTemp, Warning, TEXT("markov HandleKeyDownEvent %s %s"), *FocusedWidget->GetType().ToString(), *FocusedWidget->GetTag().ToString());
	//		}
	//	}
	//	return false;
	//}

	//virtual bool HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override {
	//	UE_LOG(LogTemp, Warning, TEXT("markov HandleKeyUpEvent"));
	//	return false;
	//}
	//virtual bool HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent) override {
	//	UE_LOG(LogTemp, Warning, TEXT("markov HandleAnalogInputEvent"));
	//	return false;
	//}
	///** Mouse button press */
	//virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) {
	//	UE_LOG(LogTemp, Warning, TEXT("markov HandleMouseButtonDownEvent"));
 //       //GetSelectedActorAndPropertyBeingEdit();
	//	return false;
	//}

	virtual bool HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) { 
		UE_LOG(LogTemp, Warning, TEXT("markov HandleMouseButtonUpEvent"));
		UFastInputSubsystem* FastInputSubsystem = UFastInputSubsystem::Get();
		TSharedPtr<SDetailSingleItemRow> DetailSingleItemRowPtr;
		FastInputSubsystem->GetSDetailSingleItemRow(SlateApp, DetailSingleItemRowPtr);
		FastInputSubsystem->GetAllPropertiesNameAndClass(DetailSingleItemRowPtr);
		return false; 
	}
};