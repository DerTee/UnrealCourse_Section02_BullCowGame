/*
* This class is responsible for the game logic,
* so it's responsibility is to provide words to guess,
* enforce the rules for guessing, check win / lose condition etc.
* No view code or direct user interaction.
*
* For the view layer see main.cpp
*/

#pragma once
#include <string>
#include <map>
#define TMap std::map

// to make syntax Unreal friendly
using FString = std::string;
using int32 = int;

struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

struct FWordLengthRange
{
	int32 Lower = 0;
	int32 Upper = 0;
};

enum class EWordLengthStatus
{
	Invalid_Status,
	OK,
	Not_A_Number,
	Out_Of_Range,
	Wrong_Length,
};

enum class EGuessStatus
{
	Invalid_Status,
	OK,
	Not_Isogram,
	Wrong_Length,
	Not_Lowercase
};

class FBullCowGame {
public:
	FBullCowGame(); // constructor

	int32 GetMaxTries() const;
	int32 GetCurrentTry() const;
	int32 GetHiddenWordLength() const;
	bool IsGameWon() const;

	EGuessStatus CheckGuessValidity(FString) const;
	EWordLengthStatus CheckWordLengthValidity(FString) const;
	FWordLengthRange GetWordLengthRange() const;

	void GenerateHiddenWord(int32);
	void Reset();

	// provide a method for counting bulls and cows and increasing turn number
	FBullCowCount SubmitValidGuess(FString);
	
private:
	// initialisation in constructor
	int32 MyCurrentTry;
	int32 MyMaxTries;
	FString MyHiddenWord;
	bool bIsGameWon;
	TMap<int32, FString> WordLengthToIsogram;
	
	bool IsIsogram(FString) const;
	bool IsLowercase(FString) const;
};
