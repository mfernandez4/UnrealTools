// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidgets/AdvancedDeletionWidget.h"
#include <SlateOptMacros.h>

#include "DebugHeader.h"
#include "UEditorExtension.h"

#include "EditorFontGlyphs.h"
#include "Styling/SlateStyleMacros.h"
#include "Widgets/Layout/SScrollBox.h"


#define LOCTEXT_NAMESPACE "SAdvancedDeletionTab"
#define ListAllAssets "List All Available Assets"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION


// Constructor
SAdvancedDeletionTab::SAdvancedDeletionTab()
{
	// initialize the DeleteButtonTextStyle
	DeleteButtonTextStyle = FTextBlockStyle()
		// .SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 18)) // example of how to set the font
		.SetFont(DEFAULT_FONT("Bold", 13))
		.SetColorAndOpacity(FLinearColor::White)
		.SetShadowOffset(FVector2D( 2, 2 ))
		.SetShadowColorAndOpacity(FLinearColor::Black);

	DeleteButtonIconStyle = FTextBlockStyle()
		.SetFont(FAppStyle::Get().GetFontStyle("FontAwesome.12"))
		.SetColorAndOpacity(FLinearColor::White)
		.SetShadowOffset(FVector2D( 2, 2 ))
		.SetShadowColorAndOpacity(FLinearColor::Black);

	// create settings for the brush outline
	FSlateBrushOutlineSettings DeleteButtonOutlineSettings = ( FLinearColor(0.f, 0.f, 0.f, 0.f), 1.f );
	DeleteButtonOutlineSettings.CornerRadii = FVector4( 5.f, 5.f, 5.f, 5.f );
	DeleteButtonOutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
	
	// create the FButtonStyle for the delete button
	TestButtonStyle = FButtonStyle()
        .SetNormal( FSlateNoResource() )
        .SetNormalPadding( FMargin( 2,2,2,2 ) )
        .SetPressedPadding( FMargin( 2,3,2,1 ) )
        .SetNormalForeground(FLinearColor( 0, 0, 0 ))
        .SetPressedForeground(FLinearColor( 0, 0, 0 ))
        .SetHoveredForeground(FLinearColor( 0, 0, 0 ))
        .SetDisabledForeground(FLinearColor( 0, 0, 0 ));

	/* Normal button style */
	TestButtonStyle.Normal.DrawAs = ESlateBrushDrawType::RoundedBox;
	TestButtonStyle.Normal.OutlineSettings = DeleteButtonOutlineSettings;
	TestButtonStyle.Normal.TintColor = FLinearColor(0.05f, 0.05f, 0.05f, 0.8f);
	// DeleteButtonStyle.Normal.TintColor = FLinearColor( 0.013702f, 0.031896f, 0.013702f); // dark green

	/* Hovered button style */
	TestButtonStyle.Hovered.DrawAs = ESlateBrushDrawType::RoundedBox;
	TestButtonStyle.Hovered.OutlineSettings = DeleteButtonOutlineSettings;
	// DeleteButtonStyle.Hovered.TintColor = FLinearColor( 0.728f, 0.364f, 0.003f); // old color
	TestButtonStyle.Hovered.TintColor = FLinearColor( 0.028f, 0.364f, 0.728f);

	/* Pressed button style */
	TestButtonStyle.Pressed.DrawAs = ESlateBrushDrawType::RoundedBox;
	TestButtonStyle.Pressed.OutlineSettings = DeleteButtonOutlineSettings;
	TestButtonStyle.Pressed.TintColor = FLinearColor( 0.701f, 0.225f, 0.003f);
	
	;
}


