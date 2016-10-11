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

/**
 * Vehicle Speed & Odometer
 * CANID: 258 (0x102)
 */
CAN_OBJECT(VehicleOdometer, 	guint32, NULL);
CAN_OBJECT(VehicleSpeed, 	guint16, Speed);

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
CAN_OBJECT(CheckEnging, 	gboolean, NULL);
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
CAN_OBJECT(LampDoorOpenR_R, 	gboolean, NULL);
CAN_OBJECT(LampDoorOpenR_L, 	gboolean, NULL);
CAN_OBJECT(LampDoorOpenBonnet, 	gboolean, NULL);
CAN_OBJECT(LampDoorOpenTrunk, 	gboolean, NULL);
CAN_OBJECT(WarningParkingBreak, gboolean, NULL);

#ifdef __cplusplus
}
#endif

#endif /* __LIBSAMSUNG_CAN_H__ */
