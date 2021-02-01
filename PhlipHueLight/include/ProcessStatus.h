#pragma once
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include "LightData.h"

class ProcessStatus
{
public:
	ProcessStatus() = default;

	void doProcess(std::string response_light_status);
private:
	void addCacheLightStatus(std::string id, std::string name, bool on, int brightness);
	void updateCacheLightStatus(std::string id, std::string name, bool on, int brightness, lsd::displayUpdate value_type);
	void seriliazeLightStatus(std::string response_data);
	std::vector<std::pair<lsd::LightData, lsd::displayUpdate>> checkUpdateStatus();
	void displayUpdateStatus(const std::pair<lsd::LightData, lsd::displayUpdate>& update_data);

	std::vector<lsd::LightData> light_data_list;
	std::unordered_map<std::string, lsd::LightData> cached_light_status_data;
};

