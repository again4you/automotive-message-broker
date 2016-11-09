/*
 * Automotive Message Broker Client Library
 *
 * Copyright (C) 2016 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the License)
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __LIBSAMSUNG_CAN_H__
#define __LIBSAMSUNG_CAN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "libamb-util.h"

enum GearboxPositionDisplay {
	PARKING = (1<<3) | (1<<1), // 10, 0x0A
	DRIVE,			   // 11, 0x0B
	NEUTRAL,
	REVERSE,
};

enum DriveMode {
	NORMAL = (1<<3) | (1<<1),	// 10, 0x0A
	ECO = (1<<3) | (1<<2),		// 12, 0x0C
	SPORT, 				// 13, 0x0D
};

/**
 * Gear Information
 * CANID: 257 (0x101)
 */
CAN_OBJECT(GearboxPositionDisplay, guchar, NULL);
CAN_OBJECT(GearboxPosition, guchar, NULL);
CAN_OBJECT(DriveMode, guchar, NULL);
CAN_OBJECT(AliveCounter, guchar, NULL);

/**
 * Vehicle Speed & Odometer
 * CANID: 258 (0x102)
 */
CAN_OBJECT(VehicleSpeed, 	guint16, Speed);
CAN_OBJECT(VehicleOdometer, 	guint32, NULL);

/**
 * Engine Information
 * CANID: 259 (0x103)
 */
CAN_OBJECT(EngineRPM, 		guint16, NULL);
CAN_OBJECT(WarterTemperature, 	guint16, NULL);
CAN_OBJECT(FuelGage, 		guchar,  NULL);

/**
 * TPMS Tire Air pressure
 * CANID: 260 (0x104)
 */
CAN_OBJECT(TPMS_FL, guchar, NULL);
CAN_OBJECT(TPMS_FR, guchar, NULL);
CAN_OBJECT(TPMS_RL, guchar, NULL);
CAN_OBJECT(TPMS_RR, guchar, NULL);

/**
 * Battery Status
 * CANID: 261 (0x105)
 */
CAN_OBJECT(LampAutomaticHold,   guchar, NULL);
CAN_OBJECT(BatteryVoltage,      gdouble, NULL);
CAN_OBJECT(BatteryCurrent,      guchar, NULL);
CAN_OBJECT(BatteryChargeLevel,   guchar, NULL);

/**
 * Warning Message & Status
 * CANID: 518 (0x206)
 */
CAN_OBJECT(WarningSafetybelts, 	gboolean, NULL);
CAN_OBJECT(WarningAirbag, 	gboolean, NULL);
CAN_OBJECT(CheckCharging, 	gboolean, NULL);
CAN_OBJECT(WarningLowOilLevel, 	gboolean, NULL);
CAN_OBJECT(CheckPowerSteering, 	gboolean, NULL);
CAN_OBJECT(WarningBrake, 	gboolean, NULL);
CAN_OBJECT(WarningEBD, 		gboolean, NULL);

CAN_OBJECT(LeftTurnSignal,  gboolean, NULL);
CAN_OBJECT(RightTurnSignal,  gboolean, NULL);

CAN_OBJECT(EmergencyFlasher, 	gboolean, NULL);
CAN_OBJECT(FuelLeveltooLow, 	gboolean, NULL);
CAN_OBJECT(WarningTPM, 		gboolean, NULL);
CAN_OBJECT(LowWasher, 		gboolean, NULL);
CAN_OBJECT(SystemESC, 		gboolean, NULL);
CAN_OBJECT(SystemESCOff, 	gboolean, NULL);

CAN_OBJECT(WarningABS, 		gboolean, NULL);
CAN_OBJECT(StsDoorOpen, 	gboolean, NULL);
CAN_OBJECT(StsTrunkOpen, 	gboolean, NULL);
CAN_OBJECT(WarningImmobilizer, 	gboolean, NULL);
CAN_OBJECT(LampRearFogLights, 	gboolean, NULL);

