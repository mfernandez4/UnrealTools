﻿#pragma once

#include "Misc/MessageDialog.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"

// class DebugHeader
// {
// public:
// 	
// };

namespace DebugHeader
{
	/**
	 * Print an FString message to the viewport window or screen with a desired FColor.
	 * 
	 * @param Message - The message to print.
	 * @param Color - The color to print the message in.
	 */
	static void Print(const FString& Message, const FColor& Color)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 8.f, Color, Message);
		}
	}

	/**
	 * Log an FString message to the Output Log window.
	 * 
	 * @param Message The message to log to the Output Log window.
	 */
	static void PrintLog(const FString& Message)
	{
		UE_LOG(LogTemp, Warning, TEXT( "%s" ), *Message);
	}

	/**
	 *	Display a message box with a desired message and title.
	 *
	 *	@param MsgType - The type of message box to display.
	 *	@param Message - The message to display in the message box.
	 *	@param bShowMsgAsWarning - Whether or not to show the message as a warning.
	 *	@return - The result of the message dialog.
	 */
	static EAppReturnType::Type ShowMsgDialog(EAppMsgType::Type MsgType, const FString& Message,
	                                          bool bShowMsgAsWarning = true)
	{
		// check if we should show the message as a warning
		if (bShowMsgAsWarning)
		{
			// create the title for the message dialog box
			const FText MsgTitle = FText::FromString(TEXT("Warning"));
			// Prompt user with a dialog box to confirm the action.
			return FMessageDialog::Open(MsgType, FText::FromString(Message), &MsgTitle);
		}
		else
		{
			// Prompt user with a dialog box to confirm the action.
			return FMessageDialog::Open(MsgType, FText::FromString(Message));
		}
	}

	/**
	 *	Display a notification message in the bottom right corner of the screen.
	 *
	 *	@param Message - The message to display in the notification.
	 */
	static void ShowNotifyInfo(const FString& Message)
	{
		// create the notification message
		FNotificationInfo NotifyInfo(FText::FromString(Message));
		// use large font
		NotifyInfo.bUseLargeFont = true;
		// set the notification message to fadeIn to complete within 5 seconds
		NotifyInfo.FadeInDuration = 2.0f;
		// set the notification to expire after 20 seconds before fading out
		NotifyInfo.ExpireDuration = 20.0f;
		// set the notification message to fadeout after 5 seconds
		NotifyInfo.FadeOutDuration = 5.0f;

		// add it to the notification list which renders it to the editor
		FSlateNotificationManager::Get().AddNotification(NotifyInfo);
	}
}

