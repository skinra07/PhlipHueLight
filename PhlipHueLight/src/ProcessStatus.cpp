#include "ProcessStatus.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <iomanip>

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

// This is main process that will serialize the light status to object, check for any updates and display if any.
void ProcessStatus::doProcess(std::string response_light_status)
{
	seriliazeLightStatus(response_light_status);

	std::vector<std::pair<lsd::LightData, lsd::displayUpdate>> update_light_status = checkUpdateStatus();

	for (const auto& update_json_data : update_light_status)
	{
		this->displayUpdateStatus(update_json_data);
	}
}

// stored/cached the light status
void ProcessStatus::addCacheLightStatus(std::string id, std::string name, bool on, int brightness)
{
	lsd::LightData ld{id, name, on, brightness};

	cached_light_status_data.insert(std::make_pair(id, ld));
}

// update stored/cache light status with any latest.
void ProcessStatus::updateCacheLightStatus(std::string id, std::string name, bool on, int brightness, lsd::displayUpdate value_type)
{
	auto it = cached_light_status_data.find(id);

	if (it != cached_light_status_data.end())
	{
		if (value_type == lsd::NAME)
		{
			it->second.name = name;
		}
		else if (value_type == lsd::ON)
		{
			it->second.on = on;
		}
		else if (value_type == lsd::BRIGHTNESS)
		{
			it->second.brightness = brightness;
		}
	}
}

void ProcessStatus::seriliazeLightStatus(std::string status)
{
	this->light_data_list.clear();

	auto j = json::parse(status);

	for (auto& el1 : j.items())
	{
		if (el1.key() == lsd::LIGHTS_TAG)
		{
			for (auto& el2 : el1.value().items())
			{
				auto light_data = lsd::LightData();

				light_data.id = el2.key();

				for (auto& el3 : el2.value().items())
				{
					if (el3.key() == lsd::NAME_TAG)
					{
						light_data.name = el3.value();
					}
					if (el3.key() == lsd::STATE_TAG)
					{
						for (auto& el4 : el3.value().items())
						{
							if (el4.key() == lsd::ON_TAG)
							{
								light_data.on = el4.value();
							}
							else if (el4.key() == lsd::BRI_TAG)
							{
								light_data.brightness = el4.value();
							}
						}
					}
				}
				this->light_data_list.push_back(light_data);
			}
		}
	}
}

// This is function that caclulate any updates.
std::vector<std::pair<lsd::LightData, lsd::displayUpdate>> ProcessStatus::checkUpdateStatus()
{
	std::vector<std::pair<lsd::LightData, lsd::displayUpdate>> updates;

	// initial data is stored/cached
	if (this->cached_light_status_data.size() == 0)
	{
		for ( auto& light_data : this->light_data_list )
		{
			this->addCacheLightStatus(light_data.id, light_data.name, light_data.on, light_data.brightness);
			lsd::LightData data{ light_data.id, light_data.name, light_data.on, light_data.brightness };
			updates.push_back(std::make_pair(data, lsd::ALL));
		}
	}
	else
	{
		std::unordered_map<std::string, lsd::LightData>::iterator it;

		for (auto& light_data : this->light_data_list)
		{
			it = cached_light_status_data.find(light_data.id);

			// found the value
			if (it != cached_light_status_data.end())
			{
				lsd::LightData cache_data = it->second;

				// now we match for name, on and brightness
				if (cache_data.name != light_data.name)
				{
					lsd::LightData data{ light_data.id, light_data.name, light_data.on, light_data.brightness };
					updates.push_back(std::make_pair(data, lsd::NAME));
					this->updateCacheLightStatus(light_data.id, light_data.name, light_data.on, light_data.brightness,lsd::NAME);
				}
			    if (cache_data.on != light_data.on)
				{
					lsd::LightData data{ light_data.id, light_data.name, light_data.on, light_data.brightness };
					updates.push_back(std::make_pair(data, lsd::ON));
					this->updateCacheLightStatus(light_data.id, light_data.name, light_data.on, light_data.brightness, lsd::ON);
				}
				if (cache_data.brightness != light_data.brightness)
				{
					lsd::LightData data{ light_data.id, light_data.name, light_data.on, light_data.brightness };
					updates.push_back(std::make_pair(data, lsd::BRIGHTNESS));
					this->updateCacheLightStatus(light_data.id, light_data.name, light_data.on, light_data.brightness, lsd::BRIGHTNESS);
				}
			}
			else  // this is when there is new light status id
			{
				this->addCacheLightStatus(light_data.id, light_data.name, light_data.on, light_data.brightness);
				lsd::LightData data{ light_data.id, light_data.name, light_data.on, light_data.brightness };
				updates.push_back(std::make_pair(data, lsd::ALL));
			}
		}
	}

	return updates;
}

void ProcessStatus::displayUpdateStatus(const std::pair<lsd::LightData, lsd::displayUpdate>& update_data)
{
	ordered_json json_data;

	if (update_data.second == lsd::ALL)
	{
		json_data[lsd::ID_TAG] = update_data.first.id;
		json_data[lsd::NAME_TAG] = update_data.first.name;
		json_data[lsd::ON_TAG] = update_data.first.on;
		json_data[lsd::BRIGHTNESS_TAG] = update_data.first.brightness;
	}
	else if (update_data.second == lsd::NAME)
	{
		json_data[lsd::ID_TAG] = update_data.first.id;
		json_data[lsd::NAME_TAG] = update_data.first.name;
	}
	else if (update_data.second == lsd::ON)
	{
		json_data[lsd::ID_TAG] = update_data.first.id;
		json_data[lsd::ON_TAG] = update_data.first.on;
	}
	else if (update_data.second == lsd::BRIGHTNESS)
	{
		json_data[lsd::ID_TAG] = update_data.first.id;
		json_data[lsd::BRIGHTNESS_TAG] = update_data.first.brightness;
	}

	std::cout << std::setw(4) << json_data << std::endl;

	std::cout << "----------------------------------------------------" << std::endl;
}