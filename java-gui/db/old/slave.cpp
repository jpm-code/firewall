#include <iostream>
#include <string>

int main() {

	std::string msgIn = std::string("");

	std::cout << "MSG_COUT" << std::endl ;
	while( std::cin.eof() ) {}
	// Use useful functions: std::cin.fail() std::cin.read(buffer, sizeof(buffer) - 1) 
	// std::cin.peek();
	//std::cin  >> msgIn ;
	std::getline(std::cin, msgIn);
	std::cout << "Standard input (Java -> exe):\n" << msgIn << std::endl ;
	std::cerr << "MSG_CERR" << std::endl ;

	return 0;
}

