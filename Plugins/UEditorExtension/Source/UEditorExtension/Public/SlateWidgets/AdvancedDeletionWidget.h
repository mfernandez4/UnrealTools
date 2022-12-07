// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"

/**
 * 
 */

class SAdvancedDeletionTab : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SAdvancedDeletionTab) {}

	SLATE_ARGUMENT(TArray< TSharedPtr<FAssetData> >, AssetDataArray)
	
	SLATE_END_ARGS()

public:
	
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	
protected:
private:

	// The list of assets to display in the list view for the Advanced Deletion tab
	TArray< TSharedPtr<FAssetData> > SelectedAssetDataArray;
	
};