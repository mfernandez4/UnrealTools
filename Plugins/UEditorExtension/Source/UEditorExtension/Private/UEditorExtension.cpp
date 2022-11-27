// Copyright Epic Games, Inc. All Rights Reserved.

#include "UEditorExtension.h"

#include "AssetToolsModule.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "ContentBrowserModule.h"
#include "DebugHeader.h"
#include "EditorAssetLibrary.h"
#include "EditorStyleSet.h"
#include "EditorUtilityLibrary.h"
#include "ObjectTools.h"


#define LOCTEXT_NAMESPACE "FUEditorExtensionModule"

void FUEditorExtensionModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	InitCBMenuExtension();

	// Register Custom Slate Tab
	RegisterDeleteAssetsWindow();
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


	
	// create new delegate
	FContentBrowserMenuExtender_SelectedPaths SweetDropdownMenuDelegate;

	ContentBrowserMenuExtender.Add( FContentBrowserMenuExtender_SelectedPaths::CreateRaw
		(this, &FUEditorExtensionModule::SweetDropdownMenuExtension) );


	
	// // create a new delegate
	// FContentBrowserMenuExtender_SelectedPaths DeleteUnusedAssetsMenuDelegate;
	//
	// // bind the delegate and add it to the array
	// ContentBrowserMenuExtender.Add( FContentBrowserMenuExtender_SelectedPaths::CreateRaw
	// 	(this, &FUEditorExtensionModule::DeleteUnusedAssetsMenuExtension) );

	// // create a new delegate
	// FContentBrowserMenuExtender_SelectedPaths DeleteUnusedAssetsMenuDelegate;
	//
	// // bind the delegate and add it to the array
	// ContentBrowserMenuExtender.Add( FContentBrowserMenuExtender_SelectedPaths::CreateRaw
	// 	(this, &FUEditorExtensionModule::DeleteUnusedAssetsMenuExtension) );

	
	/*
	// bind the delegate to the function we want to call
	CustomCBMenuDelegate.BindRaw( this, &FUEditorExtensionModule::CustomCBMenuExtension );
	// CustomCBMenuDelegate.BindRaw(this, &FUEditorExtensionModule::OnExtendContentBrowserPathViewContextMenu);

	// add the delegate to the array
	ContentBrowserMenuExtender.Add( CustomCBMenuDelegate );
	*/
	
}

// Defines the position for inserting custom menu entry
TSharedRef<FExtender> FUEditorExtensionModule::SweetDropdownMenuExtension(const TArray<FString>& SelectedPaths)
{
	// Create a new extender
	TSharedRef<FExtender> MenuExtender = MakeShareable( new FExtender() );

	// Check if the selected paths is valid or greater than 0
	if( SelectedPaths.Num() > 0 )
	{
		// Create a new menu extension
		MenuExtender->AddMenuExtension(
			// Extension hook, position to insert the menu entry
			FName("PathContextBulkOperations"), // The name of the menu-button where we want to insert our new menu
			EExtensionHook::Before, // Insert our menu after the menu-button
			TSharedPtr<FUICommandList>(), // This can be used to add keyboard shortcuts to the menu
			FMenuExtensionDelegate::CreateRaw(this, &FUEditorExtensionModule::AddSweetSubMenuEntry) // Second delegate, defines the details for the menu entry
		);

		// store the folders that were selected
		SelectedFolderPaths = SelectedPaths;
	}
	
	return MenuExtender;
}

// Defines the details for the custom submenu / dropdown menu entry
void FUEditorExtensionModule::AddSweetSubMenuEntry( FMenuBuilder& MenuBuilder ) const
{
	// Title of the menu section
	const TAttribute<FText> &InHeadingText = FText::FromString("Sweet Context Operations");
	MenuBuilder.BeginSection( FName("SweetOperations"), InHeadingText );
	
	// Add a new menu entry
	MenuBuilder.AddSubMenu(
	FText::FromString( TEXT("Sweet Asset Actions") ), // The title text that will appear in the menu
	FText::FromString( TEXT("A List of Quick Asset Actions bundled with the Sweet Editor Extension Plug-in") ), // The text that will appear in the tooltip
	FNewMenuDelegate::CreateRaw( this, &FUEditorExtensionModule::AddSweetMenuEntries ),
	false,
	FSlateIcon(),
	true,
	FName("SweetAssetActions")
	);

}

