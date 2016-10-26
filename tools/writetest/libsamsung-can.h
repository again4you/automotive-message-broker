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

CAN_OBJECT(CheckSeatHeaterL,    guchar, NULL);
CAN_OBJECT(CheckSeatHeaterR,    guchar, NULL);
CAN_OBJECT(CheckSeatCoolerL,    guchar, NULL);
CAN_OBJECT(CheckSeatCoolerR,    guchar, NULL);

CAN_OBJECT_WRITE(CheckSeatHeaterL, guchar, NULL);

CAN_OBJECT_WRITE(VehicleSpeed, guint16, Speed);

#ifdef __cplusplus
}
#endif

#endif /* __LIBSAMSUNG_CAN_H__ */
