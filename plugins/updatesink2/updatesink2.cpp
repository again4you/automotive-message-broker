
#include "ambplugin.h"
#include "updatesink2.h"
#include "debugout.h"
#include "abstractroutingengine.h"

#include <glib.h>

const VehicleProperty::Property TPMS_FL = "TPMS_FL";

extern "C" void create(AbstractRoutingEngine* routingEngine, map<string, string> config)
{
	auto plugin = new AmbPlugin<UpdateSink2>(routingEngine, config);
}

UpdateSink2::UpdateSink2(AbstractRoutingEngine* re,
		const std::map<std::string, std::string>& config,
		AbstractSource& parent)
	:AmbPluginImpl(re, config, parent)
{
	DebugOut() << "SJ: Enter " << __func__ << endl;
	re->subscribeToProperty(VehicleProperty::VehicleSpeed, &source);
}

const string UpdateSink2::uuid() const
{
	return "5b0e8a04-d6d7-43af-b827-1663627a25d9";
}

void UpdateSink2::propertyChanged(AbstractPropertyType *value)
{
	VehicleProperty::Property property = value->name;
	DebugOut() << "SJ: " << property << " value: " << value->toString() << endl;
}



