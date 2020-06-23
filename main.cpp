#include <iostream>
#include <algorithm>
#include <map>

void printHelpMessage() {
	static std::string binary_name = "test";
	std::cout << "Usage:\n"
	          << "  " << binary_name << " -h                                - for output a help message\n"
	          << "  " << binary_name << " -f <file_name> -m words -v <word> - for calculate count of words <word> in the file <file_name>\n"
	          << "  " << binary_name << " -f <file_name> -m checksum        - for calculate a checksum of <file_name> file\n";
}

bool isCmdFlagExists(int argc, char** argv, const std::string& option){
	return std::find(argv, argv + argc, option) != argv + argc;
}

enum class WorkMode{
	UNDEFINED = 0,
	WORDS,
	CHECKSUM
};

std::string getCmdOptsValue(int argc, char** argv, const std::string& option){
	auto cmd_arg_iter = std::find(argv, argv + argc, option) + 1;
	return std::move( cmd_arg_iter == argv + argc ? "" : std::string{ *cmd_arg_iter } );
}

WorkMode getWorkMode(int argc, char** argv){
	static std::map<std::string, WorkMode> modes_table = {
		{ "words", WorkMode::WORDS},
		{ "checksum", WorkMode::CHECKSUM}
	};

	std::string string_work_mode = getCmdOptsValue(argc, argv, "-m");
	auto modes_table_iter = modes_table.find(string_work_mode);

	return modes_table_iter == modes_table.end() ? WorkMode::UNDEFINED : modes_table_iter->second;
}

std::string getFileName(int argc, char** argv){
	return std::move( getCmdOptsValue(argc, argv, "-f") );
}

void printErrorMessageAndExit(const std::string& error_message, int exit_code = -1){
	std::cout << "Error: " << error_message << "\n\n";
	printHelpMessage();
	exit(exit_code);
}

int main(int argc, char** argv){
	if(isCmdFlagExists(argc, argv, "-h")){
		printHelpMessage();
		return 0;
	}

	if(!isCmdFlagExists(argc, argv, "-f"))
		printErrorMessageAndExit("file name has not supplied");

	if(!isCmdFlagExists(argc, argv, "-m"))
		printErrorMessageAndExit("work mode has not supplied");

	auto work_mode = getWorkMode(argc, argv);
	std::string filename = getFileName(argc, argv);

	if(filename.empty())
		printErrorMessageAndExit("filename has not supplied");

	if(work_mode == WorkMode::UNDEFINED)
		printErrorMessageAndExit("wrong work mode");

	std::cout << "filename: '" << filename << "' and mode " << static_cast<int>(work_mode) << "\n";
    return 0;
}