// Defines the details for the custom menu entry
void FUEditorExtensionModule::AddSweetMenuEntries( FMenuBuilder& MenuBuilder ) const
{
	// Add a new menu entry for Delete Unused Assets
	MenuBuilder.AddMenuEntry
	(
		FText::FromString( TEXT("Delete Unused Assets") ), // The title text that will appear in the menu
		FText::FromString( TEXT("Safely delete all unused asset under the selected folder.") ), // The text that will appear in the tooltip
		FSlateIcon(), // No icon, we can also use FSlateIcon(FEditorStyle::GetStyleSetName(), "ClassIcon.Actor")
		FUIAction(FExecuteAction::CreateRaw( this, &FUEditorExtensionModule::OnDeleteUnusedAssetsClicked) ), // The function that will be called when the menu is clicked
		FName("DeleteUnusedAssets")
	);

	// Add a new menu entry Delete Empty Folders
	MenuBuilder.AddMenuEntry
	(
		FText::FromString( TEXT("Delete Empty Folders") ), // The title text that will appear in the menu
		FText::FromString( TEXT("Safely delete all empty folders in selection.") ), // The text that will appear in the tooltip
		FSlateIcon(), // No icon, we can also use FSlateIcon(FEditorStyle::GetStyleSetName(), "ClassIcon.Actor")
		FUIAction(FExecuteAction::CreateRaw( this, &FUEditorExtensionModule::OnDeleteEmptyFoldersClicked) ), // The function that will be called when the menu is clicked
		FName("DeleteEmptyFolders")
	);

	// Add a new menu entry Open Delete Assets Window
	MenuBuilder.AddMenuEntry
	(
		FText::FromString( TEXT("Open Delete Assets Window") ), // The title text that will appear in the menu
		FText::FromString( TEXT("Open a window that allow a you to delete individual or groups of assets.") ), // The text that will appear in the tooltip
		FSlateIcon(), // No icon, we can also use FSlateIcon(FEditorStyle::GetStyleSetName(), "ClassIcon.Actor")
		FUIAction(FExecuteAction::CreateRaw( this, &FUEditorExtensionModule::OnOpenDeleteWindowClicked) ), // The function that will be called when the menu is clicked
		FName("DeleteAssetsWindow")
	);
}

// Defines the position for inserting custom menu entry ** CURRENTLY NOT BEING USED **
TSharedRef<FExtender> FUEditorExtensionModule::DeleteUnusedAssetsMenuExtension(const TArray<FString>& SelectedPaths)
{
	// Create a new extender
	TSharedRef<FExtender> MenuExtender = MakeShareable( new FExtender() );

	// Check if the selected paths is valid or greater than 0
	if( SelectedPaths.Num() > 0 )
	{
		// Create a new menu extension
		MenuExtender->AddMenuExtension(
			// Extension hook, position to insert the menu entry
			FName("Delete"), // The name of the menu-button where we want to insert our new menu
			EExtensionHook::After, // Insert our menu after the menu-button
			TSharedPtr<FUICommandList>(), // This can be used to add keyboard shortcuts to the menu
			FMenuExtensionDelegate::CreateRaw(this, &FUEditorExtensionModule::AddSweetMenuEntries) // Second delegate, defines the details for the menu entry
		);

		// store the folders that were selected
		SelectedFolderPaths = SelectedPaths;
	}


	// return the newly created menu extension
	return MenuExtender;
}

