/*****************************************************************
Copyright (C) 2014  Intel Corporation

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *****************************************************************/

#include <boost/assert.hpp>
#include <glib.h>
#include <cstdarg>

#include <vehicleproperty.h>
#include <listplusplus.h>

#include <logger.h>

#include <canbusimpl.h>
#include "samsungcan_plugin.h"
#include "samsungcan_cansignals.h"

//using namespace SamsungCANPlugin;

static const char* DEFAULT_CAN_IF_NAME = "vcan0";

// library exported function for plugin loader
extern "C" AbstractSource * create(AbstractRoutingEngine* routingengine, std::map<std::string, std::string> config)
{
#ifndef UNIT_TESTS
    DEBUG_CONF("SamsungCANPlugin",
        CUtil::Logger::file_off|CUtil::Logger::screen_on,
        CUtil::Logger::EInfo, CUtil::Logger::EInfo
    );
#endif

    std::unique_ptr< AmbPlugin<SamsungCANPlugin> > plugin(new AmbPlugin<SamsungCANPlugin>(routingengine, config));
    plugin->init();
    return plugin.release();
}

void SamsungCANPlugin::timerDestroyNotify(gpointer data)
{
    TimerData* timerData = reinterpret_cast<TimerData*>(data);
    if(timerData){
        SamsungCANPlugin* plugin = std::get<0>(*timerData);
        const can_frame& frame = std::get<3>(*timerData);
        plugin->eraseTimer(frame.can_id);
        delete timerData;
    }

}

gboolean SamsungCANPlugin::timeoutCallback(gpointer data)
{
    TimerData* timerData = reinterpret_cast<TimerData*>(data);
    if(!timerData)
        return false;

    CANBus* bus = std::get<1>(*timerData);
    int retries = --std::get<2>(*timerData);
    const can_frame& frame = std::get<3>(*timerData);

    if(retries > 0){
        if(bus)
            bus->sendStandardFrame(frame);
        return true;
    }
    else return false;
}

//----------------------------------------------------------------------------
// SamsungCANPlugin
//----------------------------------------------------------------------------

SamsungCANPlugin::SamsungCANPlugin(AbstractRoutingEngine* re, const map<string, string>& config, AbstractSource& parent) :
      AmbPluginImpl(re, config, parent),
      interface(DEFAULT_CAN_IF_NAME),
      canBus(new CANBusImpl(*static_cast<CANObserver*>(this))),
      announcementIntervalTimer(1000),
      announcementCount(20)
{
    auto it = config.find("interface");
    if (it != config.end() && it->second.length())
        interface = it->second;

    it = config.find("announcementIntervalTimer");
    if (it != config.end() && it->second.length())
        announcementIntervalTimer = atoi(std::string(it->second).c_str());
    if(announcementIntervalTimer < 20)
        announcementIntervalTimer = 20;

    it = config.find("announcementCount");
    if (it != config.end() && it->second.length())
        announcementCount = atoi(std::string(it->second).c_str());
    if(announcementCount < 1)
            announcementIntervalTimer = 1;

#ifdef GATEWAYBOX
    notificationIntervalTime = 0;
    it = config.find("notificationIntervalTime");
    if (it != config.end() && it->second.length())
        notificationIntervalTime = atoi(std::string(it->second).c_str());
    if (notificationIntervalTime != 0 && notificationIntervalTime < 100)
        notificationIntervalTime = 1000;

    if (notificationIntervalTime)
        g_timeout_add_full(G_PRIORITY_HIGH,
                        notificationIntervalTime,
                        gwbox_callback,
                        this,
                        NULL);
    g_timeout_add_full(G_PRIORITY_HIGH,
		    1000,
		    timeupdate_callback,
		    this,
		    NULL);
#endif /* GATEWAYBOX */

    registerMessages();
}

