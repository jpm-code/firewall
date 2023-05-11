#include "stdafx.h"
#include "logger.h"

#include "nfapi.h"

using namespace nfapi;

std::string new_filename = get_logger_filename();
std::ofstream outfile(new_filename);

std::string domainLogFilename = get_domain_logger_filename();
std::ofstream domainLogFile(domainLogFilename);

/**
* Get the logger filename
**/
std::string get_logger_filename() {

	auto now = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&time), "%Y-%m-%d");
	std::string base_filename = "log-" + ss.str() + ".txt";

	std::string new_filename = base_filename;
	int index = 1;

	while (std::ifstream(new_filename)) {
		std::stringstream ss;
		ss << base_filename << "_" << index;
		new_filename = ss.str();
		index++;
	}

	return new_filename;
}

void logMessage(std::string logMsg)
{
	if (outfile.is_open()) {
		auto now = std::chrono::system_clock::now();
		std::time_t time = std::chrono::system_clock::to_time_t(now);

		std::stringstream ss;
		ss << " [" << std::put_time(std::localtime(&time), "%H:%M:%S") << "] ";
		outfile << ss.str() << logMsg << std::endl;

	}
}

std::string get_domain_logger_filename()
{

	auto now = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&time), "%Y-%m-%d");
	std::string base_filename = "domains-" + ss.str() + ".txt";

	std::string new_filename = base_filename;
	int index = 1;

	while (std::ifstream(new_filename)) {
		std::stringstream ss;
		ss << base_filename << "_" << index;
		new_filename = ss.str();
		index++;
	}

	return new_filename;
}

void logDomain(std::string domainMsg)
{
	if (domainLogFile.is_open()) {
		auto now = std::chrono::system_clock::now();
		std::time_t time = std::chrono::system_clock::to_time_t(now);

		std::stringstream ss;
		ss << " [" << std::put_time(std::localtime(&time), "%H:%M:%S") << "] ";
		domainLogFile << ss.str() << domainMsg << std::endl;
	}
}

void closeLogFile() {
	// Close log file
	outfile.close();
}