// The function that will be called when the menu is clicked
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
	else if ( SelectedFolderPaths.Num() == 0 )
	{
		DebugHeader::ShowMsgDialog( EAppMsgType::Ok, TEXT("Please select a folder.") );
		return;
	}
	
	/* // debug, show the selected folder
	DebugHeader::ShowNotifyInfo( TEXT("Currently Selected Folder: ") + SelectedFolderPaths[0] );
	*/

	// get the assets in the selected folder
	const TArray<FString> PathNames = UEditorAssetLibrary::ListAssets( SelectedFolderPaths[0], true, false );

	// check if the folder is empty,
	// if so, then notify the user that there are no assets in the folder
	if( PathNames.Num() == 0 )
	{
		DebugHeader::ShowMsgDialog( EAppMsgType::Ok, TEXT("No assets found in the selected folder."), false );
		return;
	}
	
	// notify user of the number of assets found in the folder
	const EAppReturnType::Type UserResponse = DebugHeader::ShowMsgDialog(
		EAppMsgType::YesNo,
		FString::Printf( TEXT("Found %d assets in the selected folder. Do you want to delete them?"), PathNames.Num() ),
		false
	);

	// if the user clicked no, return
	if (UserResponse == EAppReturnType::No) return;

	// container for the asset data of the assets in the selected folder
	TArray<FAssetData> UnusedAssetDataInFolder;

	// Fix up the redirectors for the assets in the selected folder
	FixUpRedirectors();
	
	// loop through the asset path names and get the asset data
	for( const FString& AssetPathName : PathNames )
	{
		// skip root folders, like Developers, Collections, etc.
		if( AssetPathName.Contains(TEXT("Developers")) ||
			AssetPathName.Contains(TEXT("Collections")) ||
			AssetPathName.Contains(TEXT("__ExternalActors__")) ||
			AssetPathName.Contains(TEXT("__ExternalObjects__"))
			)
		{
			continue;
		}

		const FString& TempAssetPathName = FPackageName::ObjectPathToPackageName(AssetPathName);
		
		// check if the asset exists in the project
		if( !UEditorAssetLibrary::DoesAssetExist(TempAssetPathName) ) continue;

		// get the references for the current asset
		TArray<FString> AssetReferences = UEditorAssetLibrary::FindPackageReferencersForAsset( TempAssetPathName );

		// check if the asset doesn't have any references
		if( AssetReferences.Num() == 0 )
		{
			// get the asset data of the current asset
			FAssetData AssetData = UEditorAssetLibrary::FindAssetData( TempAssetPathName );

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
		DebugHeader::ShowMsgDialog( EAppMsgType::Ok, FString::Printf(TEXT("Successfully deleted %d unused assets!"), NumOfAssetsDeleted), false );
	}
	else
	{
		// Notify the user that no unused assets were found
		DebugHeader::ShowMsgDialog( EAppMsgType::Ok,TEXT("No unused assets found in the selected folder."), false );
	}

	// Ask the user if they want to delete empty folders
	const EAppReturnType::Type DeleteEmptyFoldersResponse = DebugHeader::ShowMsgDialog(
		EAppMsgType::YesNo,
		TEXT("Do you want to delete all empty folders in the selected folder?"),
		false
	);

	// check the user response
	if( DeleteEmptyFoldersResponse == EAppReturnType::No ) return;

	if ( DeleteEmptyFoldersResponse == EAppReturnType::Yes )
	{
		// delete all empty folders in the selected folder
		OnDeleteEmptyFoldersClicked();
	}
}

