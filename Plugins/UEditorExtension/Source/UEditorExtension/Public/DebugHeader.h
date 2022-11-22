#pragma once

#include "Misc/MessageDialog.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"

// class DebugHeader
// {
// public:
// 	
// };

/**
 * Print an FString message to the viewport window or screen with a desired FColor.
 * 
 * @param Message - The message to print.
 * @param Color - The color to print the message in.
 */
inline void Print( const FString& Message, const FColor& Color )
{
	if ( GEngine )
	{
		GEngine->AddOnScreenDebugMessage( -1, 8.f, Color, Message );
	}
}

/**
 * Log an FString message to the Output Log window.
 * 
 * @param Message The message to log to the Output Log window.
 */
inline void PrintLog( const FString& Message )
{
	UE_LOG( LogTemp, Warning, TEXT( "%s" ), *Message );
}

/**
 *	Display a message box with a desired message and title.
 *
 *	@param MsgType - The type of message box to display.
 *	@param Message - The message to display in the message box.
 *	@param bShowMsgAsWarning - Whether or not to show the message as a warning.
 *	@return - The result of the message dialog.
 */
inline EAppReturnType::Type ShowMsgDialog( EAppMsgType::Type MsgType, const FString& Message, bool bShowMsgAsWarning = true )
{
	// check if we should show the message as a warning
	if ( bShowMsgAsWarning )
	{
		// create the title for the message dialog box
		const FText MsgTitle = FText::FromString(TEXT("Warning"));
		// Prompt user with a dialog box to confirm the action.
		return  FMessageDialog::Open( MsgType, FText::FromString( Message ), &MsgTitle );
	}
	else
	{
		// Prompt user with a dialog box to confirm the action.
		return  FMessageDialog::Open( MsgType, FText::FromString( Message ) );
	}
}

/**
 *	Display a notification message in the bottom right corner of the screen.
 *
 *	@param Message - The message to display in the notification.
 */
inline void ShowNotifyInfo( const FString& Message )
{
	// create the notification message
	FNotificationInfo NotifyInfo( FText::FromString( Message ) );
	// use large font
	NotifyInfo.bUseLargeFont = true;
	// set the notification message to expire after 7 seconds
	NotifyInfo.FadeOutDuration = 7.0f;

	// add it to the notification list which renders it to the editor
	FSlateNotificationManager::Get().AddNotification( NotifyInfo );
}