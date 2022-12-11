// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidgets/AdvancedDeletionWidget.h"

#include "DebugHeader.h"
#include "EditorFontGlyphs.h"
#include "Styling/SlateStyleMacros.h"
#include "Widgets/Layout/SBackgroundBlur.h"
#include "Widgets/Layout/SScrollBox.h"


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
	// Can the widget ever support keyboard focus
	bCanSupportFocus = true; // This is required for keyboard focus to be passed to this widget

	// Capture the Asset Data from the selected folder. This is the data we will display in the list view.
	SelectedAssetDataArray = InArgs._StoredAssetsDataArray;
	
	// Create Font Info for the text block
	FSlateFontInfo TitleTextFont = GetEmbossedTextFont();
	TitleTextFont.Size = 30;

	ChildSlot
	[
		// Main Vertical Box
		SNew(SVerticalBox)
		.Cursor(EMouseCursor::Crosshairs)

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
			SNew(SHorizontalBox)
		]
		
		// The asset list
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		.Padding(10)
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
			[
				SNew(SListView< TSharedPtr<FAssetData> >)
				.ItemHeight(30.f)
				.ListItemsSource(&SelectedAssetDataArray)
				.OnGenerateRow(this, &SAdvancedDeletionTab::OnGenerateRowForList)
			]
		]

		// Slot for 3 buttons
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
		]

		// Slot for background blur
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBackgroundBlur)
			.BlurStrength(10.f)
			.BlurRadius(10.f)
			.Content()
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("This is a background blur")))
				.Font(TitleTextFont)
				.Justification(ETextJustify::Center)
				.ColorAndOpacity(FColor::White)
			]
		]
	];
}


// Details about this function here.
TSharedRef<ITableRow> SAdvancedDeletionTab::OnGenerateRowForList(TSharedPtr<FAssetData> AssetData, const TSharedRef<STableViewBase>& OwnerTable) const
{
	// Check if the asset data is valid, if not return an empty row widget
	if ( !AssetData.IsValid() ) return SNew( STableRow< TSharedPtr<FAssetData> >, OwnerTable );
	
	const FString& DisplayAssetName = AssetData->AssetName.ToString();
	const FString& DisplayClassName = AssetData->GetClass()->GetName();

	FSlateFontInfo AssetClassNameFont = GetEmbossedTextFont();
	AssetClassNameFont.Size = 12;
	FSlateFontInfo AssetNameFont = FCoreStyle::Get().GetFontStyle("BoldFont");
	AssetNameFont.Size = 12;
	
	const FLinearColor& DeleteButtonColor = FLinearColor(0.3f, 0.8f, 0.3f, 1.f);
	// const FLinearColor& DeleteButtonColor = FLinearColor(0.10616f, 0.48777f, 0.10616f, 1.f);
	
	// Create a new row widget, and return it
	const TSharedRef< STableRow< TSharedPtr<FAssetData> > > ListViewRowWidget =
	 SNew( STableRow< TSharedPtr<FAssetData> >, OwnerTable )
		[
			// Create a horizontal box to hold the asset name and other options
			SNew(SHorizontalBox)

			
			// Checkbox to select the asset
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Padding(5.f)
			// .FillWidth(.15f)
			.AutoWidth()
			.MaxWidth(20.f)
			[
				// insert construct checkbox function here
				ConstructCheckBox(AssetData)
			]

			
			// Display the asset class name
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.Padding( 25, 5 )
			// .AutoWidth()
			// .MaxWidth(250)
			.FillWidth(.1f)
			[
				ConstructTextForRowWidget( DisplayClassName, AssetClassNameFont )
			]

			
			// Display the asset name
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.Padding( 25, 5 )
			.FillWidth(.25f)
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
			.Padding(10, 5)
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
TSharedRef<SCheckBox> SAdvancedDeletionTab::ConstructCheckBox(const TSharedPtr<FAssetData> AssetData) const
{
	// Create a new checkbox widget, and return it
	TSharedRef<SCheckBox> ConstructedCheckBoxWidget = SNew(SCheckBox)
	.Type(ESlateCheckBoxType::CheckBox) // This is the default type, but I like to be explicit about it
	.OnCheckStateChanged(this, &SAdvancedDeletionTab::OnAssetCheckBoxStateChanged, AssetData) // Bind the OnCheckStateChanged event to the OnAssetCheckBoxStateChanged function
	.Visibility(EVisibility::Visible) // This is the default visibility
	.IsChecked(ECheckBoxState::Unchecked) // This is the default state
	.Clipping(EWidgetClipping::ClipToBoundsAlways)
	
	;

	return ConstructedCheckBoxWidget;
}

void SAdvancedDeletionTab::OnAssetCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData) const
{
	switch (NewState) {
		case ECheckBoxState::Unchecked:
			DebugHeader::Print( FString::Printf(TEXT("%s is unchecked!"), *AssetData->AssetName.ToString()), FColor::Red );
			
			break;
		case ECheckBoxState::Checked:
			DebugHeader::Print( FString::Printf(TEXT("%s is checked!"), *AssetData->AssetName.ToString()), FColor::Green );
			
			break;
		case ECheckBoxState::Undetermined:
			break;
		default:
			break;
	}
	
}


/** The OnDeleteButtonClicked function is bound to the OnClicked event of the delete button widget. We are just printing a debug message for now, but this is where we will call the function that will delete the asset. */
FReply SAdvancedDeletionTab::OnDeleteButtonClicked(TSharedPtr<FAssetData> AssetData) const
{
	DebugHeader::Print( FString::Printf(TEXT("Delete button clicked! %s"), *AssetData->AssetName.ToString()), FColor::Green );
	return FReply::Handled();
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
	const TSharedRef<STextBlock> ButtonTextBlock = ConstructTextForRowWidget( TEXT("Delete"), ButtonTextFont );
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



