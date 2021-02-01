#pragma once

#include<string>

namespace lsd 
{
	const std::string LIGHTS_TAG{ "lights" };
	const std::string ID_TAG{ "id" };
	const std::string STATE_TAG{ "state" };
	const std::string NAME_TAG{ "name" };
	const std::string ON_TAG{ "on" };
	const std::string BRI_TAG{ "bri" };
	const std::string BRIGHTNESS_TAG{ "brightness" };
	const std::string LIGHT_STATUS_API{ "/api/newdeveloper" };

	enum displayUpdate
	{
		ALL,
		NAME,
		ON,
		BRIGHTNESS
	};

	struct LightData
	{
		std::string id;
		std::string name;
		bool on;
		int brightness;
	};
}