#ifndef TEST_FILE_UTIL_TESTAPPLICATION_H
#define TEST_FILE_UTIL_TESTAPPLICATION_H

#include <string>
#include <vector>

namespace Test {

class Application {
public:
	explicit Application( int argc, char **argv );

	int run();

private:
	void printHelpMessage();
	bool isArgExists( const std::string &arg ) const;
	std::string getArgValue( const std::string &arg_name ) const;
	void detectWorkMode();

	unsigned long countOfWords( const std::string &target_word );

	enum class WorkMode {
		HELP,
		WORDS,
		CHECKSUM
	};
	WorkMode current_mode = WorkMode::HELP;
	std::string target_filename = "";

	std::string application_name;
	std::vector<std::string> args;
};

}

#endif //TEST_FILE_UTIL_TESTAPPLICATION_H
