#!/bin/sh

CANSEND=/usr/bin/cansend
INTERFACE="vcan0"

OBJNAME="$1"
VALUE="$2"
#HEXVALUE=$(printf "%x" $2)

if [ "$OBJNAME" = "VehicleSpeed" ]; then
	CANID="102"
	CANMSG=${VALUE}".00.00.00.00.00.00"
elif [ "$OBJNAME" = "VehicleOdometer" ]; then
	CANID="102"
	CANMSG="00.00.00.00.00."${VALUE}
else
	echo "${OBJNAME} is not support in this script!"
fi
echo ${CANSEND} ${INTERFACE} ${CANID}#${CANMSG}
${CANSEND} ${INTERFACE} ${CANID}#${CANMSG}

