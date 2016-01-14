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

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include "CaesarCipher.h"

// Function Declarations *****************************************************
/**
 * Entry point of CCUtil.
 * @param argc, number of command line arguments passed.
 * @param argv, value of command line arguments passed.
 * @return 0 if no errors occured, otherwise an error occured.
 */
int main (int argc, char* argv[]);

/**
 * Runs unit tests on the static class CaesarCipher.
 * @return whether unit tests have passed or failed.
 */
std::string caesarCipherUnitTests ();

/**
 * Parses the given command line arguments.
 * @param argc, number of command line arguments.
 * @param argv, values of command line arguments.
 * @return true if commands parsed, otherwise false.
 */
bool parseArguments (int argc, char* argv[]);

/**
 * Attempts to get a cipher key from a command line argument.
 * @param argument to parse key from.
 * @return -1 if key could not be parsed, otherwise an unsigned key is returned.
 */
int parseKeyFromArg (const char argument[]);

/**
 * Enciphers the given string with the given key and outputs the result to
 * the console.
 * @param plaintext to encipher.
 * @param key to encipher with.
 */
void encipherAndPrint (const std::string& plaintext, const unsigned int key);

/**
 * Deciphers the given string with the given key and outputs the result to
 * the console.
 * @param ciphertext to decipher.
 * @param key to decipher with.
 */
void decipherAndPrint (const std::string& ciphertext, const unsigned int key);

/**
 * Attempts to guess the correct key for a ciphertext, decipher with it,
 * and print the result to the console.
 * @param ciphertext to analyze and decipher.
 */
void crackAndPrint (const std::string& ciphertext);

/**
 * Prints every possible translation of the given ciphertext to the console.
 * @param ciphertext to brute force.
 */
void bruteForceAndPrint (const std::string& ciphertext);

// Function Definitions ******************************************************
int main (int argc, char* argv[]) {

#ifdef _DEBUG
	std::cout << caesarCipherUnitTests () << std::endl;
#endif

	if (!parseArguments (argc, argv)) {

		return -1;

	}

	return 0;

}

//****************************************************************************
bool parseArguments (int argc, char* argv[]) {

	const std::string HELP_ARG = "-h";
	const std::string ENCIPHER_ARG = "-e";
	const std::string DECIPHER_ARG = "-d";
	const std::string CRACK_ARG = "-c";
	const std::string BRUTE_FORCE_ARG = "-b";

	int key = 0;

	if (argc > 1) {

		if (argc == 2 && argv[1] == HELP_ARG) {

			std::cout << "To encipher, type:\tCCUtil -e \"PLAINTEXT TO ENCIPHER\" KEY" << std::endl
					  << "To decipher, type:\tCCUtil -d \"CIPHERTEXT TO DECIPHER\" KEY" << std::endl
					  << "To crack key, type:\tCCUtil -c \"CIPHERTEXT TO CRACK\"" << std::endl
					  << "To brute force, type:\tCCUtil -b \"CIPHERTEXT TO BRUTE FORCE\"" << std::endl
					  << "To get help, type:\tCCUtil -h" << std::endl
					  << "Note: KEY must be a positive integer." << std::endl;
			return true;

		} if (argc == 3 && argv[1] == CRACK_ARG) {
		
			crackAndPrint (argv[2]);
			return true;
		
		} else if (argc == 3 && argv[1] == BRUTE_FORCE_ARG) {

			bruteForceAndPrint (argv[2]);
			return true;

		} else if (argc == 4 && (argv[1] == ENCIPHER_ARG || argv[1] == DECIPHER_ARG)) {
			
			key = parseKeyFromArg (argv[3]);

			if (key < 0) {

				// Do not add newline. This way the usage text is on the same line
				// as this message.
				std::cout << "KEY must be a positive integer. ";

			} else if (argv[1] == ENCIPHER_ARG) {

				encipherAndPrint (argv[2], static_cast<unsigned int> (key));
				return true;

			} else if (argv[1] == DECIPHER_ARG) {

				decipherAndPrint (argv[2], static_cast<unsigned int> (key));
				return true;

			}
			
		} else {

			// Do not add newline. This way the usage text is on the same line
			// as this message.
			std::cout << "Invalid or missing argument(s). ";

		}

	}

	std::cout << "For usage help, type: CCUtil -h" << std::endl;
	return false;

}

//****************************************************************************
int parseKeyFromArg (const char argument[]) {

	int key = 0;

	try {

		key = std::stoi (argument);
		
		if (key >= 0) {

			// Though not necessary, wrapping the key to the proper domain
			// is good practice.
			key = key % CaesarCipher::getAlphabetLength ();

		} else {

			// Any error results in returning a sentinel value.
			key = -1;

		}

	} catch (...) {

		// Any error results in returning a sentinel value.
		key = -1;

	}

	return key;

}

