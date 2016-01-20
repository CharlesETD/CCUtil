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

#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include "CaesarCipher.h"

// Global Constants **********************************************************
static const char HELP_ARG = 'h';				/**< Command line argument for requesting help text. >*/
static const char ENCIPHER_ARG = 'e';			/**< Command line argument for encrypting. >*/
static const char DECIPHER_ARG = 'd';			/**< Command line argument for decrypting. >*/
static const char CRACK_ARG = 'c';				/**< Command line argument for cracking a cipher key. >*/
static const char BRUTE_FORCE_ARG = 'b';		/**< Command line argument for brute forcing a cipher. >*/
static const char INPUT_FILENAME_ARG = 'i';		/**< Command line argument for loading text from a file. >*/
static const char OUTPUT_FILENAME_ARG = 'o';	/**< Command line argument for specifying a save file. >*/
static const char INPUT_TEXT_ARG = 't';			/**< Command line argument for specifying raw cipher/plain text. >*/

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
bool parseArgs (const int argc, const char* const argv[]);

/**
 * Prints help text to the console.
 */
void printHelp (void);

/**
 * Prints a usage error to the screen.
 */
void printUsageError (void);

/**
 * Attempts to extract an unsinged integer from the given string.
 * @param cString to parse.
 * @param key, where to store the parsed key.
 * @return true if parsed, otherwise false.
 */
bool parseCStringAsKey (const char* const cString, unsigned int& key);

/**
 * Enciphers the given string with the given key and outputs the result to
 * the console.
 * @param plaintext to encipher.
 * @param key to encipher with.
 * @return ciphertext.
 */
std::string encipherAndPrint (const std::string& plaintext, const unsigned int key);

/**
 * Deciphers the given string with the given key and outputs the result to
 * the console.
 * @param ciphertext to decipher.
 * @param key to decipher with.
 * @return plaintext.
 */
std::string decipherAndPrint (const std::string& ciphertext, const unsigned int key);

/**
 * Attempts to guess the correct key for a ciphertext, decipher with it,
 * and print the result to the console.
 * @param ciphertext to analyze and decipher.
 * @return estimated plaintext.
 */
std::string crackAndPrint (const std::string& ciphertext);

/**
 * Prints every possible translation of the given ciphertext to the console.
 * @param ciphertext to brute force.
 * @return every possible plaintext.
 */
std::string bruteForceAndPrint (const std::string& ciphertext);

/**
 * Attempts to load the text of a file.
 * @param filename of the desired file.
 * @param outputString to store file contents.
 * @return true if loaded, otherwise false.
 */
bool loadFileContents (const std::string& filename, std::string& outputString);

/**
 * Attempts to save a string to the given file.
 * @param filename to save to.
 * @param contents to save.
 * @return true if saved, otherwise false.
 */
bool saveFile (const std::string& filename, const std::string& contents);

/**
 * Prints an error reporting a failure to load a file.
 * @param filename of file that failed to load.
 */
void printFileLoadingError (const std::string& filename);

/**
 * Prints an error reporting a failure to save a file.
 * @param filename of file that failed to save.
 */
void printFileSavingError (const std::string& filename);

// Function Definitions ******************************************************
int main (int argc, char* argv[]) {

#ifdef _DEBUG
	std::cout << caesarCipherUnitTests () << std::endl;
#endif

	if (!parseArgs (argc, argv)) {

		return -1;

	}

	return 0;

}

