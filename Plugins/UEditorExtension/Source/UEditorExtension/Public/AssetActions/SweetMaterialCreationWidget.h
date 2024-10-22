// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "SweetMaterialCreationWidget.generated.h"

/**
 * 
 */
UCLASS()
class UEDITOREXTENSION_API USweetMaterialCreationWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

#pragma region MaterialCreationCore
public:

	/**
	 * Create a new material from the selected textures.
	 */
	UFUNCTION(BlueprintCallable, Category = "Material Creation")
	void CreateMaterialFromSelectedTextures();

	/**
	 * Boolean to check if the material will be created with a custom name.
	 */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Material Creation" )
	bool bCustomMaterialName = true;
	
	/**
	 * Material Name to create. If empty, the name will be generated automatically.
	 */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Material Creation", meta = ( EditCondition = "bCustomMaterialName" ) )
	FString MaterialName = TEXT("M_");

#pragma endregion

#pragma region MaterialCreation
private:

	/**
	 * Process the selected data. Filters out textures to create a material.
	 * @param SelectedAssets - The selected data.
	 * @param OutSelectedTextures - The filtered textures.
	 * @param OutSelectedTexturePath - The texture path for one of the filtered textures.
	 * @returns True if the process was successful. False if none of the selected assets
	 * were textures. 
	 */
	bool ProcessSelectedData( const TArray<FAssetData>& SelectedAssets, TArray<UTexture2D*>& OutSelectedTextures, FString& OutSelectedTexturePath );

	/**
	 * Checks if the MaterialName is not being used by another material. If it is, it will return true.
	 * @param FolderPathToCheck - The folder path to check for the material.
	 * @param MaterialNameToCheck - The name of the material to check.
	 * @returns True if the material name is being used. False if it is not.
	 */
	bool CheckIfMaterialNameIsUsed( const FString& FolderPathToCheck, const FString& MaterialNameToCheck );

	/**
	 * Creates a new material from the selected textures.
	 * @param NewMaterialName - The name of the material to create.
	 * @param SaveMaterialPath - The path to save the material.
	 */
	UMaterial* CreateMaterial( const FString& NewMaterialName, const FString& SaveMaterialPath );

#pragma endregion

#pragma region TexureNameHelpers

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Texture Names" )
	TArray<FString> BaseColorArray = {
		TEXT("_basecolor"),
		TEXT("_albedo"),
		TEXT("_diffuse"),
		TEXT("_diff"),
		TEXT("_b"),
		TEXT("_a"),
		TEXT("_d"),
	};

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Texture Names" )
	TArray<FString> MetallicArray = {
		TEXT("_metallic"),
		TEXT("_metal"),
		TEXT("_m"),
	};

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Texture Names" )
	TArray<FString> RoughnessArray = {
		TEXT("_roughness"),
		TEXT("_rough"),
		TEXT("_r"),
	};

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Texture Names" )
	TArray<FString> NormalMapArray = {
		TEXT("_normal"),
		TEXT("_normalmap"),
		TEXT("_n"),
	};

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Texture Names" )
	TArray<FString> AmbientOcclusionArray = {
		TEXT("_ambientocclusion"),
		TEXT("_ambient"),
		TEXT("_occlusion"),
		TEXT("_ao"),
	};

#pragma endregion TexureNameHelpers
};