void SAdvancedDeletionTab::Construct(const FArguments& InArgs)
{
	//WidgetName is now of type TAttribute<FName> to resolve, use WidgetName.Get();
	WidgetName = InArgs._WidgetTitle;
	
	// Can the widget ever support keyboard focus
	bCanSupportFocus = true; // This is required for keyboard focus to be passed to this widget

	// Capture the Asset Data from the selected folder. This is the data we will display in the list view.
	SelectedAssetData = InArgs._StoredAssetsDataArray;

	// Store the SelectedAssetDataArray to use for searching
	StoredAssetDataArray = SelectedAssetData;

	// Empty arrays to get rid of any old data
	AssetDataToDelete.Empty();
	CheckBoxesArray.Empty();

	// Create the conditional dropdown options
	DropDownOptions.Add(MakeShared<FString>( ListAllAssets ));
	
	// Create Font Info for the text block
	FSlateFontInfo TitleTextFont = GetEmbossedTextFont();
	TitleTextFont.Size = 30;

	ChildSlot
	[
		// Main Vertical Box
		SNew(SVerticalBox)

		// First Vertical Slot for Title Text
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Advanced Deletion")))
			.Font(TitleTextFont)
			.Justification(ETextJustify::Center)
			.ColorAndOpacity(FColor::White)
		]

		// Drop down to specify the listing condition
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBox)
			.HeightOverride(30.f)
			[
				SNew(SBorder)
				// .BorderImage(FAppStyle::Get().GetBrush("Border"))
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SHorizontalBox)

					// Condition Drop Down
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						ConstructDropDown()
					]

					// Refresh Asset List Button
					// + SHorizontalBox::Slot()
					// .AutoWidth()
					// [
					// 	
					// ]
				]
			]
		]
		
		// The asset list
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SNew(SBox)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::Get().GetBrush("Border"))
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				// .BorderBackgroundColor(FColor::Green)
				[
					SNew(SScrollBox)
					+ SScrollBox::Slot()
					[
						ConstructAssetListView()
					]
				]
			]
		]

		// Slot for 3 buttons ( Delete All, Select All, Deselect All )
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBox)
			.HeightOverride(35.f)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::Get().GetBrush("Border"))
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SHorizontalBox)

					// Search Bar
					+ SHorizontalBox::Slot()
					.FillWidth(1.f)
					.Padding(0.8f, 0.8f, 3.0f, 0.8f)
					[
						SAssignNew( AssetSearchBox, SEditableTextBox )
						.Font( FCoreStyle::Get().GetFontStyle("BoldFont") )
						.HintText( LOCTEXT("SearchBoxHint", "Search Assets") )
						.OnTextChanged( this, &SAdvancedDeletionTab::OnSearchBoxChanged )
					]
					
					
					// Delete All Button
					+SHorizontalBox::Slot()
					.Padding(0.8f)
					.HAlign(HAlign_Fill)
					.AutoWidth()
					[
					ConstructButtonWithIcon(
						LOCTEXT("AdvancedDeletionDeleteAllGroup", "Delete Selected").ToString(),
					FEditorFontGlyphs::Times,
					DeleteButtonStyle,
					FOnClicked::CreateSP(this, &SAdvancedDeletionTab::OnDeleteAllButtonClicked)
					)
					]

					// Select All Button
					+SHorizontalBox::Slot()
					.Padding(0.8f)
					.HAlign(HAlign_Fill)
					.AutoWidth()
					[
					ConstructButtonWithIcon( 
					"Select All",
					FEditorFontGlyphs::Check_Circle,
					DeleteButtonStyle,
					FOnClicked::CreateSP(this, &SAdvancedDeletionTab::OnSelectAllButtonClicked)
					)
					]

					// Deselect All Button
					+SHorizontalBox::Slot()
					.Padding(0.8f)
					.HAlign(HAlign_Fill)
					.AutoWidth()
					[
					ConstructButtonWithIcon( 
					"Deselect All",
					FEditorFontGlyphs::Circle_O,
					DeleteButtonStyle,
					FOnClicked::CreateSP(this, &SAdvancedDeletionTab::OnDeselectAllButtonClicked)
					)
					]
				]
			]
		]
	];
}


