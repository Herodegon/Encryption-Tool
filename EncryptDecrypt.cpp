#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <cctype>

#include "EncryptDecrypt.h"

//Check validity of arguments to ensure program is safe to run
void EncryptDecrypt::CheckArgs() {
    
    //Check for arguments
    std::string s;
    s.assign(argv[0]);
    
    for(int i = 1; i < (argc-2); i++) {
        if(strlen(argv[i]) == 2) {
            switch(argv[i][1]) {
                case 'e':
                    m = ENCRYPT;
                    break;
                case 'd':
                    m = DECRYPT;
                    break;
                case 'k':
                    if( (i+1) < argc) {
                        if(strlen(argv[i+1]) != 0) {
                            key.assign(argv[i+1]);
                        }
                    }
                    else {
                        std::cout << "No key set. Please check that there "
                                  << "is a key phrase after \'-k\'. "
                                  << "Shutting down...\n";
                        fail = true;
                        return;
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

//Generate cipher for ENCRYPT and DECRYPT
void EncryptDecrypt::GenerateCipher() {
    std::string cipherWord = key;
    std::string cipherABC = abc;
    
    for(size_t i = 0; i < cipherWord.size(); i++) {
        cipherWord.at(i) = tolower(cipherWord.at(i));
    }
    
    /*!TEST cipherWord and cipherABC String Output BEFORE; REMOVE
    std::cout << "cipherWord: " << cipherWord << std::endl
              << "cipherABC: " << cipherABC << std::endl
              << std::endl;
    */
    
    char currentChar;
    unsigned int foundChar;
    std::string usedChars;
    
    for(unsigned size_t = 0; size_t < key.size(); size_t++) {
        currentChar = tolower(key.at(size_t));
        
        /*!TEST currentChar; REMOVE
        std::cout << "Current Char: " << currentChar << std::endl;
        */
        
        //Check if character has been used before
        if(usedChars.find(currentChar) != std::string::npos) {
            int firstOf = cipherWord.find_first_of(currentChar);
            int lastOf = cipherWord.find_last_of(currentChar);
            
            while(firstOf != lastOf) {
                cipherWord.erase(cipherWord.begin() + lastOf);
                lastOf = cipherWord.find_last_of(currentChar);                   
            }
        }
        
        //Check if current letter is still in alphabet
        if(cipherABC.find(currentChar) != std::string::npos) {
            foundChar = cipherABC.find(currentChar);
            cipherABC.erase(cipherABC.begin() + foundChar);
        }
        
        //If character hasn't been seen before, put in usedChars
        if(usedChars.find(currentChar) == std::string::npos) {
            usedChars.push_back(currentChar);
        }
    }
    
    /*!TEST cipherWord and cipherABC String Output AFTER; REMOVE
    std::cout << "cipherWord: " << cipherWord << std::endl
              << "cipherABC: " << cipherABC << std::endl
              << std::endl;
    */
    
    //Store Completed cipherWord in Cipher
    for(unsigned size_t = 0; size_t < cipherWord.size(); size_t++) {
        cipher.push_back(cipherWord.at(size_t));
    }
    
    //Store Remaining Reversed Alphabet in Cipher
    for(int i = (cipherABC.size() - 1); i >= 0; i--) {
        cipher.push_back(cipherABC.at(i));
    }
    
    /*!TEST cipher.size(); REMOVE
    std::cout << "Cipher Size: " << cipher.size() << std::endl;
    */
    
    //Exclaim function success
    std::cout << "Cipher generation successful!\n"
              << std::endl;
}

void EncryptDecrypt::RunMode() {
    
    //Open files using user-specified file paths
    std::ifstream userFileInput;
    std::ofstream userFileOutput;
    std::string userPathInput;
    std::string userPathOutput;
    
    //!TEST userPath Assignment BEFORE; REMOVE
    std::cout << "Assigning variables with file paths...\n"
              << std::endl;
    
    int inputPath = argc-2;
    int outputPath = argc-1;
    
    userPathInput.assign(argv[inputPath]);
    userPathOutput.assign(argv[outputPath]);
    
    //!TEST userPath Assignment AFTER; REMOVE
    std::cout << "Assignment Successful!\n"
              << "Input Path: " << userPathInput << std::endl
              << "Output Path: " << userPathOutput << std::endl
              << std::endl;
    
    userFileInput.open(userPathInput);
    userFileOutput.open(userPathOutput, std::ofstream::trunc);
    
    //Check if files can be opened
    if(!((userFileInput.is_open()) || (userFileOutput.is_open()))) {
        std::cout << "Cannot open files. Please check file paths. "
                  << "Shutting down...\n";
        fail = true;
        return;
    }
    
    //Execute mode
    std::cout << "Executing main function...\n"
              << std::endl;
    
    std::string progMode;
    switch(m) {
        //Encryption
        case ENCRYPT:
            progMode = "Encryption";
            Encrypt(&userFileInput, &userFileOutput);
            break;
        
        //Decryption
        case DECRYPT:
            progMode = "Decryption";
            Decrypt(&userFileInput, &userFileOutput);
            break;
        
        //Unspecified Mode
        default:
            std::cout << "No valid mode set. Shutting down...\n";
            
            userFileInput.close();
            userFileOutput.close();
            
            fail = true; //Set error flag to true
            return;
    }
    
    //Check fail state
    if(IsFail()) { //Encountered error during runtime
        std::cout << progMode << " Unsuccessful.\n";
    }
    else { //If no flag, then proclaims successful execution
        std::cout << progMode << " Successful!\n";
    }
    std::cout << std::endl;
    
    //Close files to end function
    std::cout << "Closing files...\n";
    userFileInput.close();
    userFileOutput.close();
}

//Encryption Mode
void EncryptDecrypt::Encrypt(std::ifstream *input, std::ofstream *output) {
    
    //!IDEA Create Custom key.txt File Based On Output Name
    //Create key file
    std::ofstream keyStore("key.txt", std::ofstream::trunc);
    
    if(!(keyStore.is_open())) {
        std::cout << "Error encountered when making key file. "
                  << "Shutting down...\n";
        fail = true; //Set error flag to true
        return;
    }
    
    //Write to key file
    std::ostringstream keyStream;
    
    keyStore << key << " <-- This is your key. Keep it in a safe place.";
    
    //Execute main function
    std::string str;
    if((input->is_open()) && (output->is_open())) {
        while(getline(*input, str)) {
            size_t i = 0;
            std::ostringstream fileStr;
            
            while(i < str.size()) {
                char fileC = str.at(i);
                
                if(isalpha(fileC)) { //If char is letter
                    char trueChar;
                    int loc;
                    
                    //Check if uppercase
                    if(isupper(fileC)) {
                        fileStr << "^^";
                    }
                    
                    //Encrypt character
                    loc = abc.find(tolower(fileC));
                    trueChar = cipher.at(loc);
                    
                    fileStr << trueChar;
                }
                else { //If char is symbol or newspace
                    fileStr << fileC;
                }
                
                //Move to next iteration
                i++;
            }
            *output << fileStr.str() << std::endl;
        }
    }
}

//Decryption Mode
void EncryptDecrypt::Decrypt (std::ifstream *input, std::ofstream *output) {
    std::ifstream keyCheck;
    keyCheck.open("key.txt");
    
    //Compare user key to key file
    std::string fileKey;
    
    getline(keyCheck, fileKey);
    
    if(fileKey.find(key) == std::string::npos) {
        std::cout << "Key does not match file key. Shutting down...\n";
        fail = true;
        return;
    }
    
    //Execute main function
    std::string str;
    if((input->is_open()) && (output->is_open())) {
        while(getline(*input, str)) {
            size_t i = 0;
            std::ostringstream fileStr;
            
            while(i < str.size()) {
                char fileC = str.at(i);
                bool isUpperCase = ((fileC == '^') && (str.at(i+1) == '^'));
                
                //Check if uppercase
                if(isUpperCase) {
                    if(str.at(i+2) != std::string::npos) {
                        fileC = tolower(str.at(i+2));
                        
                        i += 2;
                    }
                }
                
                if(isalpha(fileC)) { //If char is letter
                    char trueChar;
                    int loc;
                    
                    //Decypher character
                    loc = cipher.find(fileC);
                    trueChar = abc.at(loc);
                    
                    //Change trueChar if uppercase
                    if(isUpperCase) {
                        trueChar = toupper(trueChar);
                    }
                    
                    //Insert character into stringstream
                    fileStr << trueChar;
                }
                else { //If char is symbol or newspace
                    fileStr << fileC;
                }
                
                //Move to next iteration
                i++;
            }
            *output << fileStr.str() << std::endl;
        }
    } else { //Error with accessing file
        std::cout << "Error with opening file. Shutting down...\n";
        fail = true;
        return;
    }
}

//Optional: Tests various arguments in program
void EncryptDecrypt::TestArgs() {
    int numTests = 0;
    int testsPassed = 0;
    
    std::cout << "Test Start!\n"
              << std::endl;
    
    //Output full argument list
    for(int i = 0; i < argc; i++) {
        std::cout << argv[i] << " ";
    }
    std::cout << std::endl
              << "Test Successful!\n"
              << std::endl;
    
    testsPassed++;
    numTests++;
              
    //Flag inclusion of encrypt/decrypt argument
    bool isEncryptDecrypt = false;
    
    for(int i = 1; i < argc; i++) {
        if(strlen(argv[i]) == 2) {
            std::cout << "Current CString: " << argv[i];
            if((argv[i][1] == 'e') || (argv[i][1] == 'd')) {
                std::cout << " | Program State: ";
                switch(argv[i][1]) {
                    case 'e':
                        std::cout << "Encrypt\n";
                        break;
                    case 'd':
                        std::cout << "Decrypt\n";
                        break;
                }
                isEncryptDecrypt = true;
                
                break;
            }
        }
        std::cout << std::endl; //End of current iteration
    }
    
    if(isEncryptDecrypt == false) {
        std::cout << "Test Failed; Could not find encrypt/decrypt argument.\n";
    } else {
        std::cout << "Test Successful!\n";
        testsPassed++;
    }
    std::cout << std::endl;
    
    numTests++;
    
    //Flag and cite user keyword
    bool isKey = false;
    
    for(int i = 1; i < argc; i++) {
        if(strlen(argv[i]) == 2) {
            std::cout << "Current CString: " << argv[i];
            if(argv[i][1] == 'k') {
                std::cout << " | Program Key: " << argv[i+1] << std::endl;
                isKey = true;
                
                break;
            }
        }
        std::cout << std::endl; //End of current iteration
    }
    
    if(isKey == false) {
        std::cout << "Test Failed; Could not find key argument.\n";
    } else {
        std::cout << "Test Successful!\n";
        testsPassed++;
    }
    std::cout << std::endl;
    
    numTests++;
    
    //Flag and cite user file paths
    int numFilePaths = 0;
    bool allFilePaths = false;
    
    for(int i = (argc-2); i < argc; i++) {
        if(strlen(argv[i]) != 2) {
            std::cout << "Filepath: " << argv[i] << std::endl;
            numFilePaths++;
        }
        if(numFilePaths == 2) {
            allFilePaths = true;
            
            break;
        }
    }
    
    if(allFilePaths == false) {
        std::cout << "Test Failed; Could not find all file paths.\n";
    } else {
        std::cout << "Test Successful!\n";
        testsPassed++;
    }
    std::cout << std::endl;
    
    numTests++;
    
    //Output ratio of numTests to testsPassed
    std::cout << testsPassed << "/" << numTests << " Passed\n"
              << std::endl;
}
