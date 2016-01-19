/**
 * Author:		Charles Duncan (CharlesETD@gmail.com)
 * Created:		Jan 13, 2015
 * Compiler:	MVSE 2012
 * OS:			Windows 7 Home Premium SP1
 *
 * Initial research from:			http://practicalcryptography.com/ciphers/caesar-cipher/
 * Letter frequencies from:			https://en.wikipedia.org/wiki/Letter_frequency#Relative_frequencies_of_letters_in_the_English_language
 * Idea to use chi squared from:	https://en.wikipedia.org/wiki/Caesar_cipher
 * Chi squared formula from:		https://en.wikipedia.org/wiki/Chi-squared_test
 * ASCII reference table from:		http://www.asciitable.com/
 * Various function lookups from:	http://www.cplusplus.com/reference/cctype/toupper/
 *									http://www.cplusplus.com/reference/string/string/reserve/
 *									http://www.cplusplus.com/reference/exception/exception/
 *
 * © Copyright 2016 Charles Duncan (CharlesETD@gmail.com)
 */

#include <cmath>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <string>
#include "CaesarCipher.h"

// Static Constant Definitions ***********************************************
const double CaesarCipher::ALPHABET_FREQUENCIES[CaesarCipher::ALPHABET_LENGTH] = {

	// These frequencies were obtained from:
	// https://en.wikipedia.org/wiki/Letter_frequency#Relative_frequencies_of_letters_in_the_English_language
	0.08167,	// A
	0.01492,	// B
	0.02782,	// C
	0.04253,	// D
	0.12702,	// E
	0.02228,	// F
	0.02015,	// G
	0.06094,	// H
	0.06966,	// I
	0.00153,	// J
	0.00772,	// K
	0.04025,	// L
	0.02406,	// M
	0.06749,	// N
	0.07507,	// O
	0.01929,	// P
	0.00095,	// Q
	0.05987,	// R
	0.06327,	// S
	0.09056,	// T
	0.02758,	// U
	0.00978,	// V
	0.02361,	// W
	0.00150,	// X
	0.01974,	// Y
	0.00074		// Z

};

// Method Definitions ********************************************************
unsigned int CaesarCipher::getAlphabetLength (void) {

	return ALPHABET_LENGTH;

}

//****************************************************************************
std::string CaesarCipher::encipher (const std::string& plaintext, const unsigned int key) {

	// This is the first character of lowercase ASCII.
	const unsigned int ASCII_LOWERCASE = 97;

	std::string ciphertext;
	unsigned int adjustedKey = 0;
	char uppercaseCharacter = '\0';
	char encipheredCharacter = '\0';

	// Adjust key to put it in range [0,ALPHABET_LENGTH].
	adjustedKey = key % ALPHABET_LENGTH;

	// Reserve enough room for the message to avoid reallocation in the following loop.
	ciphertext.reserve (plaintext.length ());

	for (const char character : plaintext) {

		uppercaseCharacter = toupper (character);

		if (uppercaseCharacter >= 65 && uppercaseCharacter <= 90) {

			// This first shifts the character so A is 0. Next it adds the adjusted key
			// and wraps the character back to A if it exceeds Z. Finally, the range
			// is adjusted once more so that the character lines up with ASCII and is
			// appended to the string.
			encipheredCharacter = (((uppercaseCharacter - 65) + adjustedKey) % ALPHABET_LENGTH) + 65;

			// Preserve case of original string by converting it back to lowercase if need be.
			if (character >= ASCII_LOWERCASE) {

				encipheredCharacter = tolower (encipheredCharacter);

			}

			ciphertext += encipheredCharacter;

		} else {

			ciphertext += character;

		}

	}

	return ciphertext;

}

//****************************************************************************
std::string CaesarCipher::decipher (const std::string& ciphertext, const unsigned int key) {

	unsigned int adjustedKey = 0;
	unsigned int decipherKey = 0;
	std::string plaintext;

	// Adjust key to put it in range [0,ALPHABET_LENGTH].
	adjustedKey = key % ALPHABET_LENGTH;

	decipherKey = ALPHABET_LENGTH - adjustedKey;

	plaintext = encipher (ciphertext, decipherKey);

	return plaintext;

}

