#pragma once
#include <string>

class LightStatus
{
public:
	LightStatus() = default;

	// Connect to simulator/server to retrieve Philip Light status
	std::string getStatus(std::string url);
};