// Construct the Asset List View
TSharedRef<SListView<TSharedPtr<FAssetData>>> SAdvancedDeletionTab::ConstructAssetListView()
{
	// Create the Asset List View
	SAssignNew( ConstructedAssetListView, SListView< TSharedPtr<FAssetData> >)
	.ItemHeight(30.f)
	.ListItemsSource(&SelectedAssetData)
	.OnGenerateRow(this, &SAdvancedDeletionTab::OnGenerateRowForList);

	// Convert the variable to a shared reference and return it
	return ConstructedAssetListView.ToSharedRef();
}


// Refresh the asset list view
void SAdvancedDeletionTab::RefreshAssetListView()
{
	// Empty the array
	CheckBoxesArray.Empty();
	AssetDataToDelete.Empty();
	
	// Check if the Asset List View Widget is valid
	if( ConstructedAssetListView.IsValid() )
	{
		// Rebuild the Asset List
		ConstructedAssetListView->RebuildList();
	}
}


#pragma region DropDownConditional

TSharedRef<SComboBox<TSharedPtr<FString>>> SAdvancedDeletionTab::ConstructDropDown()
{
	TSharedRef<SComboBox<TSharedPtr<FString>>> ConstructedComboBox =
		SNew(SComboBox<TSharedPtr<FString>>)
		.OptionsSource(&DropDownOptions)
		.OnGenerateWidget(this, &SAdvancedDeletionTab::OnGenerateWidgetContent)
		.OnSelectionChanged(this, &SAdvancedDeletionTab::OnDrowDownSelectionChanged)
		[
			SAssignNew( DisplayedDropDownOption, STextBlock )
			.Text(FText::FromString( TEXT("List Assets Options") ))
			// .Text(this, &SAdvancedDeletionTab::GetDrowDownSelectedOption)
		];
	;

	
	return ConstructedComboBox;
}

TSharedRef<SWidget> SAdvancedDeletionTab::OnGenerateWidgetContent(TSharedPtr<FString> InItem)
{
	TSharedRef<STextBlock> ConstructedComboText = SNew(STextBlock)
		.Text(FText::FromString( *InItem.Get() ));
	return ConstructedComboText;
}

void SAdvancedDeletionTab::OnDrowDownSelectionChanged(TSharedPtr<FString> Selection, ESelectInfo::Type SelectInfo)
{
	DebugHeader::Print( *Selection.Get(), FColor::Cyan );

	DisplayedDropDownOption->SetText( FText::FromString( *Selection.Get() ));
}

#pragma endregion


#pragma region RowWidgetForAssetListView

// Details about this function here.
TSharedRef<ITableRow> SAdvancedDeletionTab::OnGenerateRowForList(TSharedPtr<FAssetData> AssetData, const TSharedRef<STableViewBase>& OwnerTable)
{
	// Check if the asset data is valid, if not return an empty row widget
	if ( !AssetData.IsValid() ) return SNew( STableRow< TSharedPtr<FAssetData> >, OwnerTable );
	
	const FString& DisplayAssetName = AssetData->AssetName.ToString();
	const FString& DisplayClassName = AssetData->GetClass()->GetName();

	FSlateFontInfo AssetClassNameFont = GetEmbossedTextFont();
	AssetClassNameFont.Size = 12;
	FSlateFontInfo AssetNameFont = FCoreStyle::Get().GetFontStyle("BoldFont");
	AssetNameFont.Size = 12;
	
	// const FLinearColor& DeleteButtonColor = FLinearColor(0.3f, 0.8f, 0.3f, 1.f);
	// const FLinearColor& DeleteButtonColor = FLinearColor(0.10616f, 0.48777f, 0.10616f, 1.f);
	
	// Create a new row widget, and return it
	const TSharedRef< STableRow< TSharedPtr<FAssetData> > > ListViewRowWidget =
	 SNew( STableRow< TSharedPtr<FAssetData> >, OwnerTable )
		.Padding( FMargin(5.f) )
		[
			// Create a horizontal box to hold the asset name and other options
			SNew(SHorizontalBox)

			
			// Checkbox to select the asset
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Padding(0.f)
			// .FillWidth(.15f)
			.AutoWidth()
			.MaxWidth(20.f)
			[
				// insert construct checkbox function here
				ConstructCheckBox(AssetData)
			]

			
			// Display the asset class name
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Padding( 15, 0 )
			// .AutoWidth()
			// .MaxWidth(150)
			.FillWidth(.25f)
			[
				ConstructTextForRowWidget( DisplayClassName, AssetClassNameFont )
			]

			
			// Display the asset name
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.Padding( 15, 0 )
			// .FillWidth(.25f)
			[
				// Create a new text block widget, with the asset name
				ConstructTextForRowWidget( DisplayAssetName, AssetNameFont )
			]

			
			/** Construct the Delete Button */
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			.AutoWidth()
			.MaxWidth(450.f)
			.Padding( 10, 0 )
			[
				ConstructButtonWithIcon( 
				"Delete",
				FEditorFontGlyphs::Times,
				DeleteButtonStyle,
				FOnClicked::CreateSP(this, &SAdvancedDeletionTab::OnDeleteButtonClicked, AssetData)
				)
			]

			
		];
	return ListViewRowWidget;
}


