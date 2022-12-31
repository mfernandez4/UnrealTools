// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

class FUEditorExtensionStyle
{
public:

	/**
	 * Initialize the style set. This should only be called once per module.
	 * Creates the Slate Style, and registers the style set with the Slate Style Registry.
	 */
	static void Initialize();

	/**
	 * Shutdown the style set. Unregisters the style set and resets the singleton.
	 */
	static void Shutdown();

	/** reloads textures used by slate renderer */
	static void ReloadTextures();

	/** @return The Slate style set */
	static const ISlateStyle& Get();

	/** @returns the name of the style set. */
	static FName GetStyleSetName();

private:

	/**
	 * Creates the Slate style set.
	 * @returns The new Slate style set.
	 */
	static TSharedRef< class FSlateStyleSet > Create();

	inline static FName ExtensionStyleName = "UEditorExtensionStyle";

private:

	/**
	 * The Slate style set for the EditorExtension module
	 */
	static TSharedPtr< class FSlateStyleSet > StyleInstance;
};