#ifndef ENCRYPTDECRYPT_H
#define ENCRYPTDECRYPT_H

#include <iostream>
#include <vector>

enum mode {ENCRYPT, DECRYPT, NOT_SPECIFIED};

class EncryptDecrypt {
	public:
		EncryptDecrypt() {};
		EncryptDecrypt(int argc, char **argv) {
			
			this->argc = argc;
			
			*argv = new char[argc];
			this->argv = argv;
		}
		
		void CheckArgs();
		void TestArgs();
		
		void GenerateCipher();
		
		void RunMode();
		
		bool IsFail() {return fail;}
		
	private:
		int argc;
		char **argv;
		
		//User's keyword to use during execution
		std::string key;
		
		//Program's mode (either ENCRYPT or DECRYPT)
		mode m = NOT_SPECIFIED;
		void Encrypt(std::ifstream *input, std::ofstream *output);
		void Decrypt(std::ifstream *input, std::ofstream *output);
		
		//Strings used for ENCRYPT and DECRYPT
		std::string abc = "abcdefghijklmnopqrstuvwxyz";
		std::string cipher;
		
		//Flag to make sure program is working as intended
		bool fail = false;
};

#endif