/* Settings for Checkbox widget */
TSharedRef<SCheckBox> SAdvancedDeletionTab::ConstructCheckBox(const TSharedPtr<FAssetData> AssetData)
{
	// Create a new checkbox widget, and return it
	TSharedRef<SCheckBox> ConstructedCheckBoxWidget = SNew(SCheckBox)
	.Type(ESlateCheckBoxType::CheckBox) // This is the default type, but I like to be explicit about it
	.OnCheckStateChanged(this, &SAdvancedDeletionTab::OnAssetCheckBoxStateChanged, AssetData) // Bind the OnCheckStateChanged event to the OnAssetCheckBoxStateChanged function
	.Visibility(EVisibility::Visible) // This is the default visibility
	.IsChecked(ECheckBoxState::Unchecked) // This is the default state
	.Clipping(EWidgetClipping::ClipToBoundsAlways)
	
	;

	// Add the checkbox widget to the CheckBoxesArray
	CheckBoxesArray.Add(ConstructedCheckBoxWidget);

	return ConstructedCheckBoxWidget;
}


void SAdvancedDeletionTab::OnAssetCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData)
{
	
	switch (NewState) {
		case ECheckBoxState::Unchecked:
			// Check if the asset is in the array, if so remove it
			if ( AssetDataToDelete.Contains(AssetData) )
			{
				// Remove the asset from the array
				AssetDataToDelete.Remove(AssetData);
			}
			
			break;
		case ECheckBoxState::Checked:
			// Add the AssetData to the AssetDataToDelete array
			AssetDataToDelete.AddUnique( AssetData );
			
			break;
		case ECheckBoxState::Undetermined:
			break;
		default:
			break;
	}
	
}


/** Create a new text block widget, and return it */
TSharedRef<STextBlock> SAdvancedDeletionTab::ConstructTextForRowWidget(const FString TextContent,
	const FSlateFontInfo& TextFont) const
{
	// Create a new text block widget, and return it
	TSharedRef<STextBlock> ConstructedTextBlockWidget = SNew(STextBlock)
	.Text(FText::FromString(TextContent))
	.Font(TextFont)
	.ColorAndOpacity(FColor::White)
	.Justification(ETextJustify::Center)
	.SimpleTextMode(true)
	;
	
	return ConstructedTextBlockWidget;
}


