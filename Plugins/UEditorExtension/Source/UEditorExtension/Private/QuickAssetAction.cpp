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
