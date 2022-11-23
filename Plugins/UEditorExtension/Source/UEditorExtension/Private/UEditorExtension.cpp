// Copyright Epic Games, Inc. All Rights Reserved.

#include "UEditorExtension.h"

#include "ContentBrowserModule.h"
#include "DebugHeader.h"


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

	// Reference the content browser menu extender, which is an array of delegates.
	// This is the array that holds the delegates for the
	// menu items that appear when you right click in the content browser.
	TArray<FContentBrowserMenuExtender_SelectedPaths>& ContentBrowserMenuExtender =
	ContentBrowserModule.GetAllPathViewContextMenuExtenders();

	// create a new delegate
	FContentBrowserMenuExtender_SelectedPaths CustomCBMenuDelegate;

	/*
	// bind the delegate to the function we want to call
	CustomCBMenuDelegate.BindRaw( this, &FUEditorExtensionModule::CustomCBMenuExtension );
	// CustomCBMenuDelegate.BindRaw(this, &FUEditorExtensionModule::OnExtendContentBrowserPathViewContextMenu);

	// add the delegate to the array
	ContentBrowserMenuExtender.Add( CustomCBMenuDelegate );
	*/

	// bind the delegate and add it to the array
	ContentBrowserMenuExtender.Add( FContentBrowserMenuExtender_SelectedPaths::CreateRaw
		(this, &FUEditorExtensionModule::CustomCBMenuExtension) );
}

TSharedRef<FExtender> FUEditorExtensionModule::CustomCBMenuExtension(const TArray<FString>& SelectedPaths) const
{
	// Create a new extender
	TSharedRef<FExtender> MenuExtender = MakeShareable( new FExtender() );

	// Check if the selected paths is valid or greater than 0
	if( SelectedPaths.Num() > 0 )
	{
		// Create a new menu extension
		MenuExtender->AddMenuExtension(
			FName("Delete"), // The name of the menu-button where we want to insert our new menu
			EExtensionHook::After, // Insert our menu after the menu-button
			TSharedPtr<FUICommandList>(), // Empty command list, this can be used to add keyboard shortcuts to the menu
			FMenuExtensionDelegate::CreateRaw(this, &FUEditorExtensionModule::AddCBMenuEntry)
		);
	}


	// return the newly created menu extension
	return MenuExtender;
}

void FUEditorExtensionModule::AddCBMenuEntry(FMenuBuilder& MenuBuilder) const
{
	MenuBuilder.AddMenuEntry
	(
		FText::FromString(TEXT("Delete Unused Assets")), // The text that will appear in the menu
		FText::FromString(TEXT("Safely delete all unused asset under the selected folder.")), // The text that will appear in the tooltip
		FSlateIcon(), // No icon, we can also use FSlateIcon(FEditorStyle::GetStyleSetName(), "ClassIcon.Actor")
		FUIAction(FExecuteAction::CreateRaw(this, &FUEditorExtensionModule::OnDeleteUnusedAssetsClicked)) // The function that will be called when the menu is clicked
	);
}

void FUEditorExtensionModule::OnDeleteUnusedAssetsClicked() const
{
	ShowNotifyInfo(TEXT("Delete Unused Assets Button Clicked"));
}

#pragma endregion


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUEditorExtensionModule, UEditorExtension)