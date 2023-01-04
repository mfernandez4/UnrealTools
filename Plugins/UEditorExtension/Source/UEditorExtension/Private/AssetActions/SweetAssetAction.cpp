// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetActions/SweetAssetAction.h"

#include "AssetToolsModule.h"
#include "DebugHeader.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "ObjectTools.h"


/*
 void UQuickAssetAction::QuickCreateAsset(UClass* AssetClass, FString AssetName, FString PackagePath)
{
	if (AssetClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AssetClass is nullptr"));
		return;
	}

	if (AssetName.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("AssetName is Empty"));
		return;
	}

	if (PackagePath.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("PackagePath is Empty"));
		return;
	}

	FString PackageName = PackagePath + TEXT("/") + AssetName;
	FString AssetPath = PackageName + TEXT(".") + AssetName;

	UObject* Asset = UEditorUtilityLibrary::CreateAsset(AssetClass, FName(*PackageName), FName(*AssetName));
	if (Asset == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Create Asset Failed"));
		return;
	}

	UEditorAssetLibrary::SaveAsset(AssetPath);
}
*/


#pragma region DuplicateAssets

void UQuickAssetAction::DuplicateAssets(int32 NumOfDuplicates)
{
	// if NumOfDuplicates is 0 or less, log error and return
	if (NumOfDuplicates <= 0) 
	{
		// Prompt user with a dialog box to enter a number greater than 0
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("Please enter a VALID number greater than 0"));
		return;
	}
	// Get the selected assets data
	TArray<FAssetData> SelectedAssetsData =  UEditorUtilityLibrary::GetSelectedAssetData();
	uint32 Counter = 0;

	// Loop through the selected assets data
	for( const FAssetData& SelectedAssetData : SelectedAssetsData )
	{
		// loop through the number of duplicates to create for each asset
		for (int i = 0; i < NumOfDuplicates; i++)
		{
			// Get the selected asset's path
			const FString SourceAssetPath = SelectedAssetData.GetObjectPathString();
			// Get the selected asset's name
			const FString SourceAssetName = SelectedAssetData.AssetName.ToString();
			// Form new duplicated asset name
			const FString NewDuplicatedAssetName = FString::Printf( TEXT("%s_%d"), *SourceAssetName, (i+1) );
			// Form new duplicated asset path
			const FString NewPathName = FPaths::Combine( SelectedAssetData.PackagePath.ToString(), NewDuplicatedAssetName );

			// Duplicate the selected asset with the new name and path
			// If the asset is successfully duplicated, log the new asset path and increment the counter
			if( UEditorAssetLibrary::DuplicateAsset( SourceAssetPath, NewPathName ) )
			{
				// Save the duplicated asset
				UEditorAssetLibrary::SaveAsset(NewPathName, false);
				++Counter;
				DebugHeader::Print( FString::Printf(TEXT("Duplicated %s as %s"), *SourceAssetName, *NewDuplicatedAssetName), FColor::Green );
				DebugHeader::PrintLog( FString::Printf(TEXT("New Duplicated Asset: %s"), *NewPathName) );
			}
		}
	}

	if ( Counter > 0 )
	{
		// Notify the user, display the total number of assets duplicated
		DebugHeader::ShowNotifyInfo( FString::Printf(TEXT("Successfully duplicated %d assets!"), Counter ) );
	}
	else
	{
		// Notify the user, No assets were duplicated
		DebugHeader::ShowNotifyInfo(TEXT("No assets were duplicated!"));
		
	}
	
}
#pragma endregion


#pragma region RenameAssets
void UQuickAssetAction::AddPrefixes()
{
	// get the selected asset objects
	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();
	uint32 Counter = 0; // counter to keep track of the number of assets that were renamed

	for ( const TObjectPtr<UObject> SelectedObject : SelectedObjects )
	{
		// if the selected object is null, continue to the next object
		if( !SelectedObject ) continue;

		// get the selected asset's class
		const TObjectPtr<UClass> AssetClass = SelectedObject->GetClass();
		// find the prefix for the selected object's class
		const TObjectPtr<FString> PrefixFound = PrefixMap.Find( AssetClass );

		// if the prefix is null or empty, notify the user
		// then continue to the next object
		if ( !PrefixFound || PrefixFound->IsEmpty() )
		{
			DebugHeader::ShowNotifyInfo(FString::Printf(TEXT("Failed to find a prefix for the selected asset's class, %s"),
				*SelectedObject->GetClass()->GetName()) );
			continue;
		}

		// get the selected object's current name
		FString OldName = SelectedObject->GetName(); 

		// if the selected object's name already starts with the prefix, continue to the next object
		if ( OldName.StartsWith(*PrefixFound) ) 
		{
			DebugHeader::ShowNotifyInfo(FString::Printf(TEXT("The selected asset's name, %s, already starts with the prefix, %s"),
				*OldName, **PrefixFound) );
			continue;
		}
		else if( OldName.EndsWith(*PrefixFound) ) // if the selected object name already has the suffix, continue to the next object
		{
			DebugHeader::ShowNotifyInfo(FString::Printf(TEXT("The selected asset's name, %s, already has the suffix, %s"),
				*OldName, **PrefixFound) );
			continue;
		}

		// Query the selected object class type, remove the prefix and/or suffix if it has one
		if( SelectedObject->IsA<UMaterialInstanceConstant>() ) // Material Instance
		{
			OldName.RemoveFromStart( TEXT("M_") );
			OldName.RemoveFromEnd( TEXT("_Inst") );
		}

		// form the new name
		FString NewName;
		// check if the prefix is a suffix
		if ( PrefixFound->StartsWith("_") )
		{
			NewName = FString::Printf(TEXT("%s%s"), *OldName, **PrefixFound);
		}
		else
		{
			NewName = FString::Printf(TEXT("%s%s"), **PrefixFound, *OldName);
		}

		// rename the selected object
		UEditorUtilityLibrary::RenameAsset( SelectedObject, *NewName );
		++Counter;
	}

	// Notify the user, display the total number of assets renamed
	if( Counter > 0 )
	{
		DebugHeader::ShowNotifyInfo( FString::Printf(TEXT("Successfully renamed %d assets!"), Counter ) );
	}
	else
	{
		DebugHeader::ShowNotifyInfo(TEXT("No assets were renamed!"));
	}
}


