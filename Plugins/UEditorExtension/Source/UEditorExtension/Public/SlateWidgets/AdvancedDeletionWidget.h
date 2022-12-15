// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"

/**
 * 
 */

class SAdvancedDeletionTab : public SCompoundWidget
{
	/**
	 *	This is a macro that creates a struct of variables and exposes them to InArgs in our construct function.
	 *	You can define defaults for each argument setup by prefixing the argument name with _
	 */
	SLATE_BEGIN_ARGS(SAdvancedDeletionTab)
		: _WidgetTitle(FName())
	{ }
		
		//Example argument for slate, follows (Type, Name)
		//See construct for accessing.
		SLATE_ARGUMENT(FName, WidgetTitle)
		
		/** The data to display in the advanced deletion tab window. */
		SLATE_ARGUMENT( TArray< TSharedPtr<FAssetData> >, StoredAssetsDataArray )
	
	SLATE_END_ARGS()

public:
	
	/**
	 * Construct this widget. Called by the SNew() Slate macro.
	 *
	 * @param  InArgs	Declaration used by the SNew() macro to construct this widget
	 */
	void Construct(const FArguments& InArgs);

	SAdvancedDeletionTab();

private:

	//An example property to set in Construct
	TAttribute<FName> WidgetName;
	
protected:
	
private:

	TSharedPtr< SListView< TSharedPtr< FAssetData> > > AssetListView;
	
	// The list of assets that is displayed in the list view for the Advanced Deletion tab
	TArray< TSharedPtr<FAssetData> > SelectedAssetDataArray;

	// Stores the Original list of assets to display in the list view used for filtering
	TArray< TSharedPtr<FAssetData> > StoredAssetDataArray;

	// Constructs the list view for the Advanced Deletion tab which holds the assets to delete
	TSharedRef< SListView< TSharedPtr <FAssetData> > > ConstructAssetListView();

	// Reference to the list view for the Advanced Deletion tab
	TSharedPtr< SListView< TSharedPtr <FAssetData> > > ConstructedAssetListView;

	// Reference to the editable text box for the Advanced Deletion tab
	TSharedPtr< SEditableTextBox > AssetSearchBox;

	// Refreshes the list of assets to display in the list view for the Advanced Deletion tab
	void RefreshAssetListView() const;

	// Updates the asset list view as the search box is changed
	void OnSearchBoxChanged(const FText& Text);
	
	// Called when the user clicks on the "Delete All" button
	FReply OnDeleteAllButtonClicked(  );

	// Called when the user clicks on the "Delete All" button
	FReply OnSelectAllButtonClicked(  );

	// Called when the user clicks on the "Delete All" button
	FReply OnDeselectAllButtonClicked(  );



#pragma region RowWidgetForAssetListView
	
	// The list view widget for the Advanced Deletion tab. Will display the assets held within the selected folder.
	TSharedRef<ITableRow> OnGenerateRowForList(
		TSharedPtr<FAssetData> AssetData, // The asset data for the asset to display
		const TSharedRef<STableViewBase>& OwnerTable // The widget that owns the row
	);

	// Constructs a checkbox widget for the given asset data
	TSharedRef<SCheckBox> ConstructCheckBox( const TSharedPtr<FAssetData> AssetData ) const;

	// Called when the user clicks on the checkbox for an asset
	void OnAssetCheckBoxStateChanged( ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData ) const;
	
	// Constructs a text block widget for the given asset data.
	TSharedRef<STextBlock> ConstructTextForRowWidget( const FString TextContent, const FSlateFontInfo& TextFont ) const;

	/** Constructs a button widget with an option for an EditorGlyph. Returns a TSharedRef<SButton> */
	TSharedRef<SButton> ConstructButtonWithIcon( const FString ButtonText, const FText EditorGlyph, const FButtonStyle& InButtonStyle, const FOnClicked& OnClickedDelegate ) const;
	
	// Called when the user clicks on the "Delete" button
	FReply OnDeleteButtonClicked( TSharedPtr<FAssetData> AssetData );

#pragma endregion
	

private:
	
	//	/**
	//	*	Define some 'normal' button & text styles, upon which other variations can be based.
	//	*/
	

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