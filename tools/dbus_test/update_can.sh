#!/bin/bash

CANSEND=/usr/bin/cansend
#INTERFACE="samsungcan0"
INTERFACE="vcan0"

OBJNAME="$1"
VALUE=$2

if [ "$#" -ne 2 ]; then
	echo "Usage: $0 [Object Name] [Value]"
	echo "Usage: $0 Wheel [Event_Name]"
	echo "Usage: $0 Vehicle [Event_Name]"
    echo "Supported Object List:"
    echo "  VehicleSpeed VehicleOdometer EngineRPM FuelGage WarterTemperature"
    echo "  TPMS_FL TPMS_FR TPMS_RL TPMS_RR"
    echo "  FR_KeyEvent01 FR_KeyEvent02 FR_KeyEvent03 FR_KeyEvent04 FR_KeyEvent05 FR_KeyEvent06 FR_KeyEvent07 FR_KeyEvent08"
    echo "  CheckSeatHeaterL CheckSeatHeaterR CheckSeatCoolerL CheckSeatCoolerR"
    echo "  AirDistributionCID LeftTemperatureCID RightTemperatureCID LeftAirflowCID RightAirflowCID"
    echo "  AirDistributionLeftKnob LeftTemperatureLeftKnob LeftAirflowLeftKnob MediaVolumeLeftKnob"
    echo "  AirDistributionRightKnob RightTemperatureRightKnob RightAirflowLeftKnob MediaVolumeRightKnob"
    echo "Supported Wheel Event"
    echo "  VoiceCommand MediaChange Mute (Toggle)"
    echo "  VolumnUp VolumnDown MediaUp MediaDown"
    echo "  NaviFull BarUp BarDown"
    echo "Supported Vehicle Event"
    echo "  AllWarning LowOilLevel AirBagPowerSteering WarningClear"
    echo "  TPMSOK TPMSError"
	exit
fi

if [ "$OBJNAME" = "Vehicle" ]; then
	if [ "$VALUE" = "AllWarning" ]; then
		${CANSEND} ${INTERFACE} "206#FF.FF.FF.FF.FF.FF.FF.FF"
	elif [ "$VALUE" = "LowOilLevel" ]; then
		${CANSEND} ${INTERFACE} "206#02.00.00.00.00.00.00.00"
	elif [ "$VALUE" = "AirBagPowerSteering" ]; then
		${CANSEND} ${INTERFACE} "206#12.00.00.00.00.00.00.00"
	elif [ "$VALUE" = "WarningClear" ]; then
		${CANSEND} ${INTERFACE} "206#00.00.00.00.00.00.00.00"
	elif [ "$VALUE" = "TPMSOK" ]; then
		${CANSEND} ${INTERFACE} "104#3C.3B.3C.3C.00.00.00.00"
	elif [ "$VALUE" = "TPMSError" ]; then
		${CANSEND} ${INTERFACE} "104#3C.10.3C.3C.00.00.00.00"
	fi
	exit
fi

if [ "$OBJNAME" = "Wheel" ]; then
	CANID="207"
	if [ "$VALUE" = "VoiceCommand" ]; then
		${CANSEND} ${INTERFACE} ${CANID}"#01.00.00.00.00.00.00.00"
	elif [ "$VALUE" = "MediaChange" ]; then
		${CANSEND} ${INTERFACE} ${CANID}"#02.00.00.00.00.00.00.00"
	elif [ "$VALUE" = "Mute" ]; then
		${CANSEND} ${INTERFACE} ${CANID}"#04.00.00.00.00.00.00.00"
	elif [ "$VALUE" = "VolumnUp" ]; then
		${CANSEND} ${INTERFACE} ${CANID}"#08.00.00.00.00.00.00.00"
	elif [ "$VALUE" = "VolumnDown" ]; then
		${CANSEND} ${INTERFACE} ${CANID}"#10.00.00.00.00.00.00.00"
	elif [ "$VALUE" = "MediaUp" ]; then
		${CANSEND} ${INTERFACE} ${CANID}"#20.00.00.00.00.00.00.00"
	elif [ "$VALUE" = "MediaDown" ]; then
		${CANSEND} ${INTERFACE} ${CANID}"#40.00.00.00.00.00.00.00"
	elif [ "$VALUE" = "NaviFull" ]; then
		${CANSEND} ${INTERFACE} ${CANID}"#00.20.00.00.00.00.00.00"
	elif [ "$VALUE" = "BarUp" ]; then
		${CANSEND} ${INTERFACE} ${CANID}"#00.40.00.00.00.00.00.00"
	elif [ "$VALUE" = "BarDown" ]; then
		${CANSEND} ${INTERFACE} ${CANID}"#00.80.00.00.00.00.00.00"
	fi
	sleep 1
	${CANSEND} ${INTERFACE} ${CANID}"#00.00.00.00.00.00.00.00"
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
    VALUE=$(echo ${VALUE} | awk '{print $1 * 10 - 170;}' )
	HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG="00.00.00."${HEXVALUE:0:2}".00.00.00.00"
elif [ "$OBJNAME" = "RightTemperatureCID" ]; then
    CANID="402"
    VALUE=$(echo ${VALUE} | awk '{print $1 * 10 - 170;}' )
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
elif [ "$OBJNAME" = "MediaVolumeLeftKnob" ]; then
    CANID="701"
	HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG="00.00.00.00.00.00.00."${HEXVALUE:0:2}
elif [ "$OBJNAME" = "LeftTemperatureLeftKnob" ]; then
    CANID="701"
    VALUE=$(echo ${VALUE} | awk '{print $1 * 10 - 170;}' )
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
    VALUE=$(echo ${VALUE} | awk '{print $1 * 10 - 170;}' )
	HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG="00.00.00.00.00."${HEXVALUE:0:2}".00.00"
elif [ "$OBJNAME" = "MediaVolumeRightKnob" ]; then
    CANID="702"
	HEXVALUE=$(printf "%02X" ${VALUE})
    CANMSG="00.00.00.00.00.00.00."${HEXVALUE:0:2}
else
	echo "${OBJNAME} is not support in this script!"
fi

echo "Send CAN Message: " ${CANSEND} ${INTERFACE} ${CANID}#${CANMSG}
${CANSEND} ${INTERFACE} ${CANID}#${CANMSG}