SamsungCANPlugin::~SamsungCANPlugin()
{
    std::list<guint> timerList;

    mutex.lock();
    for(auto it=timers.begin();it!=timers.end();++it)
        timerList.push_back(it->second);
    mutex.unlock();

    for(auto it=timerList.begin();it!=timerList.end();++it)
        g_source_remove(*it);

    canBus->stop();
}

void SamsungCANPlugin::init()
{
    canBus->start(interface.c_str());

    for(auto iter = messages.begin(); iter != messages.end(); iter++) {
        if (!iter->second.registerOnCANBus(*canBus))
            LOG_ERROR("Cannot register a message with can_id=0x" << std::hex << iter->first);
    }
#ifdef GATEWAYBOX
    subscribeProperty();
    setInitPropertyValue();
#endif /* GATEWAYBOX */
}

AsyncPropertyReply *SamsungCANPlugin::setProperty(const AsyncSetPropertyRequest& request )
{
    AsyncPropertyReply* reply = new AsyncPropertyReply(request);
    reply->success = false;
    reply->error = AsyncPropertyReply::InvalidOperation;

    AbstractPropertyType *value = findPropertyType(request.property, request.zoneFilter);
    if (value && request.value) {
        DebugOut(2) << "updating property "<< request.property << " to: " << request.value->toString() << endl;

        std::unique_ptr<GVariant, decltype(&g_variant_unref)> variant(request.value->toVariant(), &g_variant_unref);
        value->fromVariant(variant.get());

        if(sendValue(request.value)) {
            DebugOut(2) << "New value of property "<< request.property << " is: " << value->toString() << endl;
            value->timestamp = amb::currentTime();
            routingEngine->updateProperty(value, uuid());

            reply->success = true;
            reply->error = AsyncPropertyReply::NoError;
        }
    }

    try {
        if(reply->completed)
            reply->completed(reply);
    }
    catch (...) { }

    return reply;
}

int SamsungCANPlugin::supportedOperations() const
{
    return AbstractSource::Get | AbstractSource::Set;
}

void SamsungCANPlugin::onMessage(const can_frame& frame)
{
    auto messageIt = messages.find(frame.can_id);
    if(messageIt == messages.end())
        return;

    CANMessage& message(messageIt->second);
    const std::string guid = uuid();
    AbstractRoutingEngine* re = routingEngine;
    message.onMessage( frame, [&re, &guid](AbstractPropertyType* value){re->updateProperty(value, guid);} );
}

void SamsungCANPlugin::onTimeout(const can_frame& frame)
{
    auto messageIt = messages.find(frame.can_id);
    if(messageIt == messages.end())
        return;

    CANMessage& message(messageIt->second);
    const std::string guid = uuid();
    AbstractRoutingEngine* re = routingEngine;
    message.onTimeout( frame, [&re, &guid](AbstractPropertyType* value){re->updateProperty(value, guid);} );
}

bool SamsungCANPlugin::sendValue(AbstractPropertyType* value)
{
    if(!value)
        return false;

    auto it = propertyToMessage.find(value->name);
    if(it == propertyToMessage.end())
        return false;

    CANMessage* canMessage = it->second;
    can_frame frame;
    canMessage->setupFrame(&frame);
    return sendStandardFrame(frame);
}

// from CANObserver
void SamsungCANPlugin::errorOccured(CANObserver::CANError error)
{
    (void) error;
    LOG_INFO( "SamsungCANPlugin::errorOccured() not implemented "<< std::endl );
}

void SamsungCANPlugin::standardFrameReceived(const can_frame& frame)
{
    LOG_INFO("SamsungCANPlugin::standardFrameReceived()");
    printFrame( frame );

    onMessage(frame);
}

void SamsungCANPlugin::extendedFrameReceived(const can_frame& frame)
{
    LOG_INFO("SamsungCANPlugin::extendedFrameReceived()");
    printFrame(frame);

    onMessage(frame);
}

void SamsungCANPlugin::timeoutDetected(const can_frame& frame)
{
    LOG_INFO("testPlugin::timeoutDetected()");
    printFrame( frame );

    onTimeout(frame);
}

