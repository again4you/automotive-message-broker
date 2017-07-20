
#include <glib.h>
#include <gio/gio.h>
#include <gio/gunixsocketaddress.h>

#include <logger.h>

#include "samsungcan_plugin.h"
#include "samsungcan_cansignals.h"

#define SOCK_PATH   "/tmp/amb_socket"
#define BUFSIZE     1024

const int MAX_VOLUMN = 15;
const int MIN_VOLUMN = 0;

struct udsmsg_prop {
    int32_t zone;
    uint32_t len;
    uint8_t name[0];
};

struct udsmsg_val {
    int32_t res;
    uint16_t type; /* gvariant type char */
    uint16_t len;
    uint8_t val[0];
};

enum udsmsg_type {
    UT_GET_REQ,
    UT_GET_RES,
    UT_SET_REQ,
    UT_SET_RES,
    UT_END,
};

struct udsmsg_head {
    uint32_t type; /* enum UDSMSG_TYPE */
    uint32_t len;
};

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

static int udsmsg_fill_res(uint8_t *buf, uint32_t size,
        enum udsmsg_type type, int zone, const char *name,
        int res, uint8_t vtype, uint16_t vsz, void *value)
{
    int len;
    struct udsmsg_head *uh;
    struct udsmsg_prop *up;
    struct udsmsg_val *uv;

    if (!buf || size == 0 || !name)
        return -1;

    len = strlen(name) + 1;
    if (len + vsz + sizeof(*uh) + sizeof(*up) + sizeof(*uv) > size)
        return -1;

    // uv = (void *)buf + sizeof(*uh) + sizeof(*up) + len;
    uv = (struct udsmsg_val *)((uint8_t *)buf + sizeof(*uh) + sizeof(*up) + len);
    uv->res = res;
    if (res == 0) {
        uv->type = vtype;
        uv->len = vsz;
        if (value && vsz > 0)
            memcpy(uv->val, value, vsz);
    }

    // up = (void *)buf + sizeof(*uh);
    up = (struct udsmsg_prop *)((uint8_t *)buf + sizeof(*uh));
    up->zone = zone;
    up->len = len;
    memcpy(up->name, name, len);

    // uh = (void *)buf;
    uh = (struct udsmsg_head *)buf;
    uh->type = type;
    uh->len = sizeof(*uh) + sizeof(*up) + len + sizeof(*uv) + vsz;

    return uh->len;
}

static int udsmsg_fill_get_res(uint8_t *buf, uint32_t size,
        int zone, const char *name,
        int res, uint8_t vtype, uint16_t vsz, void *value)
{
    return udsmsg_fill_res(buf, size, UT_GET_RES, zone, name,
            res, vtype, vsz, value);
}

static int udsmsg_fill_set_res(uint8_t *buf, uint32_t size,
        int zone, const char *name,
        int res, uint8_t vtype, uint16_t vsz, void *value)
{
    return udsmsg_fill_res(buf, size, UT_SET_RES, zone, name,
            res, vtype, vsz, value);
}

