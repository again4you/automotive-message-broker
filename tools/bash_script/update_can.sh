#!/bin/bash

CANSEND=/usr/bin/cansend
INTERFACE="vcan0"

OBJNAME="$1"
VALUE=$2

if [ "$#" -ne 2 ]; then
	echo "Usage: $0 [Object Name] [Value]"
    echo "Supported Object List:"
    echo "  VehicleSpeed VehicleOdometer EngineRPM FuelGage WarterTemperature"
    echo "  TPMS_FL TPMS_FR TPMS_RL TPMS_RR"
    echo "  FR_KeyEvent01 FR_KeyEvent02 FR_KeyEvent03 FR_KeyEvent04 FR_KeyEvent05 FR_KeyEvent06 FR_KeyEvent07 FR_KeyEvent08"
    echo "  CheckSeatHeaterL CheckSeatHeaterR CheckSeatCoolerL CheckSeatCoolerR"
    echo "  AirDistributionCID LeftTemperatureCID RightTemperatureCID LeftAirflowCID RightAirflowCID"
    echo "  AirDistributionLeftKnob LeftTemperatureLeftKnob LeftAirflowLeftKnob"
    echo "  AirDistributionRightKnob RightTemperatureRightKnob RightAirflowLeftKnob"
	exit
fi

if [ "$OBJNAME" = "VehicleSpeed" ]; then
	CANID="102"
	HEXVALUE=$(printf "%04X" ${VALUE})
	CANMSG=${HEXVALUE:2:2}"."${HEXVALUE:0:2}".00.00.00.00.00.00"
elif [ "$OBJNAME" = "VehicleOdometer" ]; then
	CANID="102"
	HEXVALUE=$(printf "%06X" ${VALUE})
	CANMSG="00.00.00.00.00."${HEXVALUE:4:2}"."${HEXVALUE:2:2}"."${HEXVALUE:0:2}
elif [ "$OBJNAME" = "EngineRPM" ]; then
	CANID="103"
	HEXVALUE=$(printf "%04X" ${VALUE})
	CANMSG=${HEXVALUE:2:2}"."${HEXVALUE:0:2}".00.00.00.00.00.00"
elif [ "$OBJNAME" = "FuelGage" ]; then
	CANID="103"
	HEXVALUE=$(printf "%04X" ${VALUE})
	CANMSG="00.00."${HEXVALUE:2:2}"."${HEXVALUE:0:2}".00.00.00.00"
elif [ "$OBJNAME" = "WarterTemperature" ]; then
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
    CANMSG=${HEXVALUE:0:2}".00.00.00.00.00.00.00"
elif [ "$OBJNAME" = "FR_KeyEvent01" ]; then
    CANID="207"
    HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG=${HEXVALUE:0:2}".00.00.00.00.00.00.00"
elif [ "$OBJNAME" = "FR_KeyEvent02" ]; then
    CANID="207"
    VALUE=$((${VALUE} << 1))
    HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG=${HEXVALUE:0:2}".00.00.00.00.00.00.00"
elif [ "$OBJNAME" = "FR_KeyEvent03" ]; then
    CANID="207"
    VALUE=$((${VALUE} << 2))
    HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG=${HEXVALUE:0:2}".00.00.00.00.00.00.00"
elif [ "$OBJNAME" = "FR_KeyEvent04" ]; then
    CANID="207"
    VALUE=$((${VALUE} << 3))
    HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG=${HEXVALUE:0:2}".00.00.00.00.00.00.00"
elif [ "$OBJNAME" = "FR_KeyEvent05" ]; then
    CANID="207"
    VALUE=$((${VALUE} << 4))
    HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG=${HEXVALUE:0:2}".00.00.00.00.00.00.00"
elif [ "$OBJNAME" = "FR_KeyEvent06" ]; then
    CANID="207"
    VALUE=$((${VALUE} << 5))
    HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG=${HEXVALUE:0:2}".00.00.00.00.00.00.00"