void SamsungCANPlugin::errorFrameReceived(const can_frame& frame)
{
    LOG_INFO("SamsungCANPlugin::errorFrameReceived()");
    printFrame(frame);
}

void SamsungCANPlugin::remoteTransmissionRequest(const can_frame& frame)
{
    (void) frame;
    LOG_INFO( "SamsungCANPlugin::remoteTransmissionRequest() not implemented "<< std::endl );
}

bool SamsungCANPlugin::sendStandardFrame(const can_frame& frame)
{
    guint oldTimer(0);
    mutex.lock();
    auto it = timers.find(frame.can_id);
    if(it != timers.end()){
        oldTimer = it->second;
        timers.erase(it);
    }
    mutex.unlock();

    if(oldTimer != 0)
        g_source_remove(oldTimer);

    if ( canBus->sendStandardFrame(frame) ) {
        LOG_TRACE( "SamsungCANPlugin::sendStandardFrame() success "<< std::endl );
        scoped_lock<interprocess_recursive_mutex> lock(mutex);
        std::unique_ptr< TimerData > timerData(new TimerData(this, canBus.get(), announcementCount, frame));
        guint timer = g_timeout_add_full(G_PRIORITY_HIGH, announcementIntervalTimer, timeoutCallback, timerData.get(), timerDestroyNotify);
        if(timer){
            timers[frame.can_id] = timer;
            timerData.release();
        }
        return true;
    }
    else {
        LOG_WARNING( "SamsungCANPlugin::sendStandardFrame() failed "<< std::endl );
        return false;
    }
}

bool SamsungCANPlugin::sendExtendedFrame(const can_frame& frame)
{
    if ( canBus->sendExtendedFrame(frame) ) {
        LOG_TRACE( "SamsungCANPlugin::sendExtendedFrame() success "<< std::endl );
        return true;
    }
    else {
        LOG_WARNING( "SamsungCANPlugin::sendExtendedFrame() failed "<< std::endl );
        return false;
    }
}

void SamsungCANPlugin::printFrame(const can_frame& frame) const
{
    LOG_INFO( "SamsungCANPlugin::printFrame can_id: " << std::hex << frame.can_id << std::dec );
    LOG_INFO( "SamsungCANPlugin::printFrame can_dlc: " << int(frame.can_dlc) );

    std::stringstream ss;
    for(int i=0; i<frame.can_dlc; ++i){
        ss << " " << std::hex << (int)(frame.data[i]);
    }
    ss << std::dec;

    LOG_INFO( "SamsungCANPlugin::printFrame can data" << ss.str() );
}

