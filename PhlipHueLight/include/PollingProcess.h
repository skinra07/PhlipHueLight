#pragma once

#include "LightStatus.h"
#include "ProcessStatus.h"

#include <memory>

class PollingProcess
{
public:
	PollingProcess(std::string url, int polling, int wait);
	void doPolling();
private:
	std::string retrieveLightStatus();
	
	bool running;
	std::string url;
	int polling_time;
	int wait_time;
	std::unique_ptr<LightStatus> light_status;
	std::unique_ptr<ProcessStatus> process_status;
};