//****************************************************************************
void encipherAndPrint (const std::string& plaintext, const unsigned int key) {

	std::string ciphertext;

	ciphertext = CaesarCipher::encipher (plaintext, key);

	std::cout << "Plaintext:\t" << plaintext << std::endl
			  << "Ciphertext:\t" << ciphertext << std::endl
			  << "Key:\t\t" << key << std::endl;

}

//****************************************************************************
void decipherAndPrint (const std::string& ciphertext, const unsigned int key) {

	std::string plaintext;

	plaintext = CaesarCipher::decipher (ciphertext, key);

	std::cout << "Ciphertext:\t" << ciphertext << std::endl
			  << "Plaintext:\t" << plaintext << std::endl
			  << "Key:\t\t" << key << std::endl;

}

//****************************************************************************
void crackAndPrint (const std::string& ciphertext) {

	unsigned int estimatedKey = 0;
	std::string plaintext;

	try {

		estimatedKey = CaesarCipher::crackKey (ciphertext);

	} catch (const std::logic_error& e) {

		// While it is technically possible for any std::exception to be caught here,
		// it is highly likely that the exception is for an invalid alphabet frequency
		// and even if it wasn't there is no reason to report it to the user.
		std::cout << "Internal Error: " << e.what () << std::endl << "Please report this error to the supplier of this utility." << std::endl;
		return;

	}

	plaintext = CaesarCipher::decipher (ciphertext, estimatedKey);

	std::cout << "Ciphertext:\t\t" << ciphertext << std::endl
			  << "Most Likely Plaintext:\t" << plaintext << std::endl
			  << "Estimated Key:\t\t" << estimatedKey << std::endl
			  << "If the plaintext does not look correct, try a brute force crack (-b) of all possible translations." << std::endl;

}

//****************************************************************************
void bruteForceAndPrint (const std::string& ciphertext) {

	std::string plaintext;
	plaintext.reserve (ciphertext.length ());

	std::cout << "Key\tPlaintext" << std::endl;

	for (unsigned int key = 0; key < CaesarCipher::getAlphabetLength (); key++) {

		plaintext = CaesarCipher::decipher (ciphertext, key);

		// A width of 3 is used to pad the key value out to the end of the column
		// under the word "Key" in the header.
		std::cout << std::setw (3) << key << '\t' << plaintext << std::endl;

	}

	std::cout << "If multiple keys generate plausible plaintext try cracking the key (-c) to see the most statistically likely translation." << std::endl;

}

//****************************************************************************
std::string caesarCipherUnitTests () {

	// I would have used "Hello World!" however that phrase is not statistically
	// common and does not result in finding the correct key when cracking. This
	// phrase is more common and results in the correct key.
	const std::string TEST_PLAINTEXT = "attack at Dawn!";
	const std::string TEST_PLAINTEXT_U = "ATTACK AT DAWN!";
	const std::string TEST_CIPHERTEXT = "leelnv le Olhy!";
	const std::string TEST_CIPHERTEXT_U = "LEELNV LE OLHY!";
	const std::string EMPTY_TEXT = "";
	const unsigned int TEST_KEY = 11;
	const unsigned int TEST_KEY_LONG = 271;

	if (CaesarCipher::encipher (TEST_PLAINTEXT, TEST_KEY) != TEST_CIPHERTEXT_U) {

		return "Failed to encipher string.";

	}

	if (CaesarCipher::decipher (TEST_CIPHERTEXT, TEST_KEY) != TEST_PLAINTEXT_U) {

		return "Failed to decipher string.";

	}

	if (CaesarCipher::crackKey (TEST_CIPHERTEXT) != TEST_KEY) {

		return "Failed to determine key.";

	}

	if (CaesarCipher::encipher (TEST_PLAINTEXT, TEST_KEY_LONG) != TEST_CIPHERTEXT_U) {

		return "Failed to encipher string with long key.";

	}

	if (CaesarCipher::decipher (TEST_CIPHERTEXT, TEST_KEY_LONG) != TEST_PLAINTEXT_U) {

		std::cout << CaesarCipher::decipher (TEST_CIPHERTEXT, TEST_KEY_LONG) << std::endl;

		return "Failed to decipher string with long key.";

	}

	if (CaesarCipher::crackKey (TEST_CIPHERTEXT) != TEST_KEY) {

		return "Failed to determine long key.";

	}

	if (CaesarCipher::encipher (EMPTY_TEXT, TEST_KEY) != EMPTY_TEXT) {

		return "Failed to encipher empty string.";

	}

	if (CaesarCipher::decipher (EMPTY_TEXT, TEST_KEY) != EMPTY_TEXT) {

		return "Failed to decipher empty string.";

	}

	if (CaesarCipher::crackKey (EMPTY_TEXT) != 0) {

		return "Failed to determine key from empty string.";

	}

	return "All unit tests passed.";

}