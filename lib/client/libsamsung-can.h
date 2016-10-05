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

enum GearboxPositionDisplay{
	PARKING = (1<<3) | (1<<1), // 10, 0x0A
	DRIVE,			   // 11, 0x0B
	NEUTRAL,
	REVERSE,
};

CAN_OBJECT(VehicleOdometer, guint32, NULL);
CAN_OBJECT(VehicleSpeed, guint16, Speed);
CAN_OBJECT(GearboxPositionDisplay, guchar, NULL);

#ifdef __cplusplus
}
#endif

#endif /* __LIBSAMSUNG_CAN_H__ */
