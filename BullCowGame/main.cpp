/* 
 * This is the console executable that makes use of the BullCow class
 * This acts a view in the MVC pattern and is responsible for all user
 * interaction. For game logic see the FBullCowGame.
*/
#pragma once
#include <iostream>
#include <string>
#include "FBullCowGame.h"

// to make syntax Unreal friendly
using FText = std::string;
using int32 = int;

void PrintIntro();
void PlayGame();
void PrintGameSummary();
FText GetValidGuess();
bool AskToPlayAgain();
void AskWordLength();
int32 GetValidWordLength();

FBullCowGame BCGame; // instatiate a new game which we reuse across plays

int main()
{
	bool bPlayAgain = false;
	do {
		PrintIntro();
		PlayGame();
		bPlayAgain = AskToPlayAgain();
	}
	while (bPlayAgain);

	return 0;
}

void PrintIntro()
{
	std::cout << std::endl;
	std::cout << "\n   (         )                                 \n";
	std::cout << "\n _  \\ _~~~_ /  _                               \n";
	std::cout << "\n \\_\\ |     | /_/       _ ___ _________^ [ __ ] \n";
	std::cout << "\n    / o   o \\         /     `            \\. .| \n";
	std::cout << "\n    \\       /        |\\                   \\  | \n";
	std::cout << "\n     |  _  |         | \\                 / (__)\n";
	std::cout << "\n     /)   (\\         /  |    _____~~~   /      \n";
	std::cout << "\n    { >---< }           | \\\\__/      | /       \n";
	std::cout << "\n     \\_____/             \\ \\         ||        \n";
	std::cout << "\n                          |_\\        \\_\\       \n";
	std::cout << "\n\n";
	std::cout << "\n\nWelcome to Bulls & Cows, a fun word game!\n\n";
	return;
}

// plays a single game to completion
void PlayGame()
{
	BCGame.Reset();

	AskWordLength();
	
	int32 MaxTries = BCGame.GetMaxTries();

	// loop for the number of guesses while the game is NOT won
	// and there are still tries remaining
	
	int32 i = 0;
	while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= MaxTries)
	{
		FText Guess = GetValidGuess();

		// submit valid guess to the game
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);
		
		// print number of bulls and cows
		std::cout << "Your guess was " << Guess << std::endl;
		std::cout << "Bulls " << BullCowCount.Bulls;
		std::cout << ". Cows " << BullCowCount.Cows << "\n\n";		
	}

	PrintGameSummary();

	return;
}

void AskWordLength() {
	int32 PlayerWordLength = GetValidWordLength();
	BCGame.GenerateHiddenWord(PlayerWordLength);

	std::cout << std::endl << std::endl;
	std::cout << "Can you guess the " << BCGame.GetHiddenWordLength();
	std::cout << " letter isogram I'm thinking of?\n";
	return;
}

int32 GetValidWordLength() {
	FText PlayerWordLength = "";
	EWordLengthStatus Status = EWordLengthStatus::Invalid_Status;
	FWordLengthRange WordLengthRange = BCGame.GetWordLengthRange();

	do
	{
		std::cout << "\nHow long do you want the hidden isogram to be (" << WordLengthRange.Lower << "-" << WordLengthRange.Upper << ")? ";
		getline(std::cin, PlayerWordLength);

		Status = BCGame.CheckWordLengthValidity(PlayerWordLength);
		
		switch (Status)
		{
		case EWordLengthStatus::Wrong_Length:
			std::cout << "Please enter only one digit\n";
			break;
		case EWordLengthStatus::Not_A_Number:
			std::cout << "Please enter a number, not different letters\n";
			break;
		case EWordLengthStatus::Out_Of_Range:
			std::cout << "Please enter a number in the given range\n";
			break;
		}

	} while (Status != EWordLengthStatus::OK);
	
	return std::stoi(PlayerWordLength);
}

// loop continually until user puts in a valid guess
FText GetValidGuess()
{
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	FText Guess = "";

	do
	{
		// get a guess from the player
		std::cout << "Try " << BCGame.GetCurrentTry() << "/" << BCGame.GetMaxTries() << " ";
		std::cout << "Enter your guess: ";

		std::getline(std::cin, Guess);
	

		Status = BCGame.CheckGuessValidity(Guess);
		switch (Status)
		{
		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << " letter word!\n\n";
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter an isogram! Every letter can only be used once!\n\n";
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << "Please enter all lowercase letter!\n\n";
			break;
		default:
			// assume the guess is valid
			break;
		}
	} while (Status != EGuessStatus::OK); // keep looping till we get no errors

	return Guess;
}

bool AskToPlayAgain()
{
	std::cout << "Do you want to play again (y/n)? ";
	FText Response = "";
	std::getline(std::cin, Response);

	char ResponseChar = Response[0];
	return (ResponseChar == 'y'
		|| ResponseChar == 'Y');
}

void PrintGameSummary()
{
	if (BCGame.IsGameWon())
	{
		std::cout << "\n  Congratulations! You've won the game!\n";
	}
	else {
		std::cout << "\n  You've lost! Better luck next time!\n";
	}
	return;
}
