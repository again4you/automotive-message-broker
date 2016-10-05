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
CAN_OBJECT(VehicleSpeed, guint16, "Speed");
// CAN_OBJECT(VehicleSpeed, guint16, Speed);

#ifdef __cplusplus
}
#endif

#endif /* __LIBSAMSUNG_CAN_H__ */
