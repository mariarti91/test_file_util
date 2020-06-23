#ifndef TEST_FILE_UTIL_TESTAPPLICATION_H
#define TEST_FILE_UTIL_TESTAPPLICATION_H

#include <string>
#include <vector>

class TestApplication {
public:
	explicit TestApplication(int argc, char** argv);
	int run();

private:
	void printHelpMessage();

	bool isArgExists(const std::string& arg) const;
	std::string getArgValue(const std::string &arg_name) const;
	void detectWorkMode();

	enum class WorkMode{
		HELP,
		WORDS,
		CHECKSUM
	};
	WorkMode current_mode = WorkMode::HELP;
	std::string target_filename = "";

	std::string application_name;
	std::vector<std::string> args;
};


#endif //TEST_FILE_UTIL_TESTAPPLICATION_H