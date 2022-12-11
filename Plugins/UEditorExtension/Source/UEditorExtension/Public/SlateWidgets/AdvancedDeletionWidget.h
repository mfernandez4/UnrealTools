// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"

/**
 * 
 */

class SAdvancedDeletionTab : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SAdvancedDeletionTab)
	{ }
	/** The data to display in the advanced deletion tab window. */
	SLATE_ARGUMENT( TArray< TSharedPtr<FAssetData> >, StoredAssetsDataArray )
	
	SLATE_END_ARGS()

public:
	
	SAdvancedDeletionTab();
	
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	
protected:
private:
	
	// The list of assets to display in the list view for the Advanced Deletion tab
	TArray< TSharedPtr<FAssetData> > SelectedAssetDataArray;

	// The list view widget for the Advanced Deletion tab. Will display the assets held within the selected folder.
	TSharedRef<ITableRow> OnGenerateRowForList(
		TSharedPtr<FAssetData> AssetData, // The asset data for the asset to display
		const TSharedRef<STableViewBase>& OwnerTable // The widget that owns the row
	) const;

	// Constructs a checkbox widget for the given asset data
	TSharedRef<SCheckBox> ConstructCheckBox( const TSharedPtr<FAssetData> AssetData ) const;

	// Called when the user clicks on the checkbox for an asset
	void OnAssetCheckBoxStateChanged( ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData ) const;

	// Called when the user clicks on the "Delete" button
	FReply OnDeleteButtonClicked( TSharedPtr<FAssetData> AssetData ) const;
	
	// Constructs a text block widget for the given asset data
	TSharedRef<STextBlock> ConstructTextForRowWidget( const FString TextContent, const FSlateFontInfo& TextFont ) const;

	/** Constructs a button widget with an option for an EditorGlyph. Returns a TSharedRef<SButton> */
	TSharedRef<SButton> ConstructButtonWithIcon( const FString ButtonText, const FText EditorGlyph, const FButtonStyle& InButtonStyle, const FOnClicked& OnClickedDelegate ) const;

private:
	/**
	 * Define some 'normal' button & text styles, upon which other variations can be based
	 */

	/** The style for the 'Delete' button. */
	const FButtonStyle DeleteButtonStyle = FAppStyle::GetWidgetStyle<FButtonStyle>("FlatButton.Danger");
	
	FButtonStyle TestButtonStyle;

	/** The style for the 'Delete' button's text */
	FTextBlockStyle DeleteButtonTextStyle;
	
	/** The style for the icon in the 'Delete' button */
	FTextBlockStyle DeleteButtonIconStyle;


	/**
	 * Getter && Setter Functions
	 */

	// Getter function for EmbossedText font style
	FORCEINLINE FSlateFontInfo GetEmbossedTextFont() const { return FCoreStyle::Get().GetFontStyle( FName("EmbossedText") ); }
};