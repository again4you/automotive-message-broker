
#include "updatesink.h"
#include "abstractroutingengine.h"
#include "debugout.h"

#include <glib.h>

const VehicleProperty::Property TPMS_FL = "TPMS_FL";
const VehicleProperty::Property TPMS_FR = "TPMS_FR";

extern "C" void create(AbstractRoutingEngine* routingEngine, map<string, string> config)
{
	new UpdateSink(routingEngine, config);
}

const std::string UpdateSink::uuid()
{
	return "5b0e8a04-d6d7-43af-b827-1663627a25d9";
}

UpdateSink::UpdateSink(AbstractRoutingEngine* re, map<string, string> config)
	:AbstractSink(re, config)
{
	DebugOut() << "SJ: Enter " << __func__ << endl;

	// routingEngine->subscribeToProperty(VehicleProperty::VehicleSpeed, this);
	routingEngine->subscribeToProperty(TPMS_FL, this);
	
}
UpdateSink::~UpdateSink()
{

}

void UpdateSink::propertyChanged(AbstractPropertyType* value)
{
	VehicleProperty::Property property = value->name;
	DebugOut() << "SJ: " << property << " value: " << value->toString() << endl;

	GVariant *var = value->toVariant();
	gchar *s = g_variant_print(var, TRUE);
	DebugOut() << "GVariant: " << s << endl;

	// create new data
	AbstractPropertyType* nvalue = new BasicPropertyType<char>(TPMS_FR, 1);
	nvalue->priority = AbstractPropertyType::High;

	// Not working
	// routingEngine->updateProperty(nvalue, uuid());
	routingEngine->updateProperty(nvalue, "");
	// final
	delete nvalue;

	g_free(s);
	g_variant_unref(var);
}

void UpdateSink::supportedChanged(const PropertyList & supportedProperties)
{
	DebugOut() << "SJ: Enter Support changed!" << endl;
}
