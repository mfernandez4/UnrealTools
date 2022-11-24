// Copyright Epic Games, Inc. All Rights Reserved.

#include "UEditorExtension.h"

#include "AssetToolsModule.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "ContentBrowserModule.h"
#include "DebugHeader.h"
#include "EditorAssetLibrary.h"
#include "EditorUtilityLibrary.h"
#include "ObjectTools.h"


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

TSharedRef<FExtender> FUEditorExtensionModule::CustomCBMenuExtension(const TArray<FString>& SelectedPaths)
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

		// store the folders that were selected
		SelectedFolderPaths = SelectedPaths;
	}


	// return the newly created menu extension
	return MenuExtender;
}

void FUEditorExtensionModule::AddCBMenuEntry( FMenuBuilder& MenuBuilder ) const
{
	// Add a new menu entry
	MenuBuilder.AddMenuEntry
	(
		FText::FromString( TEXT("Delete Unused Assets") ), // The text that will appear in the menu
		FText::FromString( TEXT("Safely delete all unused asset under the selected folder.") ), // The text that will appear in the tooltip
		FSlateIcon(
	
		), // No icon, we can also use FSlateIcon(FEditorStyle::GetStyleSetName(), "ClassIcon.Actor")
		FUIAction(FExecuteAction::CreateRaw( this, &FUEditorExtensionModule::OnDeleteUnusedAssetsClicked) ) // The function that will be called when the menu is clicked
	);
}

void FUEditorExtensionModule::OnDeleteUnusedAssetsClicked() const
{
	// debug, show message "Delete Unused Assets Button Clicked"
	// DebugHeader::ShowNotifyInfo(TEXT("Delete Unused Assets Button Clicked"));
	
	// check if the user has more that one folder selected,
	// if so, notify the user that only one folder can be selected
	if( SelectedFolderPaths.Num() > 1 )
	{
		DebugHeader::ShowMsgDialog( EAppMsgType::Ok, TEXT("Please select only one folder.") );
		return;
	}
	
	/* // debug, show the selected folder
	DebugHeader::ShowNotifyInfo( TEXT("Currently Selected Folder: ") + SelectedFolderPaths[0] );
	*/

	// get the assets in the selected folder
	const TArray<FString> AssetPathNames = UEditorAssetLibrary::ListAssets( SelectedFolderPaths[0] );

	// check if the asset in the folder is empty,
	// if so, then notify the user that there are no assets in the folder
	if( AssetPathNames.Num() == 0 )
	{
		DebugHeader::ShowMsgDialog( EAppMsgType::Ok, TEXT("No assets found in the selected folder.") );
		return;
	}
	
	// notify user of the number of assets found in the folder
	const EAppReturnType::Type UserResponse = DebugHeader::ShowMsgDialog(
		EAppMsgType::YesNo,
		FString::Printf( TEXT("Found %d assets in the selected folder. Do you want to delete them?"), AssetPathNames.Num() )
	);

	// if the user clicked no, return
	if (UserResponse == EAppReturnType::No) return;

	// container for the asset data of the assets in the selected folder
	TArray<FAssetData> UnusedAssetDataInFolder;

	// Fix up the redirectors for the assets in the selected folder
	FixUpRedirectors();
	
	// loop through the asset path names and get the asset data
	for( const FString& AssetPathName : AssetPathNames )
	{
		// skip root folders, like Developers, Collections, etc.
		if( AssetPathName.Contains(TEXT("Developers")) ||
			AssetPathName.Contains(TEXT("Collections"))
		)
		{
			continue;
		}

		// check if the asset exists in the project
		if( !UEditorAssetLibrary::DoesAssetExist(AssetPathName) ) continue;

		// get the references for the current asset
		TArray<FString> AssetReferences = UEditorAssetLibrary::FindPackageReferencersForAsset( AssetPathName );

		// check if the asset doesn't have any references
		if( AssetReferences.Num() == 0 )
		{
			// get the asset data of the current asset
			FAssetData AssetData = UEditorAssetLibrary::FindAssetData( AssetPathName );

			// add the asset data to the container
			UnusedAssetDataInFolder.Add( AssetData );
		}
	}

	
	// check if unused assets were found
	if( UnusedAssetDataInFolder.Num() > 0 )
	{
		// Delete the unused assets
		const int32 NumOfAssetsDeleted = ObjectTools::DeleteAssets( UnusedAssetDataInFolder );
		
		// Notify the user, display the total number of assets deleted
		DebugHeader::ShowMsgDialog( EAppMsgType::Ok, FString::Printf(TEXT("Successfully deleted %d unused assets!"), NumOfAssetsDeleted) );
	}
	else
	{
		// Notify the user that no unused assets were found
		DebugHeader::ShowMsgDialog( EAppMsgType::Ok,TEXT("No unused assets found in the selected folder.") );
	}
}


void FUEditorExtensionModule::FixUpRedirectors() const
{
	// Array of redirectors to fix
	TArray<UObjectRedirector*> RedirectorsToFix;

	// get the module manager and load the FAssetRegistryModule
	const FAssetRegistryModule& AssetRegistryModule =
		FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	// Create a FARFilter to filter through assets
	FARFilter Filter;
	Filter.bRecursivePaths = true; // search through all subfolders
	Filter.PackagePaths.Emplace("/Game"); // search through the /Game folder
	Filter.ClassPaths.Emplace("ObjectRedirector"); // search for redirectors

	// create an array to hold the asset data of the redirectors
	TArray<FAssetData> OutRedirectorAssetData;

	// get the asset registry and search for redirectors
	AssetRegistryModule.Get().GetAssets( Filter, OutRedirectorAssetData );

	// loop through the redirector asset data and get the redirectors and add them to the redirectors to fix array
	for( const FAssetData& AssetData : OutRedirectorAssetData )
	{
		// cast the asset data to a UObjectRedirector,
		// if valid, add it to the redirectors to fix array
		if( UObjectRedirector* RedirectorToFix = Cast<UObjectRedirector>(AssetData.GetAsset()) )
		{
			RedirectorsToFix.Add( RedirectorToFix );
		}
	}

	// get the module manager and load the FAssetToolsModule
	const FAssetToolsModule& AssetToolsModule =
		FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));

	// Get the asset tools and fix the redirectors
	AssetToolsModule.Get().FixupReferencers( RedirectorsToFix );
	
}

#pragma endregion


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUEditorExtensionModule, UEditorExtension)