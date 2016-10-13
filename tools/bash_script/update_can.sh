#!/bin/sh

CANSEND=/usr/bin/cansend
INTERFACE="vcan0"

OBJNAME="$1"
VALUE=$2

if [ "$OBJNAME" = "VehicleSpeed" ]; then
	CANID="102"
	
	echo ${CANSEND} ${INTERFACE} ${CANID}#"01.00.00.00.00.00.00.00"
	#${CANSEND} ${INTERFACE} ${CANID}#"01.00.00.00.00.00.00.00"
fi
${CANSEND} ${INTERFACE} ${CANID}#"AABB00.00.00.00.00.00"
