// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "QuickAssetAction.generated.h"

/**
 * 
 */
UCLASS()
class UEDITOREXTENSION_API UQuickAssetAction : public UAssetActionUtility
{
	GENERATED_BODY()

public:

	/**
	 * Duplicate the selected assets a specified number of times in the same folder as the original asset.
	 * @param NumOfDuplicates - The number of times to duplicate the selected assets
	 */
	UFUNCTION(CallInEditor, Category = "QuickAssetAction")
	static void DuplicateAssets( int32 NumOfDuplicates );
	
	// static void QuickCreateAsset(UClass* AssetClass, FString AssetName, FString PackagePath);
	
};
