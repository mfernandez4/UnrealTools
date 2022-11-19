// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTools/Tools/RenameAssetUtility.h"
#include "EditorUtilityLibrary.h"


#pragma region RenameSelectedAssets

void URenameAssetUtility::RenameSelectedAssets(FString SearchString, FString ReplaceString, ESearchCase::Type SearchCase)
{
	// Check if something needs to be done
	if (SearchString.IsEmpty() || SearchString.Equals(ReplaceString, SearchCase))
	{
		return;
	}
	
	// Get the selected objects
	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();
	
	uint32 Counter = 0;

	// Check each Asset if it needs to be renamed
	for (UObject* SelectedObject : SelectedObjects)
	{
		// Check if SelectedObject pointer is not nullptr
		if (SelectedObject != nullptr)
		{
			// Get Asset name
			FString AssetName = SelectedObject->GetName();

			// Check if Asset name contains SearchString
			if (AssetName.Contains(SearchString, SearchCase))
			{
				// Form new name and store in a variable
				FString NewName = AssetName.Replace(*SearchString, *ReplaceString, SearchCase);
				UEditorUtilityLibrary::RenameAsset(SelectedObject, NewName);
				
				// increment counter variable
				Counter++;
			}
		}
	}

	GiveFeedback(TEXT("Renamed"), Counter, 5.f);
}

#pragma endregion



#pragma region Helper

void URenameAssetUtility::PrintToScreen(FString Message, FColor Color, float Duration)
{
	/// Display a message to the user
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			Duration,
			Color,
			Message
		);
	}
}

void URenameAssetUtility::GiveFeedback(FString Method, uint32 Counter, float Duration)
{
	FString Message = FString("No matching files found.");
	FColor Color = Counter > 0 ? FColor::Green : FColor::Red;
	if (Counter > 0)
	{
		Message = Method.Append(" ").Append(FString::FromInt(Counter)).Append( Counter == 1 ? TEXT(" file") : TEXT(" files") ).Append(" renamed.");
	}
	PrintToScreen(Message, Color, Duration);
	
}

#pragma endregion