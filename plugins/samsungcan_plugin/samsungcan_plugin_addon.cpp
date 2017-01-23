
#include <logger.h>

#include "samsungcan_plugin.h"
#include "samsungcan_cansignals.h"

const int MAX_VOLUMN = 15;
const int MIN_VOLUMN = 0;

VehicleProperty::Property notiItems[] = {
    CidWatchHour,
    AirDistributionCID
};

VehicleProperty::Property subscribedItems[] = {
    AirDistributionLeftKnob,
    LeftTemperatureLeftKnob,
    LeftAirflowLeftKnob,
    AirDistributionRightKnob,
    RightTemperatureRightKnob,
    RightAirflowLeftKnob,
    LeftAirflowCID,
    AirDistributionCID,
    LeftTemperatureCID,
    RightTemperatureCID,
};

void SamsungCANPlugin::updateTime(gpointer data)
{
    SamsungCANPlugin *scan = (SamsungCANPlugin *)data;
    time_t ctime;
    struct tm ts;
    VehicleProperty::Property timeItems[] = {
        CidWatchHour,
        CidWatchMin,
        CidWatchSec,
        CidWatchYY,
        CidWatchMM,
        CidWatchDD
    };
    int cnt = sizeof(timeItems)/sizeof(timeItems[0]);

    ctime = time(NULL);
    localtime_r(&ctime, &ts);

    for (int i=0; i<cnt; ++i) {
        GVariant *var;

        AbstractPropertyType *nvalue = scan->findPropertyType(timeItems[i], Zone::None);
        if (!nvalue) {
            LOG_ERROR("Fail to find " << timeItems[i] << endl);
            continue;
        }

        if (!nvalue->name.compare(CidWatchYY)) {
            var = g_variant_new_uint16(ts.tm_year + 1900);
        } else if (!nvalue->name.compare(CidWatchMM)) {
            var = g_variant_new_byte(ts.tm_mon + 1);
        } else if (!nvalue->name.compare(CidWatchDD)) {
            var = g_variant_new_byte(ts.tm_mday);
        } else if (!nvalue->name.compare(CidWatchHour)) {
            var = g_variant_new_byte(ts.tm_hour);
        } else if (!nvalue->name.compare(CidWatchMin)) {
            var = g_variant_new_byte(ts.tm_min);
        } else { // CidWatchSec
            var = g_variant_new_byte(ts.tm_sec);
        }
        nvalue->fromVariant(var);
        scan->routingEngine->updateProperty(nvalue, scan->uuid());
        g_variant_unref(var);
    }
}

gboolean SamsungCANPlugin::gwbox_callback(gpointer data)
{
    SamsungCANPlugin *scan = (SamsungCANPlugin *)data;

    // Update time
    updateTime(scan);

    // Send CAN Frame
    unsigned int cnt = sizeof(notiItems)/sizeof(notiItems[0]);
    for (int i=0; i<cnt; ++i) {
        AbstractPropertyType *nvalue = scan->findPropertyType(notiItems[i], Zone::None);
        if (!nvalue) {
            LOG_ERROR("Fail to find " << notiItems[i] << endl);
            continue;
        }
        if (!scan->sendValue(nvalue)) {
            LOG_ERROR("Fail to send CAN frame: " << nvalue->name << endl);
        }
    }
    return true;
}

void SamsungCANPlugin::subscribeProperty()
{
    int cnt = sizeof(subscribedItems)/sizeof(subscribedItems[0]);
    for (int i=0; i<cnt; ++i) {
        routingEngine->subscribeToProperty(subscribedItems[i], &source);
    }
}

void SamsungCANPlugin::setInitPropertyValue()
{
    // Set LeftTemperatureCID, RightTemperatureCID as 17.0
    VehicleProperty::Property items[] = {
        LeftTemperatureCID,
        RightTemperatureCID
    };
    int cnt = sizeof(items)/sizeof(items[0]);
    for (int i=0; i<cnt; ++i) {
        AbstractPropertyType *nvalue;
        GVariant *var;
        nvalue = findPropertyType(items[i], Zone::None);
        if (!nvalue) {
            LOG_ERROR("Fail to find " << items[i] << endl);
            continue;
        }
        var = g_variant_new_double(17.0);
        nvalue->fromVariant(var);
        routingEngine->updateProperty(nvalue, uuid());

        g_variant_unref(var);
    }
}

