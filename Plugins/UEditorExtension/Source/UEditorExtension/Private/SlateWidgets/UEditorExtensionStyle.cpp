// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SlateWidgets/UEditorExtensionStyle.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"

// Global pointer to the style set
TSharedPtr< FSlateStyleSet > FUEditorExtensionStyle::StyleInstance = NULL;

void FUEditorExtensionStyle::Initialize()
{
	// Only register once, check if the style set is already registered
	if (!StyleInstance.IsValid())
	{
		// Create the style set
		StyleInstance = Create();
		// Register the style set
		FSlateStyleRegistry::RegisterSlateStyle( *StyleInstance );
	}
}

void FUEditorExtensionStyle::Shutdown()
{
	// Unregister the style set
	FSlateStyleRegistry::UnRegisterSlateStyle( *StyleInstance );
	// Ensure there are no references to the style set
	ensure( StyleInstance.IsUnique() );
	// Reset the style set
	StyleInstance.Reset();
}

FName FUEditorExtensionStyle::GetStyleSetName()
{
	// Return the style set name
	static FName StyleSetName( ExtensionStyleName );
	return StyleSetName;
}

// define the styles for the style set
#ifndef IMAGE_BRUSH
#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#endif
#ifndef BOX_BRUSH
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#endif
#ifndef BORDER_BRUSH
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#endif
#ifndef TTF_FONT
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#endif
#ifndef OTF_FONT
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )
#endif

// define the size of the icons
const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);
const FVector2D Icon64x64(64.0f, 64.0f);
const FVector2D Icon128x128(128.0f, 128.0f);


TSharedRef< FSlateStyleSet > FUEditorExtensionStyle::Create()
{
	// Create the style set and make it a shared reference
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet(ExtensionStyleName));
	// Set the content root to the plugin's content directory
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("UEditorExtension")->GetBaseDir() / TEXT("Resources"));

	// Style->Set, create a SlateBrush from an image path, and set the brush's resource name
	Style->Set("UEditorExtension.PluginAction", new IMAGE_BRUSH(TEXT("ButtonIcon_128"), Icon128x128));

	// return the created style set
	return Style;
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT

void FUEditorExtensionStyle::ReloadTextures()
{
	// Check if the style set is valid or initialized
	if (FSlateApplication::IsInitialized())
	{
		// Reload the textures used by the style set
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FUEditorExtensionStyle::Get()
{
	// Return the style set instance
	return *StyleInstance;
}