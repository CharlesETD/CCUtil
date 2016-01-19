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

#include <string>

#ifndef CAESAR_CIPHER_H
#define CAESAR_CIPHER_H

/**
 * Static class with utilities for enciphering, deciphering, and cracking Caesar Ciphers.
 */
class CaesarCipher {

private:
	// Private Static Constants **********************************************
	static const unsigned int	ALPHABET_LENGTH = 26;					/**< Number of letters in the alphabet. */
	static const double			ALPHABET_FREQUENCIES[ALPHABET_LENGTH];	/**< Frequencies each letter occurs in the alphabet. */

public:
	// Public Accessors ******************************************************
	/**
	 * Returns the number of letters in this alphabet.
	 * @return the number of letters in this alphabet.
	 */
	static unsigned int getAlphabetLength (void);

	// Public Methods ********************************************************
	/**
	 * Enciphers the given plaintext using the given key.
	 * @param plaintext to encipher.
	 * @param key to encipher plaintext with.
	 * @return the ciphertext.
	 */
	static std::string encipher (const std::string& plaintext, const unsigned int key);

	/**
	 * Deciphers the given ciphertext using the given key.
	 * @param ciphertext to decipher.
	 * @param key to decipher with. Note this is the same as the key used to encipher.
	 * @return the plaintext.
	 */
	static std::string decipher (const std::string& ciphertext, const unsigned int key);

	/**
	 * Uses statistical analysis to determine the most-likely key used to encipher
	 * the given ciphertext.
	 * @param ciphertext to analyze.
	 * @return the most-likely key used to encipher the given ciphertext. Note
	 * this is not necessarily the right key.
	 * @throw logic_error if for some reason a letter in the alphabet has
	 * a frequency of zero.
	 */
	static unsigned int crackKey (const std::string& ciphertext);

private:
	// Private Methods *******************************************************
	/**
	 * Comuptes chi squared for the given observed and expected values.
	 * @param observedValue.
	 * @param expectedValue, should never be 0.
	 * @return NaN if expectedValue was zero (and NaN is supported), otherwise chi squared.
	 * @throw invalid_argument if NaN was unsupported and expectedValue was zero.
	 */
	static double computeChiSquared (const double observedValue, const double expectedValue);

	/**
	 * Counts how many times each letter in the alphabet is used in the ciphertext
	 * and stores the count in the given array.
	 * @param ciphertext to analyze.
	 * @param letterCounts, array to store letter counts in. Cleared automatically
	 * at start of function call.
	 * @return the total number of letters counted.
	 */
	static unsigned int countLetters (const std::string& ciphertext, unsigned int letterCounts[ALPHABET_LENGTH]);

	/**
	 * Determines the frequency at which each letter in the ciphertext is used
	 * and stores the results in the given array.
	 * @param ciphertext to analyze.
	 * @param letterFrequencies, array to store letter frequencies. Cleared automatically
	 * at the start of the function call.
	 */
	static void computeLetterFrequencies (const std::string& ciphertext, double letterFrequencies[ALPHABET_LENGTH]);

	// Private Ctors *********************************************************
	/**
	 * Private Ctor to prevent class from being instanced.
	 */
	CaesarCipher (void);

	/**
	 * Private copy Ctor to prevent class from being instanced.
	 * @param caesarCipher to copy from.
	 */
	CaesarCipher (const CaesarCipher& caesarCipher);

	// Private Operators *****************************************************
	/**
	 * Private copy assignment operator to prevent class from being instanced.
	 * @param caesarCipher to copy from.
	 * @return this CaesarCipher.
	 */
	CaesarCipher& operator= (const CaesarCipher& caesarCipher);

};

#endif