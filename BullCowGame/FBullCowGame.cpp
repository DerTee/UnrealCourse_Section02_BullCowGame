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
	FString HiddenWord = WordLengthToIsogram[PlayerWordLength];
	MyHiddenWord = HiddenWord;
	return;
}

// initalises to an incomplete state because the word length is unknown at this point
// GenerateHiddenWord() MUST be called before any guessing!
void FBullCowGame::Reset()
{
	const FString HIDDEN_WORD = ""; // broken state, that is set because hidden word length is still unknown
	MyHiddenWord = HIDDEN_WORD;

	MyCurrentTry = 1;

	bIsGameWon = false;

	WordLengthToIsogram = {
		{ 3, "sun" },
		{ 4, "glue" },
		{ 5, "plane" },
		{ 6, "beauty" },
		{ 7, "hexagon" }
	};

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
