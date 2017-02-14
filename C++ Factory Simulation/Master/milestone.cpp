#include <iostream>
#include <string>
#include "CSV_READER.h"


int main(int argc, char** argv)
{
	try {
		milestone::CSV csv(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);
		//csv.display();
		//std::cout << "\n";
		//csv.displaytwo();
		//std::cout << "\n";
		//csv.displaythree();
	}
	catch (std::string& e) {
		std::cerr << "Failed opening file Begining\n" << e << "\n";
	}

	return 0;
}