TSharedRef<SButton> SAdvancedDeletionTab::ConstructButtonWithIcon(const FString ButtonText, const FText EditorGlyph,
	const FButtonStyle& InButtonStyle, const FOnClicked& OnClickedDelegate) const
{
	// Get the font for the button text
	FSlateFontInfo ButtonTextFont = FCoreStyle::Get().GetFontStyle("BoldFont");
	ButtonTextFont.Size = 12;

	// Construct a text block widget for the button text
	const TSharedRef<STextBlock> ButtonTextBlock = ConstructTextForRowWidget( ButtonText, ButtonTextFont );
	ButtonTextBlock->SetTextStyle( &DeleteButtonTextStyle );

	/** Construct the button widget with all it's children */
	TSharedRef<SButton> ConstructedButtonWidget = SNew(SButton)
	.ContentPadding(FMargin(0))
	.HAlign( HAlign_Center )
	.VAlign( VAlign_Center )
	.ButtonStyle( &InButtonStyle )
	.ButtonStyle( FAppStyle::Get(), "FlatButton.Danger" )
	.ForegroundColor(FSlateColor::UseForeground())
	.OnClicked( OnClickedDelegate )
	[
		SNew(SHorizontalBox)

		/** Editor Glyph button text */
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign( HAlign_Center )
		.VAlign( VAlign_Center )
		[
			SNew(STextBlock)
			.TextStyle( &DeleteButtonIconStyle )
			.Text( EditorGlyph )
			.Justification( ETextJustify::Center )
		]

		/** Normal button text */
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign( HAlign_Center )
		.VAlign( VAlign_Center )
		[
			ButtonTextBlock
		]
	];

	return ConstructedButtonWidget;
}


/** The OnDeleteButtonClicked function is bound to the OnClicked event of the delete button widget. We are just printing a debug message for now, but this is where we will call the function that will delete the asset. */
FReply SAdvancedDeletionTab::OnDeleteButtonClicked( TSharedPtr<FAssetData> AssetData )
{
	/* Load Editor Extension Module */
	const FUEditorExtensionModule& EditorExtensionModule =
		FModuleManager::LoadModuleChecked<FUEditorExtensionModule>( TEXT("UEditorExtension") );
	
	// DebugHeader::Print( FString::Printf(TEXT("Delete button clicked! %s"), *AssetData->AssetName.ToString()), FColor::Green );
	
	/**
	 * Use the DeleteSingleAsset function in the EditorExtensionModule to delete the selected asset.
	 * @returns true if the asset was deleted successfully, false if it failed.
	 */
	const bool bAssetDelete = EditorExtensionModule.DeleteSingleAsset( *AssetData.Get() );

	/* If the asset was deleted, refresh the list */
	if ( bAssetDelete )
	{
		/* Update the Asset List in the SList Widget */

		// First, check if the Asset is in the Asset List
		if( SelectedAssetData.Contains( AssetData ) )
		{
			// Then, we need to remove the asset from the Asset List
			SelectedAssetData.Remove( AssetData );
			StoredAssetDataArray = SelectedAssetData;
		}

		// Refresh the list
		RefreshAssetListView();
	}
	
	
	return FReply::Handled();
}

#pragma endregion


#pragma region TabBottomButtons

// This function updates the asset list view as the search box is changed
void SAdvancedDeletionTab::OnSearchBoxChanged(const FText& Text)
{
	// Get the text from the search box
	const FString SearchText = Text.ToString();
	DebugHeader::Print( SearchText, FColor::Cyan );
	
	
	// If the search text is empty, and the SelectedAssetDataArray does not match StoredAssetDataArray, then set the SelectedAssetDataArray to the StoredAssetDataArray
	if( SearchText.IsEmpty() )
	{
		if ( SelectedAssetData.Num() < StoredAssetDataArray.Num() )
		{
			SelectedAssetData = StoredAssetDataArray;
			RefreshAssetListView();
		}
		return;
	}
	
	// Create a new array to store the filtered assets
	TArray< TSharedPtr<FAssetData> > FilteredAssetDataArray;
	
	// Loop through the SelectedAssetDataArray and add the assets that match the search text to the FilteredAssetDataArray
	
	for ( const TSharedPtr<FAssetData> AssetData : StoredAssetDataArray )
	{
		if ( AssetData->AssetName.ToString().Contains( SearchText ) )
		{
			FilteredAssetDataArray.Add( AssetData );
			DebugHeader::Print( *AssetData->AssetName.ToString(), FColor::Red );
		}
	}
	
	// Check if the FilteredAssetDataArray is empty
	if ( FilteredAssetDataArray.Num() < 1 )
	{
		// If it is empty, then set the SelectedAssetDataArray to the StoredAssetDataArray
		// SelectedAssetDataArray = StoredAssetDataArray;
		SelectedAssetData.Empty();
	}
	else
	{
		// If it is not empty, then set the SelectedAssetDataArray to the FilteredAssetDataArray
		// SelectedAssetDataArray.Empty();
		SelectedAssetData = FilteredAssetDataArray;
	}
	
	//Refresh the asset list view
	RefreshAssetListView();
}

