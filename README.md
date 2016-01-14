# CCUtil: Caesar Cipher Utility
A command line utility for enciphering, deciphering, and cracking Caesar Ciphers.

## UML Diagram
The <a href="https://creately.com/diagram/ijdk9blp1/PtoZQMSObbDN1fG1mCMRB4c7mfQ%3D" target="_blank">UML diagram</a> for this project is hosted at Creately. Note that the diagram is not generated from code and thus changes to the code will not be reflected in the UML or vice versa.

## Compiling
This project has been tested to compile on Windows 7 with Visual Studio 2012 and Linux with gcc version 4.9.2 20150212 (Red Hat 4.9.2-6). Instructions for compiling on both OSs are given below for those who may not know how. If you compile this project on another OS/compiler please let me know which one and the settings so that I may add it to this list.

### Windows | Visual Studio 2012
1. Create a new empty project named "CCUtil".
2. Clone the contents of "CCUtil" from here into the newly made "CCUtil/CCUtil" folder in your Visual Studio Project directory.
3. In Visual Studio right click the "Source Files" directory in the Solution Explorer and select Add->Existing Item.
4. Add both "CaesarCipher.cpp" and "CCUtil.cpp".
5. Add "CaesarCipher.h" to the "Header Files" directory in the Solution Explorer following the same procedure as above.
6. Right click "CCUtil" in the Solution Explorer and select "Properties".
7. Select the "Linker" tab and then the "System" tab.
8. Change "SubSystem" to "Console" and click "Apply" but do not click "Ok" yet.
9. At the top of the window change "Configuration" to "Release" and repeat step 8. You may now click "Ok".
10. Compile the program by pressing F7 and run by pressing F5. Note that by default Visual Studio will compile in Debug mode rather than release. A dropdown menu on the menu bar will allow you to switch between either Release or Debug mode.

### Linux | G++ 4.9.2
1. Clone the "CCUtil" directory to your machine.
2. Make two more directories titled "Release" and "Debug" in the same directory you cloned the "CCUtil" direcotry into.
3. At this point you should have 3 folders "Release", "Debug", and "CCUtil" all inside of your current directory.
4. To compile Debug mode type: g++ -std=c++11 -Og CCUtil/*.cpp -o Debug/CCUtil
5. To compile Release mode type: g++ -std=c++11 -O3 CCUtil/*.cpp -o Release/CCUtil

As a final note, building Debug mode in Visual Studio will cause automated tests to be built in as well. These automated tests run only durring startup of the program in Debug mode. To include these tests in other compilers you must manually define "_DEBUG" for for your Debug builds. These tests are not inclusive and are meant only for basic testing.

## Usage
CCUtil accepts only command line arguments. The following is every possible argument with proper syntax and examples:

### Help
Prints a brief overview of available command line arguments.

| Argument | Syntax    | Example   |
|----------|-----------|-----------|
| `-h`       | `CCUtil -h` | `CCUtil -h` |

### Encipher
Enciphers the given plaintext with the given key. Note that the key must be a positive integer and the plaintext to encipher must be in quotes if it contains spaces.

| Argument | Syntax                                    | Example                                    |
|----------|-------------------------------------------|--------------------------------------------|
| `-e`       | `CCUtil -e "<plaintext to encipher>" <key>` | `CCUtil -e "Command line utils are fun!" 15` |

### Decipher
Deciphers the given ciphertext with the given key. Note that the key must be a positive integer and the ciphertext to decipher must be in quotes if it contains spaces.

| Argument | Syntax                                     | Example                                    |
|----------|--------------------------------------------|--------------------------------------------|
| `-d`       | `CCUtil -d "<ciphertext to decipher>" <key>` | `CCUtil -d "RDBBPCS AXCT JIXAH PGT UJC!" 15` |

### Crack Key
Uses statistical analysis to guess the correct key and decipher the given ciphertext. Note that the ciphertext to analyze must be in quotes if it contains spaces and that the resulting plaintext is only a guess and may not be correct.

| Argument | Syntax                            | Example                                 |
|----------|-----------------------------------|-----------------------------------------|
| `-c`       | `CCUtil -c "<ciphertext to crack>"` | `CCUtil -c "RDBBPCS AXCT JIXAH PGT UJC!"` |

### Brute Force Examination
Prints the results of trying to decipher the given ciphertext with every possible key. Note that the ciphertext to brute force must be in quotes if it contains spaces.

| Argument | Syntax                            | Example                                 |
|----------|-----------------------------------|-----------------------------------------|
| `-b`       | `CCUtil -b "<ciphertext to force>"` | `CCUtil -b "RDBBPCS AXCT JIXAH PGT UJC!"` |

© Copyright 2016 Charles Duncan (CharlesETD@gmail.com)
