#include <iostream>
#include <string>

int main() {

	std::string msgIn = std::string("");

	
	while( std::cin.eof() ) {}
	// Use useful functions: std::cin.fail() std::cin.read(buffer, sizeof(buffer) - 1) 
	// std::cin.peek();
	//std::cin  >> msgIn ;
	std::getline(std::cin, msgIn);
	std::cout << msgIn << std::endl ;

       // std::cout << msgIn << std::endl ;

	//std::cerr << "MSG_CERR" << std::endl ;

	return 0;
}

