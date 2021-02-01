#include "LightStatus.h"
#include "httplib.h"
#include "LightData.h"

std::string LightStatus::getStatus(std::string url)
{
	httplib::Client cli(url.c_str());

	auto res = cli.Get(lsd::LIGHT_STATUS_API.c_str());

	if (res)
	{
		std::cout << "HttpResult for Philip Light Status = " << res->status << std::endl;

		if (res->status == 200)
		{
			return res->body;
		}
	}
	else
	{
		std::cout << "Http Error = " << res.error() << std::endl;
	}

	return std::string{};
}