// Copyright Epic Games, Inc. All Rights Reserved.

#include "FastInput.h"
#include "FastInputCustomInputListener.h"

#define LOCTEXT_NAMESPACE "FFastInputModule"

void FFastInputModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FSlateApplication& SlateApplication = FSlateApplication::Get();
	SlateApplication.RegisterInputPreProcessor(MakeShareable(new FFastInputCustomInputListener()));
	FastInputManager = NewObject<UFastInputManager>();
	FastInputManager->AddToRoot();
}

void FFastInputModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FSlateApplication& SlateApplication = FSlateApplication::Get();
	FastInputManager->RemoveFromRoot();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFastInputModule, FastInput)