CAN_OBJECT(LampHazardLight, 	gboolean, NULL);
CAN_OBJECT(LampFrontFog, 	gboolean, NULL);
CAN_OBJECT(LampTailLight, 	gboolean, NULL);
CAN_OBJECT(LampHighBeamAuto, 	gboolean, NULL);
CAN_OBJECT(LampHighBeam, 	gboolean, NULL);
CAN_OBJECT(LampCruiseCnt, 	gboolean, NULL);
CAN_OBJECT(LampCruiseCntSet, 	gboolean, NULL);

CAN_OBJECT(LampDoorOpenF_L, 	gboolean, NULL);
CAN_OBJECT(LampDoorOpenF_R, 	gboolean, NULL);
CAN_OBJECT(LampDoorOpenR_L, 	gboolean, NULL);
CAN_OBJECT(LampDoorOpenR_R, 	gboolean, NULL);
CAN_OBJECT(LampDoorOpenBonnet, 	gboolean, NULL);
CAN_OBJECT(LampDoorOpenTrunk, 	gboolean, NULL);
CAN_OBJECT(WarningParkingBreak, gboolean, NULL);
CAN_OBJECT(CheckInRear, 	gboolean, NULL);

CAN_OBJECT(CheckSeatHeaterL, 	guchar, NULL);
CAN_OBJECT(CheckSeatHeaterR, 	guchar, NULL);
CAN_OBJECT(CheckSeatCoolerL, 	guchar, NULL);
CAN_OBJECT(CheckSeatCoolerR, 	guchar, NULL);

/**
 * Wheel Remote Controller Key Event
 * CANID: 519 (0x207)
 */
CAN_OBJECT(FR_KeyEvent01, 	gboolean, NULL);
CAN_OBJECT(FR_KeyEvent02, 	gboolean, NULL);
CAN_OBJECT(FR_KeyEvent03, 	gboolean, NULL);
CAN_OBJECT(FR_KeyEvent04, 	gboolean, NULL);
CAN_OBJECT(FR_KeyEvent05, 	gboolean, NULL);
CAN_OBJECT(FR_KeyEvent06, 	gboolean, NULL);
CAN_OBJECT(FR_KeyEvent07, 	gboolean, NULL);
CAN_OBJECT(FR_KeyEvent08, 	gboolean, NULL);

CAN_OBJECT(FR_KeyEvent09, 	gboolean, NULL);
CAN_OBJECT(FR_KeyEvent10, 	gboolean, NULL);
CAN_OBJECT(FR_KeyEvent11, 	gboolean, NULL);
CAN_OBJECT(FR_KeyEvent12, 	gboolean, NULL);
CAN_OBJECT(FR_KeyEvent13, 	gboolean, NULL);
CAN_OBJECT(FR_KeyEvent14, 	gboolean, NULL);
CAN_OBJECT(FR_KeyEvent15, 	gboolean, NULL);
CAN_OBJECT(FR_KeyEvent16, 	gboolean, NULL);

CAN_OBJECT(FR_KeyEvent17, 	gboolean, NULL);
CAN_OBJECT(FR_KeyEvent18, 	gboolean, NULL);
CAN_OBJECT(FR_KeyEvent19, 	gboolean, NULL);
CAN_OBJECT(FR_KeyEvent20, 	gboolean, NULL);
CAN_OBJECT(FR_KeyEvent21, 	gboolean, NULL);
CAN_OBJECT(FR_KeyEvent22, 	gboolean, NULL);
CAN_OBJECT(FR_KeyEvent23, 	gboolean, NULL);
CAN_OBJECT(FR_KeyEvent24, 	gboolean, NULL);

/**
 * CID Time Information
 * CANID: 1025 (0x401)
 */
CAN_OBJECT(CidWatchHour, 	guchar, NULL);
CAN_OBJECT(CidWatchMin, 	guchar, NULL);
CAN_OBJECT(CidWatchSec, 	guchar, NULL);
CAN_OBJECT(CidWatchYY, 	    guint16, NULL);
CAN_OBJECT(CidWatchMM, 	    guchar, NULL);
CAN_OBJECT(CidWatchDD, 	    guchar, NULL);

/**
 * CID Climate & Media Volumn Information
 * CANID: 1026 (0x402)
 */