void SamsungCANPlugin::propertyChanged(AbstractPropertyType *value)
{
    AbstractPropertyType *nvalue;
    GVariant *var;

    if (!value->name.compare(AirDistributionLeftKnob) ||
            !value->name.compare(AirDistributionRightKnob)) {
        // Update AirDistributionCID
        nvalue = findPropertyType(AirDistributionCID, Zone::None);
        if (!nvalue) {
            LOG_ERROR("Fail to find AirDistributionCID" << endl);
            return ;
        }
        var = g_variant_new_byte(value->value<char>());
	    nvalue->internalUpdate = true;
        LOG_INFO("Update Request: " << nvalue->name << " value: " << (int)value->value<char>() << endl);
    } else if (!value->name.compare(LeftAirflowLeftKnob) ||
			!value->name.compare(RightAirflowLeftKnob)) {
        // Update LeftAirflowCID
        nvalue = findPropertyType(LeftAirflowCID, Zone::None);
        if (!nvalue) {
            LOG_ERROR("Fail to find LeftAirflowCID" << endl);
            return ;
        }

        if (value->value<char>() == nvalue->value<char>())  {
            return ;
        }

        var = g_variant_new_byte(value->value<char>());
	    nvalue->internalUpdate = true;
        LOG_INFO("Update Request: " << nvalue->name << " value: " << (int)value->value<char>() << endl);
    } else if (!value->name.compare(LeftTemperatureLeftKnob)) {
        // Update LeftTemperatureLeftKnob
        nvalue = findPropertyType(LeftTemperatureCID, Zone::None);
        if (!nvalue) {
            LOG_ERROR("Fail to find LeftTemperatureCID" << endl);
            return ;
        }
		if (nvalue->value<double>() == value->value<double>()) {
			LOG_INFO("No update since same value" << endl);
			return ;
		}
        var = g_variant_new_double(value->value<double>());
	    nvalue->internalUpdate = true;
        LOG_INFO("Update Request: " << nvalue->name << " value: " << value->value<double>() << endl);
    } else if (!value->name.compare(RightTemperatureRightKnob)) {
        // Update RightTemperatureRightKnob
        nvalue = findPropertyType(RightTemperatureCID, Zone::None);
        if (!nvalue) {
            LOG_ERROR("Fail to find RightTemperatureCID" << endl);
            return ;
        }
		if (nvalue->value<double>() == value->value<double>()) {
			LOG_INFO("No update since same value" << endl);
			return ;
		}
        var = g_variant_new_double(value->value<double>());
	    nvalue->internalUpdate = true;
        LOG_INFO("Update Request: " << nvalue->name << " value: " << value->value<double>() << endl);
    } else if (!value->name.compare(LeftAirflowCID)) {
        var = g_variant_new_byte(value->value<char>());
        AbstractPropertyType *lvalue = findPropertyType(LeftAirflowLeftKnob, Zone::None);
        if (!lvalue) {
            LOG_ERROR("Fail to find LeftAirflowLeftKnob" << endl);
            return ;
        }
        AbstractPropertyType *rvalue = findPropertyType(RightAirflowLeftKnob, Zone::None);
        if (!rvalue) {
            LOG_ERROR("Fail to find RightAirflowLeftKnob" << endl);
            return ;
        }

        if (value->value<char>() != lvalue->value<char>()) {
            lvalue->internalUpdate = true;
            lvalue->fromVariant(var);
            routingEngine->updateProperty(lvalue, uuid());
            LOG_INFO("Update Request: " << lvalue->name << " value: " << value->value<char>() << endl);
        }
        if (value->value<char>() != rvalue->value<char>()) {
            rvalue->internalUpdate = true;
            rvalue->fromVariant(var);
            routingEngine->updateProperty(rvalue, uuid());
            LOG_INFO("Update Request: " << rvalue->name << " value: " << value->value<char>() << endl);
        }

        // No need to send CAN frame
        g_variant_unref(var);
        return ;
    } else if (!value->name.compare(AirDistributionCID)) {
        var = g_variant_new_byte(value->value<char>());
        AbstractPropertyType *lvalue = findPropertyType(AirDistributionLeftKnob, Zone::None);
        if (!lvalue) {
            LOG_ERROR("Fail to find AirDistributionLeftKnob" << endl);
            return ;
        }
        AbstractPropertyType *rvalue = findPropertyType(AirDistributionRightKnob, Zone::None);
        if (!rvalue) {
            LOG_ERROR("Fail to find AirDistributionLeftKnob" << endl);
            return ;
        }

        if (value->value<char>() != lvalue->value<char>()) {
            lvalue->internalUpdate = true;
            lvalue->fromVariant(var);
            routingEngine->updateProperty(lvalue, uuid());
            LOG_INFO("Update Request: " << lvalue->name << " value: " << value->value<char>() << endl);
        }
        if (value->value<char>() != rvalue->value<char>()) {
            rvalue->internalUpdate = true;
            rvalue->fromVariant(var);
            routingEngine->updateProperty(rvalue, uuid());
            LOG_INFO("Update Request: " << rvalue->name << " value: " << value->value<char>() << endl);
        }

        // No need to send CAN frame
        g_variant_unref(var);
        return ;
    } else if (!value->name.compare(LeftTemperatureCID)) {
        nvalue = findPropertyType(LeftTemperatureLeftKnob, Zone::None);
        if (!nvalue) {
            LOG_ERROR("Fail to find LeftTemperatureLeftKnob" << endl);
            return ;
        }

        if (nvalue->value<double>() == value->value<double>()) {
            LOG_INFO("No update since same value" << endl);
            return ;
        }
        var = g_variant_new_double(value->value<double>());
        nvalue->internalUpdate = true;
        nvalue->fromVariant(var);
        routingEngine->updateProperty(nvalue, uuid());
        g_variant_unref(var);
        return ;
    } else if (!value->name.compare(RightTemperatureCID)) {
        nvalue = findPropertyType(RightTemperatureRightKnob, Zone::None);
        if (!nvalue) {
            LOG_ERROR("Fail to find RightTemperatureRightKnob" << endl);
            return ;
        }

        if (nvalue->value<double>() == value->value<double>()) {
            LOG_INFO("No update since same value" << endl);
            return ;
        }
        var = g_variant_new_double(value->value<double>());
        nvalue->internalUpdate = true;
        nvalue->fromVariant(var);
        routingEngine->updateProperty(nvalue, uuid());
        g_variant_unref(var);
        return ;
    } else {
            LOG_ERROR("Fail to find " << value->name << endl);
            return ;
    }

    nvalue->fromVariant(var);
    routingEngine->updateProperty(nvalue, uuid());
    if (!sendValue(nvalue))
        LOG_ERROR("Fail to send CAN frame" << endl);

    g_variant_unref(var);
    return ;
}
