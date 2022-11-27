// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FUEditorExtensionModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
#pragma region ContentBrowserMenuExtension

	
	/**
	 *
	 * This function is called when the Content Browser is created.
	 * It registers the extension with the Content Browser.
	 * It also registers a delegate, binds it to the content browser
	 * and adds it to the delegate list.
	 * 
	 */
	void InitCBMenuExtension();
	

	/**
	 * Creates a menu entry point in the Content Browser.
	 * This is the function that is bound to the delegate in InitCBMenuExtension.
	 * It creates the menu extension and returns it.
	 * @param SelectedPaths The paths that are selected in the Content Browser.
	 * @return The menu extension of type: TSharedRef<FExtender> .
	 */
	TSharedRef<FExtender> SweetDropdownMenuExtension(const TArray<FString>& SelectedPaths);

	/** 
	 * This is the function that creates the menu entry point in the content browser.
	 * It will show the function that is called when the menu entry is clicked.
	 * @param MenuBuilder The menu builder that is used to create the submenu entry.
	 */
	void AddSweetSubMenuEntry(class FMenuBuilder& MenuBuilder) const;

	
	/**
	 * Creates a menu entry point in the Content Browser.
	 * This is the function that is bound to the delegate in InitCBMenuExtension.
	 * It creates the menu extension and returns it.
	 * @param SelectedPaths The paths that are selected in the Content Browser.
	 * @return The menu extension of type: TSharedRef<FExtender> .
	 */
	TSharedRef<FExtender> DeleteUnusedAssetsMenuExtension( const TArray<FString>& SelectedPaths);

	/** 
	 * This is the function that creates the menu entry point in the content browser.
	 * It will show the function that is called when the menu entry is clicked.
	 * @param MenuBuilder The menu builder that is used to create the menu entry.
	 */
	void AddSweetMenuEntries(class FMenuBuilder& MenuBuilder) const;

	/**
	 * This is the function that is called when the menu entry point is clicked.
	 * Safely delete all unused asset under the selected folder in the Content Browser.
	 */
	void OnDeleteUnusedAssetsClicked() const;

	/**
	 * Removes redirectors from the assets in the content folder
	 * to allow assets to be safely deleted without breaking references.
	 */
	void FixUpRedirectors() const;

	/** Holds the folders that were selected in the content browser  */
	TArray<FString> SelectedFolderPaths;

	/**
		 * This is the function that is called when the menu entry point is clicked.
		 * Safely delete all unused folders under the selected folder in the Content Browser.
		 */
	void OnDeleteEmptyFoldersClicked() const;

	/**
	 * This is the function that is called when the menu entry point is clicked.
	 * Opens a window that allows the user to delete all unused assets found in the Content Browser.
	 */
	void OnOpenDeleteWindowClicked() const;


#pragma endregion

#pragma region CustomSlateEditorTab

	void RegisterDeleteAssetsWindow();

	TSharedRef<SDockTab> OnSpawnDeleteAssetsWindow(const FSpawnTabArgs& SpawnTabArgs) const;

#pragma endregion
};
