// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickAssetAction.h"
#include "DebugHeader.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"

void UQuickAssetAction::TestFunction()
{
	Print( TEXT("Test Function!"), FColor::FromHex("76b900") );
	PrintLog( TEXT("Test Function!") );
}
