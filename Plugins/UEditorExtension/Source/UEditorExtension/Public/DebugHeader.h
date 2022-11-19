#pragma once


/**
 * Print an FString message to the viewport window or screen with a desired FColor.
 * @param Message - The message to print.
 * @param Color - The color to print the message in.
 */
void Print( const FString& Message, const FColor& Color )
{
	if ( GEngine )
	{
		GEngine->AddOnScreenDebugMessage( -1, 8.f, Color, Message );
	}
}

/**
 * Log an FString message to the Output Log window.
 * @param Message The message to log to the Output Log window.
 */
void PrintLog( const FString& Message )
{
	UE_LOG( LogTemp, Warning, TEXT( "%s" ), *Message );
}

// class DebugHeader
// {
// public:
// 	
// };
