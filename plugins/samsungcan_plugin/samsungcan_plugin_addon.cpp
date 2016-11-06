#include <boost/assert.hpp>
#include <glib.h>
#include <cstdarg>

#include <vehicleproperty.h>
#include <listplusplus.h>

#include <logger.h>

#include <canbusimpl.h>
#include "samsungcan_plugin.h"
#include "samsungcan_cansignals.h"


#ifdef GATEWAYBOX
VehicleProperty::Property notiItems[] = {
    CidWatchHour,
    AirDistributionCID
};

gboolean SamsungCANPlugin::gwbox_callback(gpointer data)
{
    SamsungCANPlugin *scan = (SamsungCANPlugin *)data;
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

gboolean SamsungCANPlugin::timeupdate_callback(gpointer data)
{
    time_t ctime;
    struct tm ts;
    unsigned int cnt;
    SamsungCANPlugin *scan = (SamsungCANPlugin *)data;
    VehicleProperty::Property timeItems[] = {
        CidWatchHour,
	CidWatchMin,
	CidWatchSec,
	CidWatchYY,
	CidWatchMM,
	CidWatchDD
    };
    cnt = sizeof(timeItems)/sizeof(timeItems[0]);

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
	} else if (!nvalue->name.compare(CidWatchSec)) {
            var = g_variant_new_byte(ts.tm_sec);
	}
	nvalue->fromVariant(var);
	g_variant_unref(var);
    }
    return true;
}
#endif /* GATEWAYBOX */

