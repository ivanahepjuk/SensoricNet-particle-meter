#!/bin/bash

CSV_FILE="../../sensoricnet_lora_ids.csv"

DEV_ID="$1"
export IFS=","
tail -n +2 $CSV_FILE | while read devid devtype deveui appeui appkey gps_lat gps_long gps_alt; do
  if [ "$devid" == "$1" ]; then
    echo "dev_id: $devid"
    echo "dev_type: $devtype"
    echo "dev_eui: $deveui"
    echo "app_eui: $appeui"
    echo "app_key: $appkey"
    echo "gps_lat: $gps_lat"
    echo "gps_long: $gps_long"
    echo "gps_alt: $gps_alt"
    make clean ; make flash CFLAGS="-DDEV_ID=\\\"${devid}\\\" -DDEVICE_TYPE=${devtype} -DLORAWAN_DEVEUI=\\\"${deveui}\\\" -DLORAWAN_APPEUI=\\\"${appeui}\\\" -DLORAWAN_APPKEY=\\\"${appkey}\\\" -DGPS_LAT=\\\"${gps_lat}\\\" -DGPS_LONG=\\\"${gps_long}\\\" -DGPS_ALT=\\\"${gps_alt}\\\""
  fi
done

