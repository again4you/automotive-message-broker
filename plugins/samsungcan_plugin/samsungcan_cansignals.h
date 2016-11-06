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

#ifndef SAMSUNGCAN_CANSIGNALS_H_
#define SAMSUNGCAN_CANSIGNALS_H_

#include "samsungcan_plugin.h"
#include "samsungcan_cansignal.h"

#include <glib.h>
#include <vehicleproperty.h>


/**< TRIP_B_EllapsedTime.
 */
const VehicleProperty::Property TRIP_B_EllapsedTime = "TRIP_B_EllapsedTime";
CANSIGNAL(TRIP_B_EllapsedTime, uint16_t, 48, 16, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<uint16_t>(0), static_cast<uint16_t>(65535), nullptr, nullptr)

/**< TRIP_B_Avg_Speed.
 */
const VehicleProperty::Property TRIP_B_Avg_Speed = "TRIP_B_Avg_Speed";
CANSIGNAL(TRIP_B_Avg_Speed, uint16_t, 32, 16, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<uint16_t>(0), static_cast<uint16_t>(512), nullptr, nullptr)

/**< TRIP_B_Fuel_Used.
 */
const VehicleProperty::Property TRIP_B_Fuel_Used = "TRIP_B_Fuel_Used";
CANSIGNAL(TRIP_B_Fuel_Used, char, 24, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(255), nullptr, nullptr)

/**< TRIP_B_Range.
 */
const VehicleProperty::Property TRIP_B_Range = "TRIP_B_Range";
CANSIGNAL(TRIP_B_Range, uint32_t, 0, 24, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<uint32_t>(0), static_cast<uint32_t>(999999), nullptr, nullptr)

/**< TRIP_A_EllapsedTime.
 */
const VehicleProperty::Property TRIP_A_EllapsedTime = "TRIP_A_EllapsedTime";
CANSIGNAL(TRIP_A_EllapsedTime, uint16_t, 48, 16, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<uint16_t>(0), static_cast<uint16_t>(65535), nullptr, nullptr)

/**< TRIP_A_Avg_Speed.
 */
const VehicleProperty::Property TRIP_A_Avg_Speed = "TRIP_A_Avg_Speed";
CANSIGNAL(TRIP_A_Avg_Speed, uint16_t, 32, 16, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<uint16_t>(0), static_cast<uint16_t>(512), nullptr, nullptr)

/**< TRIP_A_Fuel_Used.
 */
const VehicleProperty::Property TRIP_A_Fuel_Used = "TRIP_A_Fuel_Used";
CANSIGNAL(TRIP_A_Fuel_Used, char, 24, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(255), nullptr, nullptr)

/**< TRIP_A_Range.
 */
const VehicleProperty::Property TRIP_A_Range = "TRIP_A_Range";
CANSIGNAL(TRIP_A_Range, uint32_t, 0, 24, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<uint32_t>(0), static_cast<uint32_t>(999999), nullptr, nullptr)

/**< MediaVolumeCID.
 */
const VehicleProperty::Property MediaVolumeCID = "MediaVolumeCID";
CANSIGNAL(MediaVolumeCID, char, 56, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(15), nullptr, nullptr)

/**< RightAirflowCID.
 */
const VehicleProperty::Property RightAirflowCID = "RightAirflowCID";
CANSIGNAL(RightAirflowCID, char, 48, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(5), nullptr, nullptr)

/**< RightTemperatureCID.
 */
const VehicleProperty::Property RightTemperatureCID = "RightTemperatureCID";
CANSIGNAL(RightTemperatureCID, double, 40, 8, Endian::Intel, Signedness::Unsigned, 0.1, -170, static_cast<double>(0), static_cast<double>(150), nullptr, nullptr)

/**< LeftAirflowCID.
 */
const VehicleProperty::Property LeftAirflowCID = "LeftAirflowCID";
CANSIGNAL(LeftAirflowCID, char, 32, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(5), nullptr, nullptr)

/**< LeftTemperatureCID.
 */
