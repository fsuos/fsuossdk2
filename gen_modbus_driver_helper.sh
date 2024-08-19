#!/bin/sh
if [ $# != 1 ];then
  echo "Please give the config file name"
  exit
fi
if [ ! -e Drivers/SP$1 ];then
mkdir Drivers/SP$1
fi
LDriver=`echo $1|awk '{ print tolower($1) }'`
j2 --customize customization.py template/helper.php Config/$1.yml -o DeviceWeb/application/helpers/device/$LDriver.php
# Generate AI
#export vendor=Unicom && j2 --customize customization.py template/yaml.template Config/$1.yml -o DeviceWeb/application/helpers/ini/unicom/$LDriver.yaml
#export vendor=Telecom && j2 --customize customization.py template/yaml.template Config/$1.yml -o DeviceWeb/application/helpers/ini/telecom/$LDriver.yaml