void SamsungCANPlugin::registerMessages()
{
	registerMessage(0x302, 8, 100
				   , new TRIP_B_EllapsedTimeType()
				   , new TRIP_B_Avg_SpeedType()
				   , new TRIP_B_Fuel_UsedType()
				   , new TRIP_B_RangeType()
				   );
	registerMessage(0x301, 8, 100
				   , new TRIP_A_EllapsedTimeType()
				   , new TRIP_A_Avg_SpeedType()
				   , new TRIP_A_Fuel_UsedType()
				   , new TRIP_A_RangeType()
				   );
	registerMessage(0x402, 8, 100
				   , new MediaVolumeCIDType()
				   , new RightAirflowCIDType()
				   , new RightTemperatureCIDType()
				   , new LeftAirflowCIDType()
				   , new LeftTemperatureCIDType()
				   , new AirDistributionCIDType()
				   );
	registerMessage(0x702, 8, 100
				   , new MediaVolumeRightKnobType()
				   , new RightAirflowLeftKnobType()
				   , new RightTemperatureRightKnobType()
				   , new AirDistributionRightKnobType()
				   );
	registerMessage(0x701, 8, 100
				   , new MediaVolumeLeftKnobType()
				   , new LeftAirflowLeftKnobType()
				   , new LeftTemperatureLeftKnobType()
				   , new AirDistributionLeftKnobType()
				   );
	registerMessage(0x401, 8, 100
				   , new CidWatchDDType()
				   , new CidWatchMMType()
				   , new CidWatchYYType()
				   , new CidWatchSecType()
				   , new CidWatchMinType()
				   , new CidWatchHourType()
				   );
	registerMessage(0x207, 8, 100
				   , new FR_KeyEvent24Type()
				   , new FR_KeyEvent23Type()
				   , new FR_KeyEvent22Type()
				   , new FR_KeyEvent21Type()
				   , new FR_KeyEvent20Type()
				   , new FR_KeyEvent19Type()
				   , new FR_KeyEvent18Type()
				   , new FR_KeyEvent17Type()
				   , new FR_KeyEvent16Type()
				   , new FR_KeyEvent15Type()
				   , new FR_KeyEvent14Type()
				   , new FR_KeyEvent13Type()
				   , new FR_KeyEvent12Type()
				   , new FR_KeyEvent11Type()
				   , new FR_KeyEvent10Type()
				   , new FR_KeyEvent09Type()
				   , new FR_KeyEvent08Type()
				   , new FR_KeyEvent07Type()
				   , new FR_KeyEvent06Type()
				   , new FR_KeyEvent05Type()
				   , new FR_KeyEvent04Type()
				   , new FR_KeyEvent03Type()
				   , new FR_KeyEvent02Type()
				   , new FR_KeyEvent01Type()
				   );
	registerMessage(0x206, 8, 100
				   , new CheckSeatCoolerRType()
				   , new CheckSeatCoolerLType()
				   , new CheckSeatHeaterRType()
				   , new CheckSeatHeaterLType()
				   , new CheckInRearType()
				   , new WarningParkingBreakType()
				   , new LampDoorOpenTrunkType()
				   , new LampDoorOpenBonnetType()
				   , new LampDoorOpenR_RType()
				   , new LampDoorOpenR_LType()
				   , new LampDoorOpenF_RType()
				   , new LampDoorOpenF_LType()
				   , new LampCruiseCntSetType()
				   , new LampCruiseCntType()
				   , new LampHighBeamType()
				   , new LampHighBeamAutoType()
				   , new LampTailLightType()
				   , new LampFrontFogType()
				   , new LampHazardLightType()
				   , new LampRearFogLightsType()
				   , new WarningImmobilizerType()
				   , new StsTrunkOpenType()
				   , new StsDoorOpenType()
				   , new WarningABSType()
				   , new SystemESCOffType()
				   , new SystemESCType()
				   , new LowWasherType()
				   , new WarningTPMType()
				   , new FuelLeveltooLowType()
				   , new EmergencyFlasherType()
				   , new CheckEngingType()
				   , new RightTurnSignalType()
				   , new LeftTurnSignalType()
				   , new WarningEBDType()
				   , new WarningBrakeType()
				   , new CheckPowerSteeringType()
				   , new WarningLowOilLevelType()
				   , new CheckChargingType()
				   , new WarningAirbagType()
				   , new WarningSafetybeltsType()
				   );
	registerMessage(0x105, 8, 100
				   , new BatteryChargeLevelType()
				   , new BatteryCurrentType()
				   , new BatteryVoltageType()
				   , new LampAutomaticHoldType()
				   );
	registerMessage(0x104, 8, 100
				   , new TPMS_RRType()
				   , new TPMS_RLType()
				   , new TPMS_FRType()
				   , new TPMS_FLType()
				   );
	registerMessage(0x103, 8, 100
				   , new FuelGageType()
				   , new WarterTemperatureType()
				   , new EngineRPMType()
				   );
	registerMessage(0x102, 8, 100
				   , new VehicleOdometerType()
				   , new VehicleSpeedType()
				   );
	registerMessage(0x101, 8, 100
				   , new AliveCounterType()
				   , new DriveModeType()
				   , new GearboxPositionType()
				   , new GearboxPositionDisplayType()
				   );

}
