#include <iostream>

void printHelpMessage() {
	static std::string binary_name = "test";
	std::cout << "Usage:\n"
	          << "  " << binary_name << " -h                                - for output a help message\n"
	          << "  " << binary_name << " -f <file_name> -m words -v <word> - for calculate count of words <word> in the file <file_name>\n"
	          << "  " << binary_name << " -f <file_name> -m checksum        - for calculate a checksum of <file_name> file\n";
}

int main(int argc, char** argv){
	printHelpMessage();
    return 0;
}
