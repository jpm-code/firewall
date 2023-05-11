#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>

std::string get_logger_filename();
void logMessage(std::string logMsg);
std::string get_domain_logger_filename();
void logDomain(std::string domainMsg);
void closeLogFile();
