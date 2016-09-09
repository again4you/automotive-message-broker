

#ifndef ETHERNETPLUGIN_H
#define ETHERNETPLUGIN_H

#include <abstractsource.h>
#include <string>

using namespace std;

class EthernetPlugin: public AbstractSource
{
public:
        EthernetPlugin(): AbstractSource(nullptr, map<string, string>()) {}
        EthernetPlugin(AbstractRoutingEngine* re, map<string, string> config);

        // from AbstractSink
        const string uuid();
        void supportedChanged(const PropertyList &) {}

        // from AbstractSource
        void getPropertyAsync(AsyncPropertyReply *reply);
        void getRangePropertyAsync(AsyncRangePropertyReply *reply);
        AsyncPropertyReply * setProperty(AsyncSetPropertyRequest request);
        void subscribeToPropertyChanges(VehicleProperty::Property property);
        void unsubscribeToPropertyChanges(VehicleProperty::Property property);
        int supportedOperations();
        PropertyInfo getPropertyInfo(const VehicleProperty::Property & property);
        PropertyList supported();

private:
        std::map<VehicleProperty::Property, PropertyInfo> propertyInfoMap;
        PropertyList mSupported;
        PropertyList mRequests;

        void addPropertySupport(VehicleProperty::Property property, Zone::Type zone);
};
#endif          // ETHERNETPLUGIN_H