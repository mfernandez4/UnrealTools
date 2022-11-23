// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "RenameAssetUtility.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTOOLS_API URenameAssetUtility : public UAssetActionUtility
{
	GENERATED_BODY()
	
public:

	/**
	 * Rename the selected assets
	 * @param SearchString - the string to search for
	 * @param ReplaceString - the string to replace the SearchString with
	 * @param SearchCase - should the letter case be ignored?
	 **/
	// UFUNCTION(CallInEditor)
	UFUNCTION()
	void RenameSelectedAssets(FString SearchString, FString ReplaceString, ESearchCase::Type SearchCase);

private:

	/**
	 * Prints a message to the screen
	 * @param Message - the message to print
	 * @param Color - the Color to use for the Message
	 **/
	void PrintToScreen(FString Message, FColor Color, float Duration);

	/**
	* Give Feedback to the user
	* @param Method - the Method that was used
	* @param Counter - how many objects were affected
	**/
	void GiveFeedback(FString Method, uint32 Counter, float Duration);
};