//****************************************************************************
bool parseArgs (const int argc, const char* const argv[]) {

	unsigned int argumentIndex = 0;
	std::string argument;
	char argCharacter = '\0';
	char selectedMode = '\0';
	unsigned int key = 0;
	std::string input;
	std::string output;
	std::string inputFile;
	std::string outputFile;
	bool hasInput = false;
	bool hasOutputFile = false;
	bool hasInputFile = false;
	bool usageErrorOccured = false;

	if (argc <= 1) {

		usageErrorOccured = true;

	} else {

		argumentIndex = 1;

	}

	while (argumentIndex < static_cast<unsigned int> (argc) && !usageErrorOccured) {

		argument = argv[argumentIndex];

		// Is the argument a flag? (-e, -d, etc.)
		if (argument.length () > 1 && argument[0] == '-') {

			argCharacter = argument[1];

			switch (argCharacter) {

			case INPUT_FILENAME_ARG:

				argumentIndex++;

				if (argumentIndex < static_cast<unsigned int> (argc) && !hasInput && selectedMode != HELP_ARG) {

					inputFile = argv[argumentIndex];
					hasInputFile = true;
					hasInput = true;

				} else {

					usageErrorOccured = true;

				}

				break;

			case OUTPUT_FILENAME_ARG:

				argumentIndex++;

				if (argumentIndex < static_cast<unsigned int> (argc) && !hasOutputFile && selectedMode != HELP_ARG) {

					outputFile = argv[argumentIndex];
					hasOutputFile = true;

				} else {

					usageErrorOccured = true;

				}

				break;

			case INPUT_TEXT_ARG:

				argumentIndex++;

				if (argumentIndex < static_cast<unsigned int> (argc) && !hasInput && selectedMode != HELP_ARG) {

					input = argv[argumentIndex];
					hasInput = true;

				} else {

					usageErrorOccured = true;

				}

				break;

			case ENCIPHER_ARG:
			case DECIPHER_ARG:

				// Assume that a mode has not been selected already and check for a valid key.
				// We do not break so that we roll into the next cases and check if our assumption
				// on the selected mode was right, if not we exit anyways.

				argumentIndex++;

				if (!((argumentIndex < static_cast<unsigned int> (argc)) && parseCStringAsKey (argv[argumentIndex], key))) {

					usageErrorOccured = true;
					break;

				}

			case CRACK_ARG:
			case BRUTE_FORCE_ARG:

				if (selectedMode == '\0') {

					selectedMode = argCharacter;
					break;

				} else {

					usageErrorOccured = true;

				}

				break;

			case HELP_ARG:

				if (selectedMode == '\0' && !hasInput && !hasOutputFile && !hasInputFile) {

					selectedMode = argCharacter;
					break;

				} else {

					usageErrorOccured = true;

				}

				break;

			default:

				usageErrorOccured = true;
				break;

			}

		} else {

			// Not enough arguments.
			usageErrorOccured = true;

		}

		argumentIndex++;
		
	}

	if (usageErrorOccured) {

		printUsageError ();
		return false;

	}

	if (hasInputFile) {

		if (!loadFileContents (inputFile, input)) {

			printFileLoadingError (inputFile);
			return false;

		}

	}

	switch (selectedMode) {

	case HELP_ARG:
		printHelp ();
		break;

	case ENCIPHER_ARG:
		output = encipherAndPrint (input, key);
		break;

	case DECIPHER_ARG:
		output = decipherAndPrint (input, key);
		break;

	case BRUTE_FORCE_ARG:
		output = bruteForceAndPrint (input);
		break;

	case CRACK_ARG:
		output = crackAndPrint (input);
		break;

		// Shouldn't happen.
	default:
		printUsageError ();
		break;

	}

	if (hasOutputFile) {

		if (!saveFile (outputFile, output)) {

			printFileSavingError (outputFile);
			return false;

		}

	}

	return true;

}

//****************************************************************************
bool parseCStringAsKey (const char* const cString, unsigned int& key) {

	int rawInt = 0;

	try {

		rawInt = std::stoi (cString);
		
		if (rawInt < 0) {

			return false;

		}

		// Though not necessary, wrapping the key to the proper domain
		// is good practice.
		key = rawInt % CaesarCipher::getAlphabetLength ();
		return true;

	} catch (...) {

		return false;

	}

	return false;

}

//****************************************************************************
void printFileLoadingError (const std::string& filename) {

	std::cout << "Failed to load file \"" << filename << "\"." << std::endl;

}

//****************************************************************************
void printFileSavingError (const std::string& filename) {

	std::cout << "Failed to save to file \"" << filename << "\"." << std::endl;

}

//****************************************************************************
void printUsageError (void) {

	std::cout << "Invalid arguments. For usage help, type: CCUtil -" << HELP_ARG << std::endl;

}

//****************************************************************************
void printHelp (void) {

	std::cout << "To encipher, type:\t\t\tCCUtil -" << ENCIPHER_ARG << " KEY" << std::endl
			  << "To decipher, type:\t\t\tCCUtil -" << DECIPHER_ARG << " KEY" << std::endl
			  << "To crack key, type:\t\t\tCCUtil -" << CRACK_ARG << std::endl
			  << "To brute force, type:\t\t\tCCUtil -" << BRUTE_FORCE_ARG << std::endl
			  << "To get help, type:\t\t\tCCUtil -" << HELP_ARG << std::endl
			  << "To specify input text, append:\t\t-" << INPUT_TEXT_ARG << " \"TEXT\"" << std::endl
			  << "To specify input file, append:\t\t-" << INPUT_FILENAME_ARG << " \"FILENAME\"" << std::endl
			  << "To specify output file, append:\t\t-" << OUTPUT_FILENAME_ARG << " \"FILENAME\"" << std::endl
			  << "Note: KEY must be a positive integer and text/filenames should be in quotes." << std::endl;

}

//****************************************************************************
std::string encipherAndPrint (const std::string& plaintext, const unsigned int key) {

	std::string ciphertext;

	ciphertext = CaesarCipher::encipher (plaintext, key);

	std::cout << "Plaintext:\t" << plaintext << std::endl
			  << "Ciphertext:\t" << ciphertext << std::endl
			  << "Key:\t\t" << key << std::endl;

	return ciphertext;

}

