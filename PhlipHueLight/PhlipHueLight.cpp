// PhlipHueLight.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "LightStatus.h"
#include "ProcessStatus.h"
#include "PollingProcess.h"
#include "cmdparser.hpp"

int main(int argc, char** argv)
{
	cli::Parser parser(argc, argv);

	parser.set_required<std::string>("u", "url","URL for Philip Light Status API server");
	parser.set_optional<int>("p", "poll", 10, "polling timee period");
	parser.set_optional<int>("w", "poll", 10, "response wait time out");

	parser.run_and_exit_if_error();

	std::string url = parser.get<std::string>("u");
	int pollingTime = parser.get<int>("p");
	int responseWaitTime = parser.get<int>("w");

	auto runPolling = std::make_unique<PollingProcess>(url,pollingTime,responseWaitTime);

	runPolling->doPolling();

	return 0;
}