//****************************************************************************
unsigned int CaesarCipher::crackKey (const std::string& ciphertext) {

	double letterFrequencies[ALPHABET_LENGTH];
	double currentChiSquaredRunningSum = 0.0;
	double lowestChiSquaredSum = std::numeric_limits<double>::max ();
	unsigned int shiftAmountOfLowestChiSquared = 0;
	unsigned int adjustedIndex = 0;

	computeLetterFrequencies (ciphertext, letterFrequencies);

	// Try shifting the observed frequencies until they match up with the
	// expected frequency the closest.
	for (unsigned int shiftAmount = 0; shiftAmount < ALPHABET_LENGTH; shiftAmount++) {

		currentChiSquaredRunningSum = 0.0;

		for (unsigned int i = 0; i < ALPHABET_LENGTH; i++) {

			adjustedIndex = (i + shiftAmount) % ALPHABET_LENGTH;

			if (ALPHABET_FREQUENCIES[i] == 0.0) {

				throw std::logic_error ("Divide by Zero. CaesarCipher::ALPHABET_FREQUENCIES contains at least one frequency of \'0.0\'.");
				return 0;

			}

			currentChiSquaredRunningSum += computeChiSquared (letterFrequencies[adjustedIndex], ALPHABET_FREQUENCIES[i]);

		}

		// The shift ammount with the lowest chi squared sum fits the
		// expected alphabet frequencies best and is therefore more
		// likely to resolve to the correct key.
		if (currentChiSquaredRunningSum < lowestChiSquaredSum) {

			lowestChiSquaredSum = currentChiSquaredRunningSum;
			shiftAmountOfLowestChiSquared = shiftAmount;

		}

	}

	return shiftAmountOfLowestChiSquared;

}

//****************************************************************************
double CaesarCipher::computeChiSquared (const double observedValue, const double expectedValue) {

	if (expectedValue != 0.0) {

		return (std::pow (observedValue - expectedValue, 2)) / expectedValue;

	} else if (std::numeric_limits<double>::has_signaling_NaN) {

		return std::numeric_limits<double>::signaling_NaN ();

	} else if (std::numeric_limits<double>::has_quiet_NaN) {

		return std::numeric_limits<double>::quiet_NaN ();

	}
	
	throw std::invalid_argument ("Divide by zero. \"expectedValue\" must not be zero.");
	return 0.0;

}

//****************************************************************************
unsigned int CaesarCipher::countLetters (const std::string& ciphertext, unsigned int letterCounts[ALPHABET_LENGTH]) {

	unsigned int totalLetters = 0;
	unsigned int characterIndex = 0;
	char uppercaseCharacter = '\0';

	// Clear the array of letter counts.
	memset (letterCounts, 0, sizeof (unsigned int) * ALPHABET_LENGTH);

	for (const char character : ciphertext) {

		uppercaseCharacter = toupper (character);

		// Make sure that the given character is a letter before counting.
		if (uppercaseCharacter >= 65 && uppercaseCharacter <= 90) {

			// Convert character to index
			characterIndex = uppercaseCharacter - 65;

			letterCounts[characterIndex]++;
			totalLetters++;

		}

	}

	return totalLetters;

}

//****************************************************************************
void CaesarCipher::computeLetterFrequencies (const std::string& ciphertext, double letterFrequencies[ALPHABET_LENGTH]) {

	unsigned int letterCounts[ALPHABET_LENGTH];
	unsigned int numberOfLetters = 0;

	numberOfLetters = countLetters (ciphertext, letterCounts);

	for (unsigned int i = 0; i < ALPHABET_LENGTH; i++) {

		// Prevent div by zero errors.
		if (numberOfLetters != 0) {

			letterFrequencies[i] = static_cast<double> (letterCounts[i]) / static_cast<double> (numberOfLetters);

		} else {

			letterFrequencies[i] = 0.0;

		}

	}

}

//****************************************************************************
CaesarCipher::CaesarCipher (void) {

	// Dummmy

}

//****************************************************************************
CaesarCipher::CaesarCipher (const CaesarCipher& caesarCipher) {

	// Dummy

}

//****************************************************************************
CaesarCipher& CaesarCipher::operator= (const CaesarCipher& caesarCipher) {

	// Dummy
	return *this;

}