// Delete the selected assets in the asset list view
FReply SAdvancedDeletionTab::OnDeleteAllButtonClicked()
{
	// If the SelectedAssetDataArray is empty, then notify the user and return
	if( AssetDataToDelete.Num() == 0 )
	{
		DebugHeader::ShowMsgDialog( EAppMsgType::Ok,TEXT("No assets selected!") );
		return FReply::Handled();
	}

	// Convert the SelectedAssetDataArray to an array of FAssetData
	TArray<FAssetData> AssetDataArrayToDelete;
	for ( const TSharedPtr<FAssetData> AssetData : AssetDataToDelete )
	{
		AssetDataArrayToDelete.Add( *AssetData.Get() );
	}
	
	// Load the Editor Extension Module
	FUEditorExtensionModule& EditorExtensionModule =
		FModuleManager::LoadModuleChecked<FUEditorExtensionModule>( TEXT("UEditorExtension") );

	// Pass data to the DeleteMultipleAssets function in the EditorExtensionModule
	const bool bAssetsDelete = EditorExtensionModule.DeleteMultipleAssets( AssetDataArrayToDelete );

	// Check if the assets were deleted
	if ( bAssetsDelete )
	{
		// Remove the deleted assets from the SelectedAssetDataArray
		for(const TSharedPtr<FAssetData>& DeletedAssetData : AssetDataToDelete)
		{
			// First, check if the Asset is in the Asset List
			if( SelectedAssetData.Contains( DeletedAssetData ) )
			{
				// Then, we need to remove the asset from the Asset List
				SelectedAssetData.Remove( DeletedAssetData );
				StoredAssetDataArray.Empty();
				StoredAssetDataArray = SelectedAssetData;
			}
		}

		// Refresh the list
		RefreshAssetListView();
	}

	
	return FReply::Handled();
}

// Select all assets in the asset list view
FReply SAdvancedDeletionTab::OnSelectAllButtonClicked()
{
	if(CheckBoxesArray.Num() == 0) return FReply::Handled();

	// loop through CheckBoxesArray and set the IsChecked state to Checked
	for ( const TSharedPtr<SCheckBox>& CheckBox : CheckBoxesArray )
	{
		// if the CheckBox is checked, then continue to the next CheckBox
		if ( CheckBox->IsChecked() ) continue;
		
		// Set the IsChecked state to Checked
		CheckBox->SetIsChecked( ECheckBoxState::Unchecked );
		CheckBox->ToggleCheckedState();
		
	}
	
	return FReply::Handled();
}

// Deselect all assets in the asset list view
FReply SAdvancedDeletionTab::OnDeselectAllButtonClicked()
{
	if(CheckBoxesArray.Num() == 0) return FReply::Handled();

	// loop through CheckBoxesArray and set the IsChecked state to Unchecked
	for ( const TSharedPtr<SCheckBox>& CheckBox : CheckBoxesArray )
	{
		// if the CheckBox is unchecked, then continue to the next CheckBox
		if ( !CheckBox->IsChecked() ) continue;
		
		// Set the IsChecked state to Unchecked
		CheckBox->SetIsChecked( ECheckBoxState::Checked );
		CheckBox->ToggleCheckedState();
		
	}
	
	return FReply::Handled();
}

#pragma endregion


END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE