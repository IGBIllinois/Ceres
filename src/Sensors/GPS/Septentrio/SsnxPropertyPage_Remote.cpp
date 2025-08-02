

#include "SsnxPropertyPage_Remote.hpp"
#include "SsnxIDs.hpp"
#include "../GpsExperimentStates.hpp"


cSsnxPropertyPage_Remote::cSsnxPropertyPage_Remote(QWidget* parent)
	: cGpsPropertyPage_Remote(parent)
{}

cSsnxPropertyPage_Remote::cSsnxPropertyPage_Remote(const std::string& instance, QWidget* parent)
	: cGpsPropertyPage_Remote(instance, parent)
{}

cExperimentState* cSsnxPropertyPage_Remote::createState(const std::string& type, const nlohmann::json& entry, QObject* parent)
{
	if (type == ssnx_id)
	{
		auto hostname = getHostname();
		auto port = getPort();
		auto localIp = getLocalIpAddress();
		auto use_IpV6 = usingIpV6();

		std::string instance;
		if (entry.contains("instance"))
		{
			instance = entry["instance"];
		}

		if (!instance.empty())
		{
			if (instance != this->instance())
				return nullptr;
		}

		std::string cmd = entry["command"];

		if (cmd == "reference")
		{
			auto* pState = new cGpsReferenceAcquisition_Remote(hostname, port, localIp, use_IpV6, parent);

			if (parent)
				pState->moveToThread(parent->thread());

			return pState;
		}
	}

	return nullptr;
}

