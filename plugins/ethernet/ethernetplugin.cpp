

#include "ethernetplugin.h"
#include "logger.h"
#include "listplusplus.h"

#include <iostream>
#include <glib.h>

using namespace std;

const char* id = "6dd4268a-c605-4a06-9034-59c1e8344c8e";

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// from AbstractSink
const string EthernetPlugin::uuid()
{
        return id;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// from AbstractSource
void EthernetPlugin::getPropertyAsync(AsyncPropertyReply *reply)
{
        LOG_INFO( "Enter " << __func__ << "Requested property: " << reply->property << endl);

        if (reply->property == VehicleProperty::VehicleSpeed) {
                VehicleProperty::VehicleSpeedType temp(123);
                reply->value = &temp;
                reply->success = true;
                reply->completed(reply);
        } else {
                reply->success = false;
                reply->error = AsyncPropertyReply::InvalidOperation;
                reply->completed(reply);
        }
}

void EthernetPlugin::getRangePropertyAsync(AsyncRangePropertyReply *reply)
{
        LOG_INFO( "Enter " << __func__);
}

AsyncPropertyReply * EthernetPlugin::setProperty(AsyncSetPropertyRequest request)
{
        LOG_INFO( "Enter " << __func__);
        AsyncPropertyReply *reply = new AsyncPropertyReply(request);
        reply->success = false;
        reply->error = AsyncPropertyReply::InvalidOperation;
        reply->completed(reply);

        return reply;
}

void EthernetPlugin::subscribeToPropertyChanges(VehicleProperty::Property property)
{
        LOG_INFO( "Enter " << __func__);

        mRequests.push_back(property);
}

void EthernetPlugin::unsubscribeToPropertyChanges(VehicleProperty::Property property)
{
        LOG_INFO( "Enter " << __func__);

        if(contains(mRequests,property))
                removeOne(&mRequests, property);
}

int EthernetPlugin::supportedOperations()
{
        return Get | Set;
}

PropertyInfo EthernetPlugin::getPropertyInfo(const VehicleProperty::Property & property)
{
        if(propertyInfoMap.find(property) != propertyInfoMap.end())
                return propertyInfoMap[property];

        return PropertyInfo::invalid();
}

PropertyList EthernetPlugin::supported()
{
        return mSupported;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// public
EthernetPlugin::EthernetPlugin(AbstractRoutingEngine* re, map<string, string> config)
: AbstractSource(re, config)
{
        LOG_INFO( "Enter " << __func__);

        addPropertySupport(VehicleProperty::VehicleSpeed, Zone::None);
}

extern "C" void create(AbstractRoutingEngine* routingengine, map<string, string> config)
{
        DEBUG_CONF("ethernetplugin",
                        CUtil::Logger::file_off|CUtil::Logger::screen_on,
                        CUtil::Logger::EInfo, CUtil::Logger::EInfo
        );

        new EthernetPlugin(routingengine, config);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// private
void EthernetPlugin::addPropertySupport(VehicleProperty::Property property, Zone::Type zone)
{
        mSupported.push_back(property);

        Zone::ZoneList zones;
        zones.push_back(zone);

        PropertyInfo info(0, zones);
        propertyInfoMap[property] = info;
}