// Fix up the redirectors for the assets in the selected folder
void FUEditorExtensionModule::FixUpRedirectors() const
{
	DebugHeader::PrintLog(TEXT("Fixing up redirectors..."));
	// Array of redirectors to fix
	TArray<UObjectRedirector*> RedirectorsToFix;

	// get the module manager and load the FAssetRegistryModule
	const FAssetRegistryModule& AssetRegistryModule =
		FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	// Create a FARFilter to filter through assets
	FARFilter Filter;
	Filter.bRecursivePaths = true; // search through all subfolders
	Filter.PackagePaths.Emplace("/Game"); // search through the /Game folder
	Filter.ClassPaths.Emplace( UObjectRedirector::StaticPackage() ); // search for redirectors
	// Filter.ClassNames.Emplace("ObjectRedirector"); // search for redirectors

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

void FUEditorExtensionModule::OnDeleteEmptyFoldersClicked() const
{
	// // debug, show message "Delete Unused Assets Button Clicked"
	// DebugHeader::ShowNotifyInfo(TEXT("Delete Empty Folders Button Clicked"));

	// Fix up the redirectors before deleting the empty folders
	FixUpRedirectors();

	// List out all the assets including directories with ListAssets()
	const TArray<FString> FolderPathNames = UEditorAssetLibrary::ListAssets(SelectedFolderPaths[0], true, true);
	uint32 Counter = 0;
	
	DebugHeader::ShowNotifyInfo( FString::Printf( TEXT("Root Folder path: %s"), *SelectedFolderPaths[0]) );
	
	// Filter out the directories, and check if it exists with DoesDirectoryExist()
	FString DirectoryPaths;
	TArray<FString> EmptyDirectories;
	for (const FString& DirectoryPath : FolderPathNames)
	{
		// skip root folders, like Developers, Collections, etc.
		if( DirectoryPath.Contains(TEXT("Developers")) ||
			DirectoryPath.Contains(TEXT("Collections")) ||
			DirectoryPath.Contains(TEXT("__ExternalActors__")) ||
			DirectoryPath.Contains(TEXT("__ExternalObjects__"))
			)
		{
			continue;
		}

		// Testing
		DebugHeader::PrintLog(*DirectoryPath);
		
		if ( !UEditorAssetLibrary::DoesDirectoryExist(DirectoryPath) ) continue;
	
		// Check if directory is empty with DoesDirectoryHaveAssets()
		if ( !UEditorAssetLibrary::DoesDirectoryHaveAssets(DirectoryPath) )
		{
			// Add the empty directory path to an FString to print out later
			DirectoryPaths.Append( FString::Printf(TEXT("%s\n"), *DirectoryPath) );
	
			// Add the empty directory to the array
			EmptyDirectories.Add(DirectoryPath);
		}
	}
	
	if( EmptyDirectories.Num() == 0 )
	{
		DebugHeader::ShowMsgDialog( EAppMsgType::Ok, TEXT("No empty folders found in the selected folder."), false );
		return;
	}
	
	
	// Ask the user if they want to delete the empty folders, and store the response
	const EAppReturnType::Type UserResponse =
		DebugHeader::ShowMsgDialog( EAppMsgType::YesNoYesAll,
		FString::Printf( TEXT("Are you sure you want to delete the empty folders? \n%s\n Select 'YesAll' to include the root folder. \n %s"),
			*DirectoryPaths, *SelectedFolderPaths[0] ),
			false );
	
	// Check if the user response
	if( UserResponse == EAppReturnType::No ) return;
	
	if( UserResponse == EAppReturnType::Yes )
	{
		// Loop through the empty directories and delete them
		for (const FString& EmptyDirectory : EmptyDirectories)
		{
			UEditorAssetLibrary::DeleteDirectory(EmptyDirectory)
			? Counter++ : DebugHeader::ShowNotifyInfo( FString::Printf(TEXT("Failed to delete %s"), *EmptyDirectory) );
		}
	}

	if( UserResponse == EAppReturnType::YesAll )
	{
		// Loop through the empty directories and delete them
		for (const FString& EmptyDirectory : EmptyDirectories)
		{
			UEditorAssetLibrary::DeleteDirectory(EmptyDirectory)
			? Counter++ : DebugHeader::ShowNotifyInfo( FString::Printf(TEXT("Failed to delete %s"), *EmptyDirectory) );
		}

		// Check if the root folder is empty,
		// if it is, delete it
		if( !UEditorAssetLibrary::DoesDirectoryHaveAssets(SelectedFolderPaths[0]) )
		{
			// Delete the root folder
			UEditorAssetLibrary::DeleteDirectory(SelectedFolderPaths[0])
			? Counter++ : DebugHeader::ShowNotifyInfo( FString::Printf(TEXT("Failed to delete %s"), *SelectedFolderPaths[0]) );
		}

		// if the root folder isn't empty, get the assets and prompt the user
		else 
		{
			// find assets in the root folder
			const TArray<FString> RootFolderAssets = UEditorAssetLibrary::ListAssets(SelectedFolderPaths[0], true, false);
			FString RootFolderAssetsString;
			// loop through the assets
			for (const FString& RootFolderAsset : RootFolderAssets)
			{
				// add the asset to the string
				RootFolderAssetsString.Append( FString::Printf(TEXT("%s\n"), *RootFolderAsset) );
			}
			
			// Show a message that the root folder is not empty
			const EAppReturnType::Type RootFolderResponse = DebugHeader::ShowMsgDialog( EAppMsgType::OkCancel, 
			FString::Printf(TEXT("The root folder is not empty. It constains: \n%s\n Are you sure you want to delete the root folder?"),
				*RootFolderAssetsString), true );

			// check if the user wants to delete the root folder
			if( RootFolderResponse == EAppReturnType::Ok )
			{
				// Delete the root folder
				UEditorAssetLibrary::DeleteDirectory(SelectedFolderPaths[0])
				? Counter++
				: DebugHeader::ShowNotifyInfo( FString::Printf(TEXT("Failed to delete %s"), *SelectedFolderPaths[0]) );
			}
			
			// if the user doesn't want to delete the root folder,
			// display a message that the operation was cancelled.
			if( RootFolderResponse == EAppReturnType::Cancel )
			{
				// Show a message that the operation was cancelled
				DebugHeader::ShowMsgDialog( EAppMsgType::Ok, TEXT("Delete Root Folder Operation Cancelled!!"), false );
			}
			
		} // end of else statement
		
	} // end of user response == YesAll
	
	// Notify the user, display the total number of folders deleted
	if( Counter > 0 )
	{
		DebugHeader::ShowNotifyInfo(FString::Printf(TEXT("Delete %d empty folders!"), Counter));
	}
}

void FUEditorExtensionModule::OnOpenDeleteWindowClicked() const
{
	// debug
	DebugHeader::ShowNotifyInfo( "Open Delete Assets Window Button Pressed!!" );

	FGlobalTabmanager::Get()->TryInvokeTab(FName("DeleteAssetsWindow"));
}


#pragma endregion

#pragma region CustomSlateEditorTab

void FUEditorExtensionModule::RegisterDeleteAssetsWindow()
{
	// Register the tab spawner
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		FName("DeleteAssetsWindow"), // Tab ID
		FOnSpawnTab::CreateRaw(this, &FUEditorExtensionModule::OnSpawnDeleteAssetsWindow) )
		.SetDisplayName( LOCTEXT("DeleteAssetsWindow", "Delete Assets Window") )
		.SetTooltipText( LOCTEXT("DeleteAssetsWindowTooltip", "Open the Delete Assets Window") )
		.SetIcon( FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.ContentBrowser") );
}

TSharedRef<SDockTab> FUEditorExtensionModule::OnSpawnDeleteAssetsWindow(const FSpawnTabArgs& SpawnTabArgs) const
{
	// Create the tab
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SButton)
				.Text(LOCTEXT("DeleteAssetsWindowButton", "Delete Empty Folders"))
				// .OnClicked(this, &FUEditorExtensionModule::OnDeleteEmptyFoldersClicked)
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SButton)
				.Text(LOCTEXT("OpenDeleteWindowButton", "Open Delete Assets Window"))
				// .OnClicked(this, &FUEditorExtensionModule::OnOpenDeleteWindowClicked)
			]
		];
}

#pragma endregion

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUEditorExtensionModule, UEditorExtension)