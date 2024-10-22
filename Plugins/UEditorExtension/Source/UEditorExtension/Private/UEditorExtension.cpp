// Copyright Epic Games, Inc. All Rights Reserved.

#include "UEditorExtension.h"

#include "AssetToolsModule.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "ContentBrowserModule.h"
#include "DebugHeader.h"
#include "EditorAssetLibrary.h"
#include "LevelEditor.h"
#include "ObjectTools.h"
#include "SlateWidgets/UEditorExtensionStyle.h"
#include "SlateWidgets/ToolbarCommands.h"
#include "SlateWidgets/AdvancedDeletionWidget.h"


#define LOCTEXT_NAMESPACE "FUEditorExtensionModule"

void FUEditorExtensionModule::StartupModule()
{
	
	FUEditorExtensionStyle::Initialize();

	FUEditorExtensionStyle::ReloadTextures();

	FToolbarCommands::Register();
	
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	InitCBMenuExtension();

	// Register Custom Slate Tab
	RegisterDeleteAssetsWindow();

	// Initialize the Toolbar Extension
	InitializeToolBarMenuExtension();
}

void FUEditorExtensionModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner( FName("SweetDeleteAssetsWindow") );
	FUEditorExtensionStyle::Shutdown();
}

#pragma region ToolBarMenuExtension

void FUEditorExtensionModule::InitializeToolBarMenuExtension()
{
	// The UI command list responsible for handling actions for the toolbar items you'll be extending the menu with
	PluginCommands = MakeShareable(new FUICommandList);

	// Register the commands for the toolbar
	PluginCommands->MapAction(
		FToolbarCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FUEditorExtensionModule::OnOpenDeleteWindowClicked),
		FCanExecuteAction());
	
	// Load the LevelEditor module
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	// From the LevelEditor module, get the toolbar extender manager
	const TSharedPtr<FExtensibilityManager> LevelEditorToolbarExtender =
		LevelEditorModule.GetToolBarExtensibilityManager();

	// Create a new extender for the toolbar
	const TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);

	// Add a new toolbar extension to the extender
	// ToolbarExtender->AddToolBarExtension(
	// 	"ProjectSettings",
	// 	EExtensionHook::Before,
	// 	TSharedPtr<FUICommandList>(),
	// 	FToolBarExtensionDelegate::CreateRaw(this, &FUEditorExtensionModule::AddToolbarExtension)
	// );

	// Add a new toolbar extension to the extender
	ToolbarExtender->AddToolBarExtension(
		"ProjectSettings",
		EExtensionHook::Before,
		PluginCommands,
		FToolBarExtensionDelegate::CreateRaw(this, &FUEditorExtensionModule::AddToolbarExtension)
	);

	// Add the extender to the extender manager
	LevelEditorToolbarExtender->AddExtender(ToolbarExtender);
	
	
	SelectedFolderPaths.Empty();
	SelectedFolderPaths.Add("/Game/");
}

void FUEditorExtensionModule::AddToolbarExtension(FToolBarBuilder& ToolBarBuilder) const
{
	// Add a separator before our new toolbar extension
	ToolBarBuilder.AddSeparator(FName("UEditorExtension"));

	// Add a new toolbar button to the toolbar, opens the plugin window
	ToolBarBuilder.AddToolBarButton(
		FToolbarCommands::Get().PluginAction,
		FName("UEditorExtensionButton"),
		LOCTEXT("ToolbarButton", "UEditorExtension"),
		LOCTEXT("ToolbarButtonTooltip", "Open Plugin Window"),
		FToolbarCommands::Get().PluginAction->GetIcon()
		);
}

void FUEditorExtensionModule::OpenPluginWindow()
{
}


#pragma endregion

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
	SelectedFolderPaths.Empty();
	SelectedFolderPaths.Add("/Game/");
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
		FName("SweetDeleteAssetsWindow")
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

#pragma endregion

#pragma region CustomSlateEditorTab