const VehicleProperty::Property LeftTemperatureCID = "LeftTemperatureCID";
CANSIGNAL(LeftTemperatureCID, double, 24, 8, Endian::Intel, Signedness::Unsigned, 0.1, -170, static_cast<double>(0), static_cast<double>(0), nullptr, nullptr)

/**< AirDistributionCID.
 */
const VehicleProperty::Property AirDistributionCID = "AirDistributionCID";
CANSIGNAL(AirDistributionCID, char, 16, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(4), nullptr, nullptr)

/**< MediaVolumeRightKnob.
 */
const VehicleProperty::Property MediaVolumeRightKnob = "MediaVolumeRightKnob";
CANSIGNAL(MediaVolumeRightKnob, char, 56, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(15), nullptr, nullptr)

/**< RightAirflowLeftKnob.
 */
const VehicleProperty::Property RightAirflowLeftKnob = "RightAirflowLeftKnob";
CANSIGNAL(RightAirflowLeftKnob, char, 48, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(5), nullptr, nullptr)

/**< RightTemperatureRightKnob.
 */
const VehicleProperty::Property RightTemperatureRightKnob = "RightTemperatureRightKnob";
CANSIGNAL(RightTemperatureRightKnob, double, 40, 8, Endian::Intel, Signedness::Unsigned, 0.1, 170, static_cast<double>(0), static_cast<double>(150), nullptr, nullptr)

/**< AirDistributionRightKnob.
 */
const VehicleProperty::Property AirDistributionRightKnob = "AirDistributionRightKnob";
CANSIGNAL(AirDistributionRightKnob, char, 32, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(4), nullptr, nullptr)

/**< MediaVolumeLeftKnob.
 */
const VehicleProperty::Property MediaVolumeLeftKnob = "MediaVolumeLeftKnob";
CANSIGNAL(MediaVolumeLeftKnob, char, 56, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(15), nullptr, nullptr)

/**< LeftAirflowLeftKnob.
 */
const VehicleProperty::Property LeftAirflowLeftKnob = "LeftAirflowLeftKnob";
CANSIGNAL(LeftAirflowLeftKnob, char, 48, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(5), nullptr, nullptr)

/**< LeftTemperatureLeftKnob.
 */
const VehicleProperty::Property LeftTemperatureLeftKnob = "LeftTemperatureLeftKnob";
CANSIGNAL(LeftTemperatureLeftKnob, double, 40, 8, Endian::Intel, Signedness::Unsigned, 0.1, 170, static_cast<double>(0), static_cast<double>(150), nullptr, nullptr)

/**< AirDistributionLeftKnob.
 */
const VehicleProperty::Property AirDistributionLeftKnob = "AirDistributionLeftKnob";
CANSIGNAL(AirDistributionLeftKnob, char, 32, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(4), nullptr, nullptr)

/**< CidWatchDD.
 */
const VehicleProperty::Property CidWatchDD = "CidWatchDD";
CANSIGNAL(CidWatchDD, char, 56, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(1), static_cast<char>(31), nullptr, nullptr)

/**< CidWatchMM.
 */
const VehicleProperty::Property CidWatchMM = "CidWatchMM";
CANSIGNAL(CidWatchMM, char, 48, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(1), static_cast<char>(12), nullptr, nullptr)

/**< CidWatchYY.
 */
const VehicleProperty::Property CidWatchYY = "CidWatchYY";
CANSIGNAL(CidWatchYY, uint16_t, 32, 16, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<uint16_t>(0), static_cast<uint16_t>(9999), nullptr, nullptr)

/**< CidWatchSec.
 */
const VehicleProperty::Property CidWatchSec = "CidWatchSec";
CANSIGNAL(CidWatchSec, char, 16, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(59), nullptr, nullptr)

/**< CidWatchMin.
 */
const VehicleProperty::Property CidWatchMin = "CidWatchMin";
CANSIGNAL(CidWatchMin, char, 8, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(59), nullptr, nullptr)

/**< CidWatchHour.
 */
const VehicleProperty::Property CidWatchHour = "CidWatchHour";
CANSIGNAL(CidWatchHour, char, 0, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(1), static_cast<char>(24), nullptr, nullptr)

/**< FR_KeyEvent24.
 */