elif [ "$OBJNAME" = "FR_KeyEvent07" ]; then
    CANID="207"
    VALUE=$((${VALUE} << 6))
    HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG=${HEXVALUE:0:2}".00.00.00.00.00.00.00"
elif [ "$OBJNAME" = "FR_KeyEvent08" ]; then
    CANID="207"
    VALUE=$((${VALUE} << 7))
    HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG=${HEXVALUE:0:2}".00.00.00.00.00.00.00"
elif [ "$OBJNAME" = "CheckSeatHeaterL" ]; then
    CANID="206"
    HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG="00.00.00.00.00.00."${HEXVALUE:0:2}".00"
elif [ "$OBJNAME" = "CheckSeatHeaterR" ]; then
    CANID="206"
    VALUE=$((${VALUE} << 2))
    HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG="00.00.00.00.00.00."${HEXVALUE:0:2}".00"
elif [ "$OBJNAME" = "CheckSeatCoolerL" ]; then
    CANID="206"
    VALUE=$((${VALUE} << 4))
    HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG="00.00.00.00.00.00."${HEXVALUE:0:2}".00"
elif [ "$OBJNAME" = "CheckSeatCoolerR" ]; then
    CANID="206"
    VALUE=$((${VALUE} << 6))
    HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG="00.00.00.00.00.00."${HEXVALUE:0:2}".00"
elif [ "$OBJNAME" = "AirDistributionCID" ]; then
    CANID="402"
	HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG="00.00."${HEXVALUE:0:2}".00.00.00.00.00"
elif [ "$OBJNAME" = "LeftTemperatureCID" ]; then
    CANID="402"
    VALUE=$(printf "%.f" $(bc <<< "${VALUE} * 10 - 170"))
	HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG="00.00.00."${HEXVALUE:0:2}".00.00.00.00"
elif [ "$OBJNAME" = "RightTemperatureCID" ]; then
    CANID="402"
    VALUE=$(printf "%.f" $(bc <<< "${VALUE} * 10 - 170"))
	HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG="00.00.00.00.00."${HEXVALUE:0:2}".00.00"
elif [ "$OBJNAME" = "LeftAirflowCID" ]; then
    CANID="402"
	HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG="00.00.00.00."${HEXVALUE:0:2}".00.00.00"
elif [ "$OBJNAME" = "RightAirflowCID" ]; then
    CANID="402"
	HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG="00.00.00.00.00.00"${HEXVALUE:0:2}".00"
elif [ "$OBJNAME" = "AirDistributionLeftKnob" ]; then
    CANID="701"
	HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG="00.00.00.00."${HEXVALUE:0:2}".00.00.00"
elif [ "$OBJNAME" = "LeftAirflowLeftKnob" ]; then
    CANID="701"
	HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG="00.00.00.00.00.00."${HEXVALUE:0:2}".00"
elif [ "$OBJNAME" = "LeftTemperatureLeftKnob" ]; then
    CANID="701"
    VALUE=$(printf "%.f" $(bc <<< "${VALUE} * 10 - 170"))
	HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG="00.00.00.00.00."${HEXVALUE:0:2}".00.00"
elif [ "$OBJNAME" = "AirDistributionRightKnob" ]; then
    CANID="702"
	HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG="00.00.00.00."${HEXVALUE:0:2}".00.00.00"
elif [ "$OBJNAME" = "RightAirflowLeftKnob" ]; then
    CANID="702"
	HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG="00.00.00.00.00.00."${HEXVALUE:0:2}".00"
elif [ "$OBJNAME" = "RightTemperatureRightKnob" ]; then
    CANID="702"
    VALUE=$(printf "%.f" $(bc <<< "${VALUE} * 10 - 170"))
	HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG="00.00.00.00.00."${HEXVALUE:0:2}".00.00"
else
	echo "${OBJNAME} is not support in this script!"
fi

echo "Send CAN Message: " ${CANSEND} ${INTERFACE} ${CANID}#${CANMSG}
${CANSEND} ${INTERFACE} ${CANID}#${CANMSG}