void UQuickAssetAction::RenameSelectedAssets(FString SearchString, FString ReplaceString, ESearchCase::Type SearchCase)
{
	// Check if something needs to be done
	if (SearchString.IsEmpty() || SearchString.Equals(ReplaceString, SearchCase))
	{
		return;
	}
	
	// Get the selected objects
	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();
	
	uint32 Counter = 0;

	// Check each Asset if it needs to be renamed
	for (UObject* SelectedObject : SelectedObjects)
	{
		// Check if SelectedObject pointer is not nullptr
		if (SelectedObject != nullptr)
		{
			// Get Asset name
			FString AssetName = SelectedObject->GetName();

			// Check if Asset name contains SearchString
			if (AssetName.Contains(SearchString, SearchCase))
			{
				// Form new name and store in a variable
				FString NewName = AssetName.Replace(*SearchString, *ReplaceString, SearchCase);
				UEditorUtilityLibrary::RenameAsset(SelectedObject, NewName);
				
				// increment counter variable
				Counter++;
			}
		}
	}

	DebugHeader::ShowNotifyInfo( FString::Printf(TEXT("Successfully renamed %d assets!"), Counter ) );
}
#pragma endregion


#pragma region RemoveUnusedAssets
void UQuickAssetAction::RemoveUnusedAssets()
{
	// Get Selected Assets Data
	TArray<FAssetData>SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();
	// Container to hold the unused assets
	TArray<FAssetData>UnusedAssetsData;

	// Fix up the references for the selected assets
	FixUpRedirectors();

	// Loop through the selected assets data
	for( const FAssetData& SelectedAssetData : SelectedAssetsData )
	{
		// Get the selected asset's references
		TArray<FString> AssetReferences = UEditorAssetLibrary::FindPackageReferencersForAsset( SelectedAssetData.GetObjectPathString() );

		// if the asset has no references, add it to the unused assets container
		if( AssetReferences.Num() == 0 )
		{
			// Add the unused asset to the container
			UnusedAssetsData.Add( SelectedAssetData );
		}
	}

	// check the number of unused assets, if there are none notify the user, then return
	// else, delete the unused assets
	if( UnusedAssetsData.Num() == 0 )
	{
		// Notify the user, No assets were deleted
		DebugHeader::ShowMsgDialog( EAppMsgType::Ok ,TEXT("No unused assets were found in the selected assets!"), false );
		return;
	}
	
	// Delete the unused assets
	const int32 NumOfAssetsDeleted = ObjectTools::DeleteAssets( UnusedAssetsData );

	if(NumOfAssetsDeleted == 0) return;
	
	// Notify the user, display the total number of assets deleted
	DebugHeader::ShowNotifyInfo( FString::Printf(TEXT("Successfully deleted %d unused assets!"), NumOfAssetsDeleted) );
}
#pragma endregion


#pragma region Helper Functions
void UQuickAssetAction::FixUpRedirectors() const
{
	// Array of redirectors/asset references to fix
	TArray<UObjectRedirector*> RedirectorsToFix;

	// get the module manager and load the FAssetRegistryModule
	const FAssetRegistryModule& AssetRegistryModule =
	FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	// create an FARFilter to filter through assets
	FARFilter Filter;
	Filter.bRecursivePaths = true; // search through all sub-directories
	Filter.PackagePaths.Emplace("/Game"); // search through the /Game directory
	Filter.ClassPaths.Emplace("ObjectRedirector"); // search for object redirectors

	// create an array to hold the asset data of the redirectors
	TArray<FAssetData> OutRedirectors;

	// get the asset registry and search for redirectors
	AssetRegistryModule.Get().GetAssets( Filter,OutRedirectors );

	for( const FAssetData& RedirectorData : OutRedirectors )
	{
		if( UObjectRedirector* RedirectorToFix = Cast<UObjectRedirector>(RedirectorData.GetAsset()) )
		{
			RedirectorsToFix.Add( RedirectorToFix );
		}
	}

	// get the module manager and load the FAssetToolsModule
	const FAssetToolsModule& AssetToolsModule =
	FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));

	// get the asset tools and fix the redirectors
	AssetToolsModule.Get().FixupReferencers( RedirectorsToFix );
}
#pragma endregion

