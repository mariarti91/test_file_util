#include "TestApplication.h"

#include <iostream>
#include <algorithm>
#include <map>
#include <fstream>
#include <iomanip>
#include <cstring>

using namespace Test;

Application::Application( int argc, char **argv )
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
	openFile();
}

int Application::run() {
	switch( current_mode ) {
		case WorkMode::HELP: {
			printHelpMessage();
			break;
		}
		case WorkMode::WORDS: {
			if( !isArgExists("-v"))
				throw std::runtime_error{"target word has not supplied"};
			std::string target_word = getArgValue("-v");
			unsigned long word_count = countOfWords(target_word);
			std::cout << "word count of '" << target_word << "' in file '" << target_filename << "' is " << word_count << "\n";
			break;
		}
		case WorkMode::CHECKSUM: {
			auto checksum = calculateChecksum();
			std::cout << "checksum of file '" << target_filename << "': 0x";
			std::cout << std::hex;
			for(char& ch : checksum) std::cout << std::setw(2) << std::setfill('0')  << static_cast<int>(ch);
			std::cout << std::dec << "\n";
			break;
		}
	}
	return 0;
}

void Application::printHelpMessage() {
	std::cout << "Usage:\n"
	          << "  " << application_name << " -h                                - for output a help message\n"
	          << "  " << application_name << " -f <file_name> -m words -v <word> - for calculate count of words <word> in the file <file_name>\n"
	          << "  " << application_name << " -f <file_name> -m checksum        - for calculate a checksum of <file_name> file\n";
}

bool Application::isArgExists( const std::string &arg ) const {
	return std::find(args.cbegin(), args.cend(), arg) != args.cend();
}

std::string Application::getArgValue(const std::string &arg_name) const {
	auto iter = std::find(args.cbegin(), args.cend(), arg_name) + 1;
	return iter == args.cend() ? "" : *iter ;
}

void Application::detectWorkMode() {
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

void Application::openFile() {
	static auto stream_deleter = [](std::ifstream* s){
		if(s->is_open()) s->close();
		delete s;
	};

	ifs = std::unique_ptr<std::ifstream, std::function<void(std::ifstream*)>>{new std::ifstream(), stream_deleter};
	ifs->open(target_filename);
	if(!ifs->is_open())
		throw std::runtime_error{ "'" + target_filename + "' file opening error" };
}

unsigned long Application::countOfWords( const std::string &target_word ) {
	std::string word;
	unsigned long word_count = 0;
	while(*ifs >> word)
		if(word == target_word) ++word_count;

	return word_count;
}

std::vector<char> Application::calculateChecksum() {
	uint32_t hash = 0;
	size_t buffer_size = 4;
	std::vector<char> buffer(buffer_size, 0);
	while(!ifs->eof()){
		ifs->read(buffer.data(), buffer_size);
		if(ifs->gcount() < buffer_size)
			std::for_each(buffer.begin() + ifs->gcount(), buffer.end(), [](char &ch){ch = 0;});
		hash += ((buffer[0] << 24) + (buffer[1] << 16) + (buffer[2] << 8) + buffer[3] );
	}

	return std::vector<char>{
		static_cast<char>((hash >> 24)),
		static_cast<char>((hash >> 16)),
		static_cast<char>((hash >> 8)),
		static_cast<char>((hash))
	};
}
