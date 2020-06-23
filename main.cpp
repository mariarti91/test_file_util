#include <iostream>

#include "TestApplication.h"

int main(int argc, char** argv){
	try {
		Test::Application app(argc, argv);
		return app.run();
	} catch( const std::runtime_error& e) {
		std::cerr << "Error: " << e.what() << "\n\nuse '" << argv[0] << " -h' for help\n";
	}
    return -1;
}
