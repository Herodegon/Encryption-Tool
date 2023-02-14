#include <iostream>

#include "EncryptDecrypt.h"

int main(int argc, char *argv[]) {
	EncryptDecrypt p(argc, argv);
	
	//Check if there are enough arguments to run program
	if(argc < 6) {
		std::cout << "Not enough arguments. Please check " 
				  << "that you have entered all of the "
				  << "required arguments.\n"
				  << std::endl
				  << "Example: -e -k myKey input.txt output.txt\n";
		return 1;
	}
	
	//Test user argument output
	std::cout << "Testing arguments...\n" << std::endl;
	p.TestArgs();
	
	//Check user arguments; update current program mode
	std::cout << "Checking argument data...\n" << std::endl;
	p.CheckArgs();
	if(p.IsFail()) {
		return 1;
	}
	
	//Generate cipher for encryption/decryption
	std::cout << "Generating cipher...\n" << std::endl;
	p.GenerateCipher();
	
	//Execute function based on mode
	std::cout << "Executing main program...\n" << std::endl;
	p.RunMode();
	if(p.IsFail()) {
		return 1;
	}
	
	return 0;
}