// Register the new dock-able tab window
void FUEditorExtensionModule::RegisterDeleteAssetsWindow()
{
	// Register the tab spawner
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		FName("SweetDeleteAssetsWindow"), // Tab ID
		FOnSpawnTab::CreateRaw(this, &FUEditorExtensionModule::OnSpawnDeleteAssetsWindow) )
		.SetDisplayName( LOCTEXT("SweetDeleteAssetsWindow", "Delete Assets Window") )
		.SetTooltipText( LOCTEXT("SweetDeleteAssetsWindowTooltip", "Open the Delete Assets Window") )
		// .SetIcon( FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.ContentBrowser") )
		.SetIcon( FToolbarCommands::Get().PluginAction->GetIcon() )
		;
}

// Spawn the new dock-able tab window
void FUEditorExtensionModule::OnOpenDeleteWindowClicked() const
{
	// fix up redirectors
	FixUpRedirectors();
	
	// Create a new dock-able window
	FGlobalTabmanager::Get()->TryInvokeTab(FName("SweetDeleteAssetsWindow"));
}

// UI for the new dock-able tab window
TSharedRef<SDockTab> FUEditorExtensionModule::OnSpawnDeleteAssetsWindow(const FSpawnTabArgs& SpawnTabArgs)
{
	// Check if the selected folder is empty
	if( SelectedFolderPaths.Num() == 0 )
	{
		// Empty the selected folder paths array in case of bad data
		SelectedFolderPaths.Empty();
		// Add the root folder to the selected folder paths array
		SelectedFolderPaths.Add("/Game/");
	}
	
	// Get the assets from the selected folder
	const TArray< TSharedPtr< FAssetData> > AssetDataArray = GetAssetDataFromSelectedFolder();
	
	// Create the tab
	return
	SNew(SDockTab).TabRole(ETabRole::NomadTab)
	[
		SNew(SAdvancedDeletionTab)
		// Pass the module pointer to the tab. This will show the assets from the selected folder.
		.StoredAssetsDataArray( AssetDataArray )
		.WidgetTitle( "Delete Assets Window" )
	];
}

TArray<TSharedPtr<FAssetData>> FUEditorExtensionModule::GetAssetDataFromSelectedFolder() const
{
	// create return data array
	TArray<TSharedPtr<FAssetData>> AssetDataArray;

	// list assets from the selected directory
	const TArray<FString> SelectedFolderAssets = UEditorAssetLibrary::ListAssets(SelectedFolderPaths[0], true);

	// loop through the assets in the selected directory
	for( const FString& AssetPathName : SelectedFolderAssets )
	{
		// skip root folders, like Developers, Collections, etc.
		if( AssetPathName.Contains(TEXT("Developers")) ||
		AssetPathName.Contains(TEXT("Collections")) ||
		AssetPathName.Contains(TEXT("__ExternalActors__")) ||
		AssetPathName.Contains(TEXT("__ExternalObjects__")) )
		{
			continue;
		}
		
		// convert ObjectPath to PackageName. ObjectPath is deprecated behavior.
		const FString& TempAssetPathName = FPackageName::ObjectPathToPackageName(AssetPathName);
		// DebugHeader::PrintLog( TempAssetPathName );

		// check if the asset exists, if it does, continue
		if( !UEditorAssetLibrary::DoesAssetExist( TempAssetPathName ) ) continue;

		// get the asset data
		const FAssetData AssetData = UEditorAssetLibrary::FindAssetData( TempAssetPathName );
		// DebugHeader::PrintLog(TempAssetPathName);

		// convert the asset data to a t-shared-pointer and add it to the asset data array
		AssetDataArray.Add( MakeShared<FAssetData>(AssetData) );
	}

	// return the asset data array
	return AssetDataArray;
}


#pragma endregion

#pragma region AdvancedDeletionWindowCommands


void FUEditorExtensionModule::RegisterCommands()
{
	
}

