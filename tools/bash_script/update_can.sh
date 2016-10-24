#!/bin/bash

CANSEND=/usr/bin/cansend
INTERFACE="vcan0"

OBJNAME="$1"
VALUE=$2

if [ "$OBJNAME" = "VehicleSpeed" ]; then
	CANID="102"
	HEXVALUE=$(printf "%04X" ${VALUE})
	CANMSG=${HEXVALUE:2:2}"."${HEXVALUE:0:2}".00.00.00.00.00.00"
elif [ "$OBJNAME" = "VehicleOdometer" ]; then
	CANID="102"
	CANMSG="00.00.00.00.00."${VALUE}
elif [ "$OBJNAME" = "EngineRPM" ]; then
	CANID="103"
	HEXVALUE=$(printf "%04X" ${VALUE})
	CANMSG=${HEXVALUE:2:2}"."${HEXVALUE:0:2}".00.00.00.00.00.00"
elif [ "$OBJNAME" = "FuelGage" ]; then
	CANID="103"
	HEXVALUE=$(printf "%04X" ${VALUE})
	CANMSG="00.00."${HEXVALUE:2:2}"."${HEXVALUE:0:2}".00.00.00.00"
elif [ "$OBJNAME" = "FuelGage" ]; then
	CANID="104"
	CANMSG="00.00.00.00.00.00.00."${VALUE}
elif [ "$OBJNAME" = "TPMS_FL" ]; then
	CANID="104"
	HEXVALUE=$(printf "%02X" ${VALUE})
	CANMSG=${HEXVALUE:0:2}".00.00.00.00.00.00.00"
elif [ "$OBJNAME" = "TPMS_FR" ]; then
	CANID="104"
	HEXVALUE=$(printf "%02X" ${VALUE})
	CANMSG="00."${HEXVALUE:0:2}".00.00.00.00.00.00"
elif [ "$OBJNAME" = "TPMS_RL" ]; then
	CANID="104"
	HEXVALUE=$(printf "%02X" ${VALUE})
	CANMSG="00.00."${HEXVALUE:0:2}".00.00.00.00.00"
elif [ "$OBJNAME" = "TPMS_RR" ]; then
	CANID="104"
	HEXVALUE=$(printf "%02X" ${VALUE})
	CANMSG="00.00.00."${HEXVALUE:0:2}".00.00.00.00"
else
	echo "${OBJNAME} is not support in this script!"
fi

echo "Send CAN Message: " ${CANSEND} ${INTERFACE} ${CANID}#${CANMSG}
${CANSEND} ${INTERFACE} ${CANID}#${CANMSG}

