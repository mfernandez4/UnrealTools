// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickAssetAction.h"
#include "DebugHeader.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"


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


void UQuickAssetAction::DuplicateAssets(int32 NumOfDuplicates)
{
	// if NumOfDuplicates is 0 or less, log error and return
	if (NumOfDuplicates <= 0) 
	{
		// Prompt user with a dialog box to enter a number greater than 0
		ShowMsgDialog(EAppMsgType::Ok, TEXT("Please enter a VALID number greater than 0"));
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
				Print( FString::Printf(TEXT("Duplicated %s as %s"), *SourceAssetName, *NewDuplicatedAssetName), FColor::Green );
				PrintLog( FString::Printf(TEXT("New Duplicated Asset: %s"), *NewPathName) );
			}
		}
	}

	if ( Counter > 0 )
	{
		// Notify the user, display the total number of assets duplicated
		ShowNotifyInfo( FString::Printf(TEXT("Successfully duplicated %d assets!"), Counter ) );
	}
	else
	{
		// Notify the user, No assets were duplicated
		ShowNotifyInfo(TEXT("No assets were duplicated!"));
		
	}
	
}


void UQuickAssetAction::AddPrefixes()
{
	// get the selected asset objects
	TArray<UObject*>SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();
	uint32 Counter = 0; // counter to keep track of the number of assets that were renamed

	for ( UObject* SelectedObject : SelectedObjects )
	{
		// if the selected object is null, continue to the next object
		if( !SelectedObject ) continue;

		// get the selected asset's class
		const UClass* AssetClass = SelectedObject->GetClass();
		// find the prefix for the selected object's class
		const FString* PrefixFound = PrefixMap.Find( AssetClass );

		// if the prefix is null or empty, notify the user
		// then continue to the next object
		if ( !PrefixFound || PrefixFound->IsEmpty() )
		{
			ShowNotifyInfo(FString::Printf(TEXT("Failed to find a prefix for the selected asset's class, %s"),
				*SelectedObject->GetClass()->GetName()) );
			continue;
		}

		// get the selected object's current name
		FString OldName = SelectedObject->GetName(); 

		// if the selected object's name already starts with the prefix, continue to the next object
		if ( OldName.StartsWith(*PrefixFound) ) 
		{
			ShowNotifyInfo(FString::Printf(TEXT("The selected asset's name, %s, already starts with the prefix, %s"),
				*OldName, **PrefixFound) );
			continue;
		}
		else if( OldName.EndsWith(*PrefixFound) ) // if the selected object name already has the suffix, continue to the next object
		{
			ShowNotifyInfo(FString::Printf(TEXT("The selected asset's name, %s, already has the suffix, %s"),
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
		ShowNotifyInfo( FString::Printf(TEXT("Successfully renamed %d assets!"), Counter ) );
	}
	else
	{
		ShowNotifyInfo(TEXT("No assets were renamed!"));
	}
}