//****************************************************************************
std::string decipherAndPrint (const std::string& ciphertext, const unsigned int key) {

	std::string plaintext;

	plaintext = CaesarCipher::decipher (ciphertext, key);

	std::cout << "Ciphertext:\t" << ciphertext << std::endl
			  << "Plaintext:\t" << plaintext << std::endl
			  << "Key:\t\t" << key << std::endl;

	return plaintext;

}

//****************************************************************************
std::string crackAndPrint (const std::string& ciphertext) {

	unsigned int estimatedKey = 0;
	std::string plaintext;

	try {

		estimatedKey = CaesarCipher::crackKey (ciphertext);

	} catch (const std::logic_error& e) {

		// While it is technically possible for any std::logic_error to be caught here,
		// it is highly likely that the exception is for an invalid alphabet frequency
		// and even if it wasn't, there is no reason to report it to the user.
		std::cout << "Internal Error: " << e.what () << std::endl << "Please report this error to the supplier of this utility." << std::endl;
		return "Internal Error. " + std::string (e.what ()) + "Please report this error to the supplier of this utility.";

	}

	plaintext = CaesarCipher::decipher (ciphertext, estimatedKey);

	std::cout << "Ciphertext:\t\t" << ciphertext << std::endl
			  << "Most Likely Plaintext:\t" << plaintext << std::endl
			  << "Estimated Key:\t\t" << estimatedKey << std::endl
			  << "If the plaintext does not look correct, try a brute force crack (-" << BRUTE_FORCE_ARG << ") of all possible translations." << std::endl;

	return plaintext;

}

//****************************************************************************
std::string bruteForceAndPrint (const std::string& ciphertext) {

	std::string plaintext;
	std::string output;
	output.reserve (ciphertext.length () * CaesarCipher::getAlphabetLength ());
	plaintext.reserve (ciphertext.length ());

	std::cout << "Key\tPlaintext" << std::endl;

	for (unsigned int key = 0; key < CaesarCipher::getAlphabetLength (); key++) {

		plaintext = CaesarCipher::decipher (ciphertext, key);

		// A width of 3 is used to pad the key value out to the end of the column
		// under the word "Key" in the header.
		std::cout << std::setw (3) << key << '\t' << plaintext << std::endl;
		output += plaintext + '\n';

	}

	std::cout << "If multiple keys generate plausible plaintext try cracking the key (-" << CRACK_ARG << ") to see the most statistically likely translation." << std::endl;
	return output;

}

//****************************************************************************
bool loadFileContents (const std::string& filename, std::string& outputString) {

	std::ifstream inputStream;

	try {

		inputStream.open (filename);

		outputString.clear ();

		if (!inputStream.good ()) {

			inputStream.close ();
			return false;

		}

		while (inputStream.good ()) {

			// This is pretty inefficient; however, the goal of this program
			// is not to process large files but rather simple strings so
			// this is a case of not overdesigning for the intended purpose.
			outputString += inputStream.get ();

		}

		inputStream.close ();
		return true;

	} catch (...) {

		try {

			inputStream.close ();

		} catch (...) {

			return false;

		}

		return false;

	}

	return false;

}

//****************************************************************************
bool saveFile (const std::string& filename, const std::string& contents) {

	std::ofstream outputStream;

	try {

		outputStream.open (filename);

		if (outputStream.good ()) {

			outputStream << contents;

		} else {

			outputStream.close ();
			return false;

		}

		outputStream.close ();
		return true;

	} catch (...) {

		try {

			outputStream.close ();

		} catch (...) {

			return false;

		}

		return false;

	}

	return false;

}

//****************************************************************************
std::string caesarCipherUnitTests () {

	// I would have used "Hello World!" however that phrase is not statistically
	// common and does not result in finding the correct key when cracking. This
	// phrase is more common and results in the correct key.
	const std::string TEST_PLAINTEXT = "attack at Dawn!";
	const std::string TEST_CIPHERTEXT = "leelnv le Olhy!";
	const std::string EMPTY_TEXT = "";
	const unsigned int TEST_KEY = 11;
	const unsigned int TEST_KEY_LONG = 271;

	if (CaesarCipher::encipher (TEST_PLAINTEXT, TEST_KEY) != TEST_CIPHERTEXT) {

		return "Failed to encipher string.";

	}

	if (CaesarCipher::decipher (TEST_CIPHERTEXT, TEST_KEY) != TEST_PLAINTEXT) {

		return "Failed to decipher string.";

	}

	if (CaesarCipher::crackKey (TEST_CIPHERTEXT) != TEST_KEY) {

		return "Failed to determine key.";

	}

	if (CaesarCipher::encipher (TEST_PLAINTEXT, TEST_KEY_LONG) != TEST_CIPHERTEXT) {

		return "Failed to encipher string with long key.";

	}

	if (CaesarCipher::decipher (TEST_CIPHERTEXT, TEST_KEY_LONG) != TEST_PLAINTEXT) {

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