bool FUEditorExtensionModule::DeleteSingleAsset( const FAssetData AssetToDelete ) const
{
	FixUpRedirectors();
	if( ObjectTools::DeleteAssets(TArray<FAssetData>{ AssetToDelete.GetAsset() }) > 0 )
	{
		return true;
	}

	return false;
}

bool FUEditorExtensionModule::DeleteMultipleAssets(const TArray<FAssetData> AssetsToDeleteArray)
{
	FixUpRedirectors();
	if( ObjectTools::DeleteAssets(AssetsToDeleteArray) > 0 )
	{
		return true;
	}

	return false;
}

void FUEditorExtensionModule::ListUnusedAssetsFilter( const TArray<TSharedPtr<FAssetData>>& AssetDataToFilter,
	TArray<TSharedPtr<FAssetData>>& OutUnusedAssets )
{
	
	// Make sure OutUnusedAssets is empty
	OutUnusedAssets.Empty();
	
	for( const TSharedPtr<FAssetData>& AssetData : AssetDataToFilter )
	{
		
		// check if the asset is used by getting the asset's references
			// UEditorAssetLibrary::FindPackageReferencersForAsset( FPackageName::ObjectPathToPackageName(AssetData->GetObjectPathString()) );
		// AssetData->GetPackage()->GetPathName()
		TArray<FString> AssetReferences =
			UEditorAssetLibrary::FindPackageReferencersForAsset( AssetData->PackagePath.ToString() );

		// if the asset is not used, add it to the unused assets array
		if( AssetReferences.Num() == 0 )
		{
			OutUnusedAssets.Add(AssetData);
		}
	}
}

void FUEditorExtensionModule::ListDuplicateAssetsFilter(const TArray<TSharedPtr<FAssetData>>& AssetDataToFilter,
	TArray< TSharedPtr<FAssetData> >& OutDuplicateAssets )
{
	// Make sure OutDuplicateAssets is empty
	OutDuplicateAssets.Empty();

	// TMultimap for finding duplicate assets
	TMultiMap<FString, TSharedPtr<FAssetData>> DuplicateAssetsMap;

	// loop through the asset data to filter and emplace the asset data into the map
	for( const TSharedPtr<FAssetData>& AssetData : AssetDataToFilter )
	{
		// add the asset data to the duplicate assets map
		DuplicateAssetsMap.Emplace( AssetData->AssetName.ToString(), AssetData );
	}

	// loop through again and check if the asset data is a duplicate
	for( const TSharedPtr<FAssetData>& AssetData : AssetDataToFilter )
	{
		TArray< TSharedPtr< FAssetData>> OutAssetDataArray;
		// Check if the asset data is a duplicate
		DuplicateAssetsMap.MultiFind( AssetData->AssetName.ToString(), OutAssetDataArray );

		if ( OutAssetDataArray.Num() <= 1 ) continue;
		
		for ( const TSharedPtr<FAssetData>& DuplicateAssetData : OutAssetDataArray )
		{
			// check if DuplicateAssetData is valid
			if( !DuplicateAssetData.IsValid() ) continue;
			OutDuplicateAssets.AddUnique(AssetData);
		}
	}

	// define the compare function for sorting the duplicate assets by asset name
	auto CompareFunction = [](const TSharedPtr<FAssetData>& A, const TSharedPtr<FAssetData>& B)
	{
		return A->AssetName.ToString() < B->AssetName.ToString();
	};
	
	// sort the duplicate assets array, by asset name
	OutDuplicateAssets.Sort(CompareFunction);
	
}

void FUEditorExtensionModule::SyncCBToClickedAssetForAssetList(const FString& AssetPathToSync)
{
	// create the TArray to pass to the SyncContentBrowserToAssets function
	TArray< FString > AssetsPathsToSync;
	// add the asset path to the array
	AssetsPathsToSync.Add(AssetPathToSync);

	// sync the content browser to the asset path
	UEditorAssetLibrary::SyncBrowserToObjects(AssetsPathsToSync);
}


#pragma endregion

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUEditorExtensionModule, UEditorExtension)