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

	UFUNCTION(CallInEditor, Category = "QuickAssetAction")
	static void DuplicateAssets( int32 NumOfDuplicates );
	
	// static void QuickCreateAsset(UClass* AssetClass, FString AssetName, FString PackagePath);
	
};
