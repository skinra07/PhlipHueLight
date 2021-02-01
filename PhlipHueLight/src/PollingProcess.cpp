#include "PollingProcess.h"
#include <iostream>
#include <string>
#include <future>
#include <chrono>
#include <exception>

PollingProcess::PollingProcess(std::string url, int polling, int wait) : 
	running(true), url(url), polling_time(polling), wait_time(wait)
{
	this->light_status = std::make_unique<LightStatus>();
	this->process_status = std::make_unique<ProcessStatus>();
}

std::string PollingProcess::retrieveLightStatus()
{
	try
	{
		return light_status->getStatus(this->url);
	}
	catch (std::exception& ex)
	{
		std::cout << "Exception: " << ex.what() << std::endl;
	}

	return std::string{};
}

void PollingProcess::doPolling()
{
	int wait_polling = 0;

	while (running)
	{
		std::cout << "Sleeping for: " << wait_polling << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(wait_polling));
		std::cout << "Retriving Light Status" << std::endl;

		std::future<std::string> futureData = std::async(std::launch::async, &PollingProcess::retrieveLightStatus, this );

		std::chrono::system_clock::time_point timeOut = std::chrono::system_clock::now() + std::chrono::seconds(this->wait_time);

		// wait for response from simulator until time out.
		std::future_status status = futureData.wait_until(timeOut); 

		if (status == std::future_status::ready)
		{
			auto lightStatus = futureData.get();

			if (!lightStatus.empty())
			{
				process_status->doProcess(lightStatus);
			}
		}
		else
		{
			std::cout << "Timeout reached to Retrieve Light Status...." << std::endl;
		}

		wait_polling = this->polling_time;
	}
}