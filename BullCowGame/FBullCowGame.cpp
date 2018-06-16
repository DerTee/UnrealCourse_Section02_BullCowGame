/*
 * This class is responsible for the game logic,
 * so it's responsibility is to provide words to guess,
 * enforce the rules for guessing, check win / lose condition etc.
 *
 * It is the controller in the MVC pattern.
 * For the view layer see main.cpp
 */

#pragma once
#include "FBullCowGame.h"


FBullCowGame::FBullCowGame() { Reset(); } // default constructur

int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
bool FBullCowGame::IsGameWon() const { return bIsGameWon; }

int32 FBullCowGame::GetMaxTries() const {
	TMap<int32, int32> WordLengthToMaxTries{ {3,4}, {4,7}, {5,10}, {6,15}, {7, 20} };
	return WordLengthToMaxTries[MyHiddenWord.length()];
}

// Uses given wordlength to generate a hidden word. Has to be called before any guessing.
void FBullCowGame::GenerateHiddenWord(int32 PlayerWordLength)
{
	FString HiddenWord = GetIsogramOfLength(PlayerWordLength);
	MyHiddenWord = HiddenWord;
	return;
}

void FBullCowGame::Reset()
{
	const FString HIDDEN_WORD = "parent";
	MyHiddenWord = HIDDEN_WORD;

	MyCurrentTry = 1;

	bIsGameWon = false;

	// WordLengthToIsogram

	return;
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (Guess.length() != GetHiddenWordLength())
	{
		return EGuessStatus::Wrong_Length;
	}
	else if (!IsLowercase(Guess))
	{
		return EGuessStatus::Not_Lowercase;
	}
	else if (!IsIsogram(Guess))
	{
		return EGuessStatus::Not_Isogram;
	}
	else
	{
		return EGuessStatus::OK;
	}
}

EWordLengthStatus FBullCowGame::CheckWordLengthValidity(FString PlayerWordLength) const
{
	// we don't use isograms longer than 9, so we just want one char
	if (PlayerWordLength.length() != 1)
	{
		return EWordLengthStatus::Wrong_Length;
	}
	else if (!isdigit(PlayerWordLength[0]))
	{
		return EWordLengthStatus::Not_A_Number;
	}
	else if (std::stoi(PlayerWordLength) > 7 || std::stoi(PlayerWordLength) < 3)
	{
		return EWordLengthStatus::Out_Of_Range;
	}
	else
	{
		return EWordLengthStatus::OK;
	}
}

FWordLengthRange FBullCowGame::GetWordLengthRange() const
{
	FWordLengthRange Range;
	// TODO get bounds by checking the internal isogram "library"
	Range.Lower = 3;
	Range.Upper = 7;
	return Range;
}

// receives a VALID guess, increments turn, returns count
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTry++;
	FBullCowCount BullCowCount;
	int32 WordLength = MyHiddenWord.length(); // assuming same length as guess

	// loop through all letters in the hidden word
	for (int32 GuessIter = 0; GuessIter < WordLength; GuessIter++)
	{
		char GuessedChar = Guess[GuessIter];
		for (int32 HiddenIter = 0; HiddenIter < WordLength; HiddenIter++)
		{
			char HiddenChar = MyHiddenWord[HiddenIter];
			if (GuessedChar == HiddenChar)
			{
				if (GuessIter == HiddenIter)
				{
					BullCowCount.Bulls++;
				}
				else {
					BullCowCount.Cows++;
				}
			}
		}
	}
	
	if(BullCowCount.Bulls == WordLength) 
	{
		bIsGameWon = true;
	}

	return BullCowCount;
}

bool FBullCowGame::IsIsogram(FString Word) const
{
	// treat 0 or one letter strings as isograms
	if (Word.length() <= 1) { return true; }

	TMap<char, bool> LetterSeen;

	for (auto Letter : Word)
	{
		Letter = tolower(Letter);
		if (LetterSeen[Letter])
		{ 
			return false;
		}
		else { 
			LetterSeen[Letter] = true; 
		}
	}
	return true;
}

bool FBullCowGame::IsLowercase(FString Word) const
{
	FString LowercaseWord = "";
	for (auto Letter : Word)
	{
		if (!islower(Letter)) { return false; }
	}
	return true;
}

FString FBullCowGame::GetIsogramOfLength(int32 Length)
{
	FString Isogram = WordLengthToIsogram[Length];
	return Isogram;
}
