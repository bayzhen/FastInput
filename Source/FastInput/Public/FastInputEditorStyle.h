#pragma once
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"


class FFastInputEditorStyle : public FSlateStyleSet
{
public:
	FFastInputEditorStyle()
		: FSlateStyleSet("FastInputEditorStyle")
	{
		// Initialize and register your custom style resources here
		const FVector2D IconSize(512.0f, 512.0f);
		FSlateImageBrush* IconBrush = new FSlateImageBrush(FPaths::ProjectPluginsDir() + "FastInput/Resources/Wings", IconSize);
		Set("FastInputEditorStyle.FastInputIcon", IconBrush);
	}

	static TSharedPtr<ISlateStyle> Get()
	{
		return Instance;
	}

private:
	static TSharedPtr<FSlateStyleSet> Instance;
};

// Initialize the static instance
TSharedPtr<FSlateStyleSet> FFastInputEditorStyle::Instance = MakeShareable(new FFastInputEditorStyle());
