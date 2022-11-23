// Copyright Epic Games, Inc. All Rights Reserved.

#include "UEditorExtension.h"

#include "ContentBrowserModule.h"



#define LOCTEXT_NAMESPACE "FUEditorExtensionModule"

void FUEditorExtensionModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	InitCBMenuExtension();
}

void FUEditorExtensionModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}


#pragma region ContentBrowserMenuExtension

void FUEditorExtensionModule::InitCBMenuExtension()
{
	// Load the content browser module
	FContentBrowserModule& ContentBrowserModule =
	FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));

	// Reference the content browser menu extender, which is an array of delegates
	// This is the menu that appears when you right click on an asset in the content browser
	TArray<FContentBrowserMenuExtender_SelectedPaths>& ContentBrowserMenuExtender =
	ContentBrowserModule.GetAllPathViewContextMenuExtenders();

	// create a new delegate
	FContentBrowserMenuExtender_SelectedPaths MenuExtenderDelegate;

	// add the delegate to the array
	ContentBrowserMenuExtender.Add(MenuExtenderDelegate);
}

#pragma endregion


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUEditorExtensionModule, UEditorExtension)