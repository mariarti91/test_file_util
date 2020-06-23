#include "TestApplication.h"

#include <iostream>
#include <algorithm>
#include <map>

TestApplication::TestApplication( int argc, char **argv )
:application_name{argv[0] }
,args{argv + 1, argv + argc}
{
	if(args.empty() || isArgExists("-h")) return;

	if(!isArgExists("-f"))
		throw std::runtime_error{ "file name has not supplied" };

	if(!isArgExists("-m"))
		throw std::runtime_error{ "work mode has not supplied" };

	target_filename = getArgValue("-f");
	if(target_filename.empty())
		throw std::runtime_error{ "filename has not supplied" };

	detectWorkMode();
}

int TestApplication::run() {
	switch( current_mode ) {
		case WorkMode::HELP:
			printHelpMessage();
			break;
		case WorkMode::WORDS:
			std::cout << "calculate WORDS in file '" << target_filename << "'\n";
			break;
		case WorkMode::CHECKSUM:
			std::cout << "calculate CHECKSUM in file '" << target_filename << "'\n";
			break;
	}
	return 0;
}

void TestApplication::printHelpMessage() {
	std::cout << "Usage:\n"
	          << "  " << application_name << " -h                                - for output a help message\n"
	          << "  " << application_name << " -f <file_name> -m words -v <word> - for calculate count of words <word> in the file <file_name>\n"
	          << "  " << application_name << " -f <file_name> -m checksum        - for calculate a checksum of <file_name> file\n";
}

bool TestApplication::isArgExists( const std::string &arg ) const {
	return std::find(args.cbegin(), args.cend(), arg) != args.cend();
}

std::string TestApplication::getArgValue(const std::string &arg_name) const {
	auto iter = std::find(args.cbegin(), args.cend(), arg_name) + 1;
	return iter == args.cend() ? "" : *iter ;
}

void TestApplication::detectWorkMode() {
	static std::map<std::string, WorkMode> modes_table = {
		{ "words", WorkMode::WORDS},
		{ "checksum", WorkMode::CHECKSUM}
	};
	
	auto mode_string = getArgValue("-m");
	auto iter = modes_table.find(mode_string);
	if(iter == modes_table.end())
		throw std::runtime_error{ "illegal mode value '" + mode_string + "'" };
	current_mode = iter->second;
}