CAN_OBJECT(AirDistributionCID,  guchar, NULL);
CAN_OBJECT(LeftTemperatureCID,  gdouble, NULL);
CAN_OBJECT(LeftAirflowCID,      guchar, NULL);
CAN_OBJECT(RightTemperatureCID, gdouble, NULL);
CAN_OBJECT(RightAirflowCID,     guchar, NULL);
CAN_OBJECT(MediaVolumeCID,      guchar, NULL);

/**
 * Cluster Trip Information A
 * CANID: 769 (0x301)
 */
CAN_OBJECT(TRIP_A_Range,        guint32, NULL);
CAN_OBJECT(TRIP_A_Fuel_Used,    guchar, NULL);
CAN_OBJECT(TRIP_A_Avg_Speed,    guint16, NULL);
CAN_OBJECT(TRIP_A_EllapsedTime, guint16, NULL);

/**
 * Cluster Trip Information B
 * CANID: 770 (0x302)
 */
CAN_OBJECT(TRIP_B_Range,        guint32, NULL);
CAN_OBJECT(TRIP_B_Fuel_Used,    guchar, NULL);
CAN_OBJECT(TRIP_B_Avg_Speed,    guint16, NULL);
CAN_OBJECT(TRIP_B_EllapsedTime, guint16, NULL);

/**
 * Left Knob Control
 * CANID: 1793 (0x701)
 */
CAN_OBJECT(AirDistributionLeftKnob,     guchar, NULL);
CAN_OBJECT(LeftTemperatureLeftKnob,     gdouble, NULL);
CAN_OBJECT(LeftAirflowLeftKnob,         guchar, NULL);
CAN_OBJECT(MediaVolumeLeftKnob,         guchar, NULL);

/**
 * Right Knob Control
 * CANID: 1794 (0x702)
 */
CAN_OBJECT(AirDistributionRightKnob,    guchar, NULL);
CAN_OBJECT(RightTemperatureRightKnob,   gdouble, NULL);
CAN_OBJECT(RightAirflowLeftKnob,        guchar, NULL);
CAN_OBJECT(MediaVolumeRightKnob,        guchar, NULL);

/**
 * Writable AMB Object
 * Climate
 */
CAN_OBJECT_WRITABLE(CheckSeatHeaterL,	    guchar, NULL);
CAN_OBJECT_WRITABLE(CheckSeatHeaterR,	    guchar, NULL);
CAN_OBJECT_WRITABLE(CheckSeatCoolerL,	    guchar, NULL);
CAN_OBJECT_WRITABLE(CheckSeatCoolerR,	    guchar, NULL);
CAN_OBJECT_WRITABLE(AirDistributionCID,	    guchar, NULL);
CAN_OBJECT_WRITABLE(LeftTemperatureCID,	    gdouble, NULL);
CAN_OBJECT_WRITABLE(LeftAirflowCID,	        guchar, NULL);
CAN_OBJECT_WRITABLE(RightTemperatureCID,    gdouble, NULL);
CAN_OBJECT_WRITABLE(RightAirflowCID,	    guchar, NULL);

/**
 * Writable AMB Object
 * Media Volumn
 */
CAN_OBJECT_WRITABLE(MediaVolumeCID,     guchar, NULL);

/**
 * Writable AMB Object
 * Left Knob Control
 */
CAN_OBJECT_WRITABLE(AirDistributionLeftKnob,    guchar, NULL);
CAN_OBJECT_WRITABLE(LeftTemperatureLeftKnob,    gdouble, NULL);
CAN_OBJECT_WRITABLE(LeftAirflowLeftKnob,        guchar, NULL);
CAN_OBJECT_WRITABLE(MediaVolumeLeftKnob,        guchar, NULL);

/**
 * Writable AMB Object
 * Right Knob Control
 */
CAN_OBJECT_WRITABLE(AirDistributionRightKnob,   guchar, NULL);
CAN_OBJECT_WRITABLE(RightTemperatureRightKnob,  gdouble, NULL);
CAN_OBJECT_WRITABLE(RightAirflowLeftKnob,       guchar, NULL);
CAN_OBJECT_WRITABLE(MediaVolumeRightKnob,       guchar, NULL);

#ifdef __cplusplus
}
#endif

#endif /* __LIBSAMSUNG_CAN_H__ */
