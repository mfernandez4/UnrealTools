// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetActions/SweetMaterialCreationWidget.h"
#include "DebugHeader.h"
#include "EditorAssetLibrary.h"
#include "EditorUtilityLibrary.h"

#pragma region MaterialCreationCore

void USweetMaterialCreationWidget::CreateMaterialFromSelectedTextures()
{
	// check if user wants to create a material with a custom name
	if ( bCustomMaterialName )
	{
		// check if user has entered a name
		if ( MaterialName.IsEmpty() || MaterialName.Equals( TEXT("M_") ) )
		{
			DebugHeader::ShowMsgDialog( EAppMsgType::Ok, TEXT("Please enter a valid name.") );
			return;
		}
	}

	// Get selected textures
	const TArray<FAssetData> SelectedAssets = UEditorUtilityLibrary::GetSelectedAssetData();
	TArray<UTexture2D*> SelectedTextures; // array to store selected textures
	FString MaterialPath; // path to store material
	FString SelectedTexturePath; // path to store selected texture

	// check if user has selected any textures
	if( !ProcessSelectedData( SelectedAssets, SelectedTextures, SelectedTexturePath) ) return;

	if ( CheckIfMaterialNameIsUsed( SelectedTexturePath, MaterialName ) ) return;
	DebugHeader::Print( SelectedTexturePath, FColor::Cyan );
	
	
}

// Process selected data, will filter out non-textures and return false if no textures are selected
bool USweetMaterialCreationWidget::ProcessSelectedData(const TArray<FAssetData>& SelectedAssets,
	TArray<UTexture2D*>& OutSelectedTextures, FString& OutSelectedTexturePath)
{
	// check if user has selected any textures
	if ( SelectedAssets.Num() ==0 )
	{
		// show error message
		DebugHeader::ShowMsgDialog( EAppMsgType::Ok, TEXT("Please select at least one texture.") );
		return false;
	}

	// bool check if bCustomMaterialName is set
	bool bCustomMaterialNameSet = false;

	// loop through selected assets
	for ( const FAssetData& SelectedData : SelectedAssets )
	{
		// get UObject from selected asset
		const UObject* SelectedAsset = SelectedData.GetAsset();

		// check if selected asset is valid, if not, skip it
		if ( !SelectedAsset ) continue;
		
		// check if selected asset is a texture
		if ( !SelectedData.GetClass()->IsChildOf( UTexture2D::StaticClass() ) )
		{
			// show error message
			DebugHeader::ShowMsgDialog( EAppMsgType::Ok, FString::Printf(TEXT("Please select only textures.\n%s is not a texture."), *SelectedAsset->GetName()) );
			return false;
		}
		
		// get texture
		UTexture2D* SelectedTexture = Cast<UTexture2D>( SelectedData.GetAsset() );

		// check if texture is valid, if not, skip it
		if ( !SelectedTexture ) continue;
		
		// add texture to array
		OutSelectedTextures.Add( SelectedTexture );

		// check if texture path is empty, assign it if it is
		// only need to do this once
		if ( OutSelectedTexturePath.IsEmpty() )
		{
			// get texture path
			OutSelectedTexturePath = SelectedData.PackagePath.ToString();
		}

		// check if bCustomMaterialName is set, if not, set it
		// only need to do this once
		if ( !bCustomMaterialName && !bCustomMaterialNameSet )
		{
			// get texture name
			FString TextureName = SelectedTexture->GetName();

			// check if texture name starts with "T_"
			if ( TextureName.StartsWith( TEXT("T_") ) )
			{
				// remove "T_" from texture name
				TextureName.RemoveFromStart( TEXT("T_") );
			}

			// add texture name to material name
			MaterialName.Empty();
			MaterialName.Append( FString::Printf(TEXT("M_%s"), *TextureName) );

			// set bCustomMaterialNameSet to true
			bCustomMaterialNameSet = true;
		}
	}

	// check if user has selected less than one texture
	if ( OutSelectedTextures.Num() < 1 )
	{
		// show error message
		DebugHeader::ShowMsgDialog( EAppMsgType::Ok, TEXT("Please select at least one texture.") );
		return false;
	}
	
	return true;
}

bool USweetMaterialCreationWidget::CheckIfMaterialNameIsUsed(const FString& FolderPathToCheck, const FString& MaterialNameToCheck)
{
	TArray<FString> ExistingAssetPaths = UEditorAssetLibrary::ListAssets( FolderPathToCheck, false );

	for ( const FString& ExistingAssetPath : ExistingAssetPaths )
	{
		const FString ExistingAssetName = FPaths::GetBaseFilename(ExistingAssetPath);
		if ( ExistingAssetName.Equals(MaterialNameToCheck) )
		{
			DebugHeader::ShowMsgDialog( EAppMsgType::Ok, FString::Printf(TEXT("Material name is already used.\n%s"), *ExistingAssetPath) );
			return true;
		}
	}
	return false;
}

#pragma endregion MaterialCreationCore
