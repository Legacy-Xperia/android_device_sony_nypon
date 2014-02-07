#!/system/bin/sh

MAC_FILE=/data/etc/wlan_macaddr
COMMAND1="/system/bin/insmod"
COMMAND2="/system/bin/iw"
MOD_PATH="/system/lib/modules"
MOD_CORE="cw1200_core.ko"
MOD_WLAN="cw1200_wlan.ko"
ARG="macaddr="

if ( /system/bin/ls $MAC_FILE > /dev/null ); then
     ADDR=`/system/bin/cat $MAC_FILE`
     echo $COMMAND1 $MOD_PATH/$MOD_CORE $ARG$ADDR
     $COMMAND1 $MOD_PATH/$MOD_CORE $ARG$ADDR

else
     echo $COMMAND1 $MOD_PATH/$MOD_CORE
     $COMMAND1 $MOD_PATH/$MOD_CORE
fi

echo $COMMAND1 $MOD_PATH/$MOD_WLAN
$COMMAND1 $MOD_PATH/$MOD_WLAN
