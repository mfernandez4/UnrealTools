// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidgets/AdvancedDeletionWidget.h"

#include "Widgets/Layout/SScrollBox.h"


void SAdvancedDeletionTab::Construct(const FArguments& InArgs)
{
	// Can the widget ever support keyboard focus
	bCanSupportFocus = true; // This is required for keyboard focus to be passed to this widget

	// Capture the Asset Data from the selected folder. This is the data we will display in the list view.
	SelectedAssetDataArray = InArgs._AssetDataArray;
	
	// Create Font Info for the text block
	FSlateFontInfo TitleTextFont = FCoreStyle::Get().GetFontStyle( FName("EmbossedText") );
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
			SNew(SHorizontalBox)
		]
		
		// The asset list
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SScrollBox)
		]

		// Slot for 3 buttons
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
		]
		
	];
}