gboolean
SamsungCANPlugin::socketSessionHandler(GIOChannel *channel,
                                    GIOCondition condition,
                                    gpointer data)
{
    GError *error;
    GSocket *sock_client;
    int fd;
    gssize nread;

    struct udsmsg_head uh;
    struct udsmsg_prop *up;
    struct udsmsg_val *uv;

    gchar buf[BUFSIZE];
    gchar out[BUFSIZE];

    SamsungCANPlugin *scan = (SamsungCANPlugin *)data;

    if (condition & (G_IO_HUP | G_IO_ERR | G_IO_NVAL))
        return FALSE;

    fd = g_io_channel_unix_get_fd(channel);
    error = NULL;
    sock_client = g_socket_new_from_fd(fd, &error);
    if (error) {
        LOG_ERROR("Fail to g_socket_new_from_fd(): " << error->message << endl);
        g_error_free(error);
        return FALSE;
    }

    nread = g_socket_receive(sock_client, (gchar *)&uh, sizeof(uh), NULL, &error);
    if (error) {
        LOG_ERROR("Fail to g_socket_receive(): " << error->message << endl);
        g_error_free(error);
        return FALSE;
    }

    nread = g_socket_receive(sock_client, (gchar *)buf, uh.len - sizeof(uh), NULL, &error);
    if (error) {
        LOG_ERROR("Fail to g_socket_receive(): " << error->message << endl);
        g_error_free(error);
        return FALSE;
    }

    if (uh.type == UT_GET_REQ) {
        // TODO
        // int i = 100;

        up = (struct udsmsg_prop *)buf;
        LOG_INFO("Get " << up->zone << " " << up->len << " " << "[" << up->name << "]" << endl);

        // VehicleOdometer get test
        AbstractPropertyType *nvalue = scan->findPropertyType(VehicleOdometer, Zone::None);
        if (!nvalue) {
            LOG_ERROR("Fail to find findPropertyType()" << endl);
            return FALSE;
        }
        LOG_INFO("Object: " << nvalue->name << " value: " << nvalue->value<uint32_t>() << endl);

        uint32_t v = nvalue->value<uint32_t>();


        nread = udsmsg_fill_get_res((uint8_t *)out, sizeof(out),
                    up->zone, (const char *)nvalue->name.c_str(),
                    0, 'i', sizeof(v), &v);
#if 0
        nread = udsmsg_fill_get_res((uint8_t *)out, sizeof(out),
                    up->zone, (const char *)up->name,
                    0, 'i', sizeof(i), &i);
#endif
    } else if (uh.type == UT_SET_REQ) {
        // TODO
        up = (struct udsmsg_prop *)buf;
        uv = (struct udsmsg_val *)((uint8_t *)buf + sizeof(*up) + up->len);

        LOG_INFO("Set " << up->zone << " " << up->len << " " << "[" << up->name << "] "
                << (char)uv->type << " " << uv->len << endl);

        AbstractPropertyType *nvalue = scan->findPropertyType(VehicleOdometer, Zone::None);
        if (!nvalue) {
            LOG_ERROR("Fail to find findPropertyType()" << endl);
            return FALSE;
        }

        GVariant *var = g_variant_new_uint32(*(int *)uv->val);
        nvalue->fromVariant(var);
        scan->routingEngine->updateProperty(nvalue, scan->uuid());
        g_variant_unref(var);

        nread = udsmsg_fill_set_res((uint8_t *)out, sizeof(out),
                    up->zone, (const char *)up->name,
                    0, uv->type, uv->len, uv->val);

    } else {
        LOG_ERROR("Unknown Req type: " << uh.type);
        return FALSE;
    }

    g_socket_send(sock_client, (gchar *)out, nread, NULL, &error);
    if (error) {
        LOG_ERROR("Fail to g_socket_receive(): " << error->message << endl);
        g_error_free(error);
        return FALSE;
    }

    g_object_unref(sock_client);
    return TRUE;
}

gboolean
SamsungCANPlugin::socketAcceptHandler(GIOChannel *channel,
                                    GIOCondition condition,
                                    gpointer data)
{
    GError *error;
    GIOChannel *io_client;
    GSocket *sock_client;
    int fd;

    SamsungCANPlugin *scan = (SamsungCANPlugin *)data;

    if (condition & (G_IO_HUP | G_IO_ERR | G_IO_NVAL))
        return FALSE;

    error = NULL;
    sock_client = g_socket_accept(scan->socket, NULL, &error);
    if (!sock_client) {
        LOG_ERROR("Fail to g_socket_accept(): " << error->message << endl);
        g_error_free(error);
        return FALSE;
    }

    fd = g_socket_get_fd(sock_client);
    io_client = g_io_channel_unix_new(fd);
    g_io_add_watch(io_client,
                (GIOCondition)(G_IO_IN | G_IO_HUP | G_IO_ERR | G_IO_NVAL),
                (GIOFunc)socketSessionHandler,
                data);

    g_io_channel_unref(io_client);
    return TRUE;
}

int SamsungCANPlugin::initUDS()
{
    GError *error;
    int sock_fd;

    // cleanup
    if (!access(SOCK_PATH, F_OK))
        unlink(SOCK_PATH);

    // socket create
    error = NULL;
    this->socket = g_socket_new(G_SOCKET_FAMILY_UNIX,
                            G_SOCKET_TYPE_STREAM,
                            G_SOCKET_PROTOCOL_DEFAULT,
                            &error);
    if (error != NULL) {
        LOG_ERROR("Fail to g_socket_new(): " << error->message << endl);
        return -1;
    }

    // server address setting
    this->sock_addr = g_unix_socket_address_new(SOCK_PATH);

    // bind
    if (!g_socket_bind(this->socket, this->sock_addr, TRUE, &error)) {
        LOG_ERROR("Fail to g_socket_bind(): " << error->message << endl);
        return -1;
    }

    // listen
    if (!g_socket_listen(this->socket, &error)) {
        LOG_ERROR("Fail to g_socket_listen(): " << error->message << endl);
        return -1;
    }

    sock_fd = g_socket_get_fd(this->socket);
    this->channel = g_io_channel_unix_new(sock_fd);
    g_io_add_watch(this->channel,
                (GIOCondition)(G_IO_IN | G_IO_ERR | G_IO_HUP | G_IO_NVAL),
                (GIOFunc)socketAcceptHandler,
                this);

    AbstractPropertyType *nvalue = findPropertyType(VehicleOdometer, Zone::None);
    if (!nvalue) {
        LOG_ERROR("Fail to find findPropertyType()" << endl);
        return FALSE;
    }
    LOG_INFO("SJ Object: " << nvalue->name << " value: " << nvalue->value<uint32_t>() << endl);

    LOG_INFO("Complete to initUDS()");
    return 0;
}