const VehicleProperty::Property FR_KeyEvent24 = "FR_KeyEvent24";
CANSIGNAL(FR_KeyEvent24, bool, 23, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< FR_KeyEvent23.
 */
const VehicleProperty::Property FR_KeyEvent23 = "FR_KeyEvent23";
CANSIGNAL(FR_KeyEvent23, bool, 22, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< FR_KeyEvent22.
 */
const VehicleProperty::Property FR_KeyEvent22 = "FR_KeyEvent22";
CANSIGNAL(FR_KeyEvent22, bool, 21, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< FR_KeyEvent21.
 */
const VehicleProperty::Property FR_KeyEvent21 = "FR_KeyEvent21";
CANSIGNAL(FR_KeyEvent21, bool, 20, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< FR_KeyEvent20.
 */
const VehicleProperty::Property FR_KeyEvent20 = "FR_KeyEvent20";
CANSIGNAL(FR_KeyEvent20, bool, 19, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< FR_KeyEvent19.
 */
const VehicleProperty::Property FR_KeyEvent19 = "FR_KeyEvent19";
CANSIGNAL(FR_KeyEvent19, bool, 18, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< FR_KeyEvent18.
 */
const VehicleProperty::Property FR_KeyEvent18 = "FR_KeyEvent18";
CANSIGNAL(FR_KeyEvent18, bool, 17, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< FR_KeyEvent17.
 */
const VehicleProperty::Property FR_KeyEvent17 = "FR_KeyEvent17";
CANSIGNAL(FR_KeyEvent17, bool, 16, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< FR_KeyEvent16.
 */
const VehicleProperty::Property FR_KeyEvent16 = "FR_KeyEvent16";
CANSIGNAL(FR_KeyEvent16, bool, 15, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< FR_KeyEvent15.
 */
const VehicleProperty::Property FR_KeyEvent15 = "FR_KeyEvent15";
CANSIGNAL(FR_KeyEvent15, bool, 14, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< FR_KeyEvent14.
 */
const VehicleProperty::Property FR_KeyEvent14 = "FR_KeyEvent14";
CANSIGNAL(FR_KeyEvent14, bool, 13, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< FR_KeyEvent13.
 */
const VehicleProperty::Property FR_KeyEvent13 = "FR_KeyEvent13";
CANSIGNAL(FR_KeyEvent13, bool, 12, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< FR_KeyEvent12.
 */
const VehicleProperty::Property FR_KeyEvent12 = "FR_KeyEvent12";
CANSIGNAL(FR_KeyEvent12, bool, 11, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< FR_KeyEvent11.
 */
const VehicleProperty::Property FR_KeyEvent11 = "FR_KeyEvent11";
CANSIGNAL(FR_KeyEvent11, bool, 10, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< FR_KeyEvent10.
 */
const VehicleProperty::Property FR_KeyEvent10 = "FR_KeyEvent10";
CANSIGNAL(FR_KeyEvent10, bool, 9, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< FR_KeyEvent09.
 */
const VehicleProperty::Property FR_KeyEvent09 = "FR_KeyEvent09";
CANSIGNAL(FR_KeyEvent09, bool, 8, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< FR_KeyEvent08.
 */
const VehicleProperty::Property FR_KeyEvent08 = "FR_KeyEvent08";
CANSIGNAL(FR_KeyEvent08, bool, 7, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< FR_KeyEvent07.
 */
const VehicleProperty::Property FR_KeyEvent07 = "FR_KeyEvent07";
CANSIGNAL(FR_KeyEvent07, bool, 6, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< FR_KeyEvent06.
 */
const VehicleProperty::Property FR_KeyEvent06 = "FR_KeyEvent06";
CANSIGNAL(FR_KeyEvent06, bool, 5, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< FR_KeyEvent05.
 */
const VehicleProperty::Property FR_KeyEvent05 = "FR_KeyEvent05";
CANSIGNAL(FR_KeyEvent05, bool, 4, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< FR_KeyEvent04.
 */
const VehicleProperty::Property FR_KeyEvent04 = "FR_KeyEvent04";
CANSIGNAL(FR_KeyEvent04, bool, 3, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< FR_KeyEvent03.
 */
const VehicleProperty::Property FR_KeyEvent03 = "FR_KeyEvent03";
CANSIGNAL(FR_KeyEvent03, bool, 2, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< FR_KeyEvent02.
 */
const VehicleProperty::Property FR_KeyEvent02 = "FR_KeyEvent02";
CANSIGNAL(FR_KeyEvent02, bool, 1, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< FR_KeyEvent01.
 */
const VehicleProperty::Property FR_KeyEvent01 = "FR_KeyEvent01";
CANSIGNAL(FR_KeyEvent01, bool, 0, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< CheckSeatCoolerR.
 */
const VehicleProperty::Property CheckSeatCoolerR = "CheckSeatCoolerR";
CANSIGNAL(CheckSeatCoolerR, char, 54, 2, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(3), nullptr, nullptr)

/**< CheckSeatCoolerL.
 */
const VehicleProperty::Property CheckSeatCoolerL = "CheckSeatCoolerL";
CANSIGNAL(CheckSeatCoolerL, char, 52, 2, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(3), nullptr, nullptr)

/**< CheckSeatHeaterR.
 */
const VehicleProperty::Property CheckSeatHeaterR = "CheckSeatHeaterR";
CANSIGNAL(CheckSeatHeaterR, char, 50, 2, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(3), nullptr, nullptr)

/**< CheckSeatHeaterL.
 */
const VehicleProperty::Property CheckSeatHeaterL = "CheckSeatHeaterL";
CANSIGNAL(CheckSeatHeaterL, char, 48, 2, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(3), nullptr, nullptr)

/**< CheckInRear.
 */
const VehicleProperty::Property CheckInRear = "CheckInRear";
CANSIGNAL(CheckInRear, bool, 47, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< WarningParkingBreak.
 */
const VehicleProperty::Property WarningParkingBreak = "WarningParkingBreak";
CANSIGNAL(WarningParkingBreak, bool, 46, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< LampDoorOpenTrunk.
 */
const VehicleProperty::Property LampDoorOpenTrunk = "LampDoorOpenTrunk";
CANSIGNAL(LampDoorOpenTrunk, bool, 45, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< LampDoorOpenBonnet.
 */
const VehicleProperty::Property LampDoorOpenBonnet = "LampDoorOpenBonnet";
CANSIGNAL(LampDoorOpenBonnet, bool, 44, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< LampDoorOpenR_R.
 */
const VehicleProperty::Property LampDoorOpenR_R = "LampDoorOpenR_R";
CANSIGNAL(LampDoorOpenR_R, bool, 43, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< LampDoorOpenR_L.
 */
const VehicleProperty::Property LampDoorOpenR_L = "LampDoorOpenR_L";
CANSIGNAL(LampDoorOpenR_L, bool, 42, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< LampDoorOpenF_R.
 */
const VehicleProperty::Property LampDoorOpenF_R = "LampDoorOpenF_R";
CANSIGNAL(LampDoorOpenF_R, bool, 41, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< LampDoorOpenF_L.
 */
const VehicleProperty::Property LampDoorOpenF_L = "LampDoorOpenF_L";
CANSIGNAL(LampDoorOpenF_L, bool, 40, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< LampCruiseCntSet.
 */
const VehicleProperty::Property LampCruiseCntSet = "LampCruiseCntSet";
CANSIGNAL(LampCruiseCntSet, bool, 38, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< LampCruiseCnt.
 */
const VehicleProperty::Property LampCruiseCnt = "LampCruiseCnt";
CANSIGNAL(LampCruiseCnt, bool, 37, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< LampHighBeam.
 */
const VehicleProperty::Property LampHighBeam = "LampHighBeam";
CANSIGNAL(LampHighBeam, bool, 36, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< LampHighBeamAuto.
 */
const VehicleProperty::Property LampHighBeamAuto = "LampHighBeamAuto";
CANSIGNAL(LampHighBeamAuto, bool, 35, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< LampTailLight.
 */
const VehicleProperty::Property LampTailLight = "LampTailLight";
CANSIGNAL(LampTailLight, bool, 34, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< LampFrontFog.
 */
const VehicleProperty::Property LampFrontFog = "LampFrontFog";
CANSIGNAL(LampFrontFog, bool, 33, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< LampHazardLight.
 */
const VehicleProperty::Property LampHazardLight = "LampHazardLight";
CANSIGNAL(LampHazardLight, bool, 32, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< LampRearFogLights.
 */
const VehicleProperty::Property LampRearFogLights = "LampRearFogLights";
CANSIGNAL(LampRearFogLights, bool, 28, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< WarningImmobilizer.
 */
const VehicleProperty::Property WarningImmobilizer = "WarningImmobilizer";
CANSIGNAL(WarningImmobilizer, bool, 27, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< StsTrunkOpen.
 */
const VehicleProperty::Property StsTrunkOpen = "StsTrunkOpen";
CANSIGNAL(StsTrunkOpen, bool, 26, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< StsDoorOpen.
 */
const VehicleProperty::Property StsDoorOpen = "StsDoorOpen";
CANSIGNAL(StsDoorOpen, bool, 25, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< WarningABS.
 */
const VehicleProperty::Property WarningABS = "WarningABS";
CANSIGNAL(WarningABS, bool, 24, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< SystemESCOff.
 */
const VehicleProperty::Property SystemESCOff = "SystemESCOff";
CANSIGNAL(SystemESCOff, bool, 23, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< SystemESC.
 */
const VehicleProperty::Property SystemESC = "SystemESC";
CANSIGNAL(SystemESC, bool, 22, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< LowWasher.
 */
const VehicleProperty::Property LowWasher = "LowWasher";
CANSIGNAL(LowWasher, bool, 21, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< WarningTPM.
 */
const VehicleProperty::Property WarningTPM = "WarningTPM";
CANSIGNAL(WarningTPM, bool, 20, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< FuelLeveltooLow.
 */
const VehicleProperty::Property FuelLeveltooLow = "FuelLeveltooLow";
CANSIGNAL(FuelLeveltooLow, bool, 18, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< EmergencyFlasher.
 */
const VehicleProperty::Property EmergencyFlasher = "EmergencyFlasher";
CANSIGNAL(EmergencyFlasher, bool, 17, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< CheckEnging.
 */
const VehicleProperty::Property CheckEnging = "CheckEnging";
CANSIGNAL(CheckEnging, bool, 16, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< RightTurnSignal.
 */
const VehicleProperty::Property RightTurnSignal = "RightTurnSignal";
CANSIGNAL(RightTurnSignal, bool, 9, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< LeftTurnSignal.
 */
const VehicleProperty::Property LeftTurnSignal = "LeftTurnSignal";
CANSIGNAL(LeftTurnSignal, bool, 8, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< WarningEBD.
 */
const VehicleProperty::Property WarningEBD = "WarningEBD";
CANSIGNAL(WarningEBD, bool, 6, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< WarningBrake.
 */
const VehicleProperty::Property WarningBrake = "WarningBrake";
CANSIGNAL(WarningBrake, bool, 5, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< CheckPowerSteering.
 */
const VehicleProperty::Property CheckPowerSteering = "CheckPowerSteering";
CANSIGNAL(CheckPowerSteering, bool, 4, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< WarningLowOilLevel.
 */
const VehicleProperty::Property WarningLowOilLevel = "WarningLowOilLevel";
CANSIGNAL(WarningLowOilLevel, bool, 3, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< CheckCharging.
 */
const VehicleProperty::Property CheckCharging = "CheckCharging";
CANSIGNAL(CheckCharging, bool, 2, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< WarningAirbag.
 */
const VehicleProperty::Property WarningAirbag = "WarningAirbag";
CANSIGNAL(WarningAirbag, bool, 1, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< WarningSafetybelts.
 */
const VehicleProperty::Property WarningSafetybelts = "WarningSafetybelts";
CANSIGNAL(WarningSafetybelts, bool, 0, 1, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<bool>(0), static_cast<bool>(1), nullptr, nullptr)

/**< BatteryChargeLevel.
 */
const VehicleProperty::Property BatteryChargeLevel = "BatteryChargeLevel";
CANSIGNAL(BatteryChargeLevel, char, 24, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(0), nullptr, nullptr)

/**< BatteryCurrent.
 */
const VehicleProperty::Property BatteryCurrent = "BatteryCurrent";
CANSIGNAL(BatteryCurrent, char, 16, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(0), nullptr, nullptr)

/**< BatteryVoltage.
 */
const VehicleProperty::Property BatteryVoltage = "BatteryVoltage";
CANSIGNAL(BatteryVoltage, double, 8, 8, Endian::Intel, Signedness::Unsigned, 0.1, 0, static_cast<double>(90), static_cast<double>(220), nullptr, nullptr)

/**< LampAutomaticHold.
 */
const VehicleProperty::Property LampAutomaticHold = "LampAutomaticHold";
CANSIGNAL(LampAutomaticHold, char, 0, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(255), nullptr, nullptr)

/**< TPMS_RR.
 */
const VehicleProperty::Property TPMS_RR = "TPMS_RR";
CANSIGNAL(TPMS_RR, char, 24, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(60), nullptr, nullptr)

/**< TPMS_RL.
 */
const VehicleProperty::Property TPMS_RL = "TPMS_RL";
CANSIGNAL(TPMS_RL, char, 16, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(60), nullptr, nullptr)

/**< TPMS_FR.
 */
const VehicleProperty::Property TPMS_FR = "TPMS_FR";
CANSIGNAL(TPMS_FR, char, 8, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(60), nullptr, nullptr)

/**< TPMS_FL.
 */
const VehicleProperty::Property TPMS_FL = "TPMS_FL";
CANSIGNAL(TPMS_FL, char, 0, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(60), nullptr, nullptr)

/**< FuelGage.
 */
const VehicleProperty::Property FuelGage = "FuelGage";
CANSIGNAL(FuelGage, char, 56, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(100), nullptr, nullptr)

/**< WarterTemperature.
 */
const VehicleProperty::Property WarterTemperature = "WarterTemperature";
CANSIGNAL(WarterTemperature, uint16_t, 16, 16, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<uint16_t>(0), static_cast<uint16_t>(100), nullptr, nullptr)

/**< EngineRPM.
 */
const VehicleProperty::Property EngineRPM = "EngineRPM";
CANSIGNAL(EngineRPM, uint16_t, 0, 16, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<uint16_t>(0), static_cast<uint16_t>(8000), nullptr, nullptr)

/**< VehicleOdometer.
 */
const VehicleProperty::Property VehicleOdometer = "VehicleOdometer";
CANSIGNAL(VehicleOdometer, uint32_t, 40, 24, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<uint32_t>(0), static_cast<uint32_t>(999000), nullptr, nullptr)

/**< VehicleSpeed.
 */
const VehicleProperty::Property VehicleSpeed = "VehicleSpeed";
CANSIGNAL(VehicleSpeed, uint16_t, 0, 16, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<uint16_t>(0), static_cast<uint16_t>(512), nullptr, nullptr)

/**< AliveCounter.
 */
const VehicleProperty::Property AliveCounter = "AliveCounter";
CANSIGNAL(AliveCounter, char, 56, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(1), static_cast<char>(9), nullptr, nullptr)

/**< DriveMode.
 */
const VehicleProperty::Property DriveMode = "DriveMode";
CANSIGNAL(DriveMode, char, 16, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(255), nullptr, nullptr)

/**< GearboxPosition.
 */
const VehicleProperty::Property GearboxPosition = "GearboxPosition";
CANSIGNAL(GearboxPosition, char, 8, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(8), nullptr, nullptr)

/**< GearboxPositionDisplay.
 */
const VehicleProperty::Property GearboxPositionDisplay = "GearboxPositionDisplay";
CANSIGNAL(GearboxPositionDisplay, char, 0, 8, Endian::Intel, Signedness::Unsigned, 1, 0, static_cast<char>(0), static_cast<char>(15), nullptr, nullptr)


#endif /* SAMSUNGCAN_CANSIGNALS_H_ */
