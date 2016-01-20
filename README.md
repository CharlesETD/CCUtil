# CCUtil: Caesar Cipher Utility
A command line utility for enciphering, deciphering, and cracking Caesar Ciphers. Created for an assignment in CS4243/CS5243 "Algorithms and Processes in Computer Security" at Oklahoma State university for Dr. Eric Chan-Tin.

## Doxygen
This project has been commented with Doxygen compatible comments. The resulting HTML files from Doxygen have been omitted from GitHub as they are not strictly part of the source code and are rather bulky; however, running Doxygen (optimized for C++) on this project's source code will produce a complete documentation in HTML. For more on Doxygen please visit their <a href="http://www.stack.nl/~dimitri/doxygen/" target="_blank">website</a>.

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
2. Make two more directories titled "Release" and "Debug" in the same directory you cloned the "CCUtil" directory into.
3. At this point you should have 3 folders "Release", "Debug", and "CCUtil" all inside of your current directory.
4. To compile Debug mode type: g++ -std=c++11 -Og CCUtil/*.cpp -o Debug/CCUtil
5. To compile Release mode type: g++ -std=c++11 -O3 CCUtil/*.cpp -o Release/CCUtil

As a final note, building Debug mode in Visual Studio will cause automated tests to be built in as well. These automated tests run only during startup of the program in Debug mode. To include these tests in other compilers you must manually define "_DEBUG" for your Debug builds. These tests are not inclusive and are meant only for basic testing.

## Usage
CCUtil accepts only command line arguments. The following is every possible argument with proper syntax and examples:

### Help
Prints a brief overview of available command line arguments. Does not support input files, specified text, or output files.

| Argument | Syntax    | Example   |
|----------|-----------|-----------|
| `-h`     | `CCUtil -h` | `CCUtil -h` |

### Specify Text
Specifies the text to be enciphered, deciphered, etc. This command must be paired with a command such as (-e). Cannot be paired with loaded text from input file. Note that the text must be in quotes if it contains spaces.

| Argument | Syntax                                    | Example                                    |
|----------|-------------------------------------------|--------------------------------------------|
| `-t`       | `-t "<text>"` | `CCUtil -e 15 -t "Command line utils are fun!"` |

### Load Text from Input File
Loads the text to be enciphered, deciphered, etc. from the given file. This command must be paired with a command such as (-e). Cannot be paired with specified text. Note that the filename must be in quotes if it contains spaces.

| Argument | Syntax                                    | Example                                    |
|----------|-------------------------------------------|--------------------------------------------|
| `-i`       | `-i "<filepath and name>"` | `CCUtil -e 22 -i "C:\Users\user\Desktop\text.txt"` |

### Save Output to File
Saves the results of the given operation to the given file. This command must be paired with a command such as (-e). Note that the filename must be in quotes if it contains spaces.

| Argument | Syntax                                    | Example                                    |
|----------|-------------------------------------------|--------------------------------------------|
| `-o`       | `-o "<filepath and name>"` | `CCUtil -d 12 -t "Hello World!" -o "C:\Users\user\Desktop\test.txt"` |

### Encipher
Enciphers the provided text or file with the given key. Requires text or input file to be specified. Output file is optional. Note that the key must be a positive integer.

| Argument | Syntax                                    | Example                                    |
|----------|-------------------------------------------|--------------------------------------------|
| `-e`       | `CCUtil -e <key>` | `CCUtil -e 15 -t "Command line utils are fun!"` |

### Decipher
Deciphers the provided text or file with the given key. Requires text or input file to be specified. Output file is optional. Note that the key must be a positive integer.

| Argument | Syntax                                     | Example                                    |
|----------|--------------------------------------------|--------------------------------------------|
| `-d`       | `CCUtil -d <key>` | `CCUtil -d 15 -t "RDBBPCS AXCT JIXAH PGT UJC!"` |

### Crack Key
Uses statistical analysis to guess the correct key and decipher the given text or file. Requires text or input file to be specified. Output file is optional. Note that the resulting plaintext is only a guess and may not be correct.

| Argument | Syntax                            | Example                                 |
|----------|-----------------------------------|-----------------------------------------|
| `-c`       | `CCUtil -c` | `CCUtil -c -t "RDBBPCS AXCT JIXAH PGT UJC!"` |

### Brute Force Examination
Prints the results of trying to decipher the provided text or file with every possible key. Requires text or input file to be specified. Output file is optional.

| Argument | Syntax                            | Example                                 |
|----------|-----------------------------------|-----------------------------------------|
| `-b`       | `CCUtil -b` | `CCUtil -b -t "RDBBPCS AXCT JIXAH PGT UJC!"` |

© Copyright 2016 Charles Duncan (CharlesETD@gmail.com)
