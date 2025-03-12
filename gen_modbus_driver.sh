#!/bin/sh
if [ $# != 1 ];then
  echo "Please give the config file name"
  exit
fi
j2 --customize customization.py template/ModbusTemplate_interface.h Config/$1.yml -o SMDSPProcessor/$1_interface.h
j2 --customize customization.py template/ModbusTemplate.h Config/$1.yml -o SMDSPProcessor/$1.h
j2 --customize customization.py template/ModbusTemplate.cpp Config/$1.yml -o SMDSPProcessor/$1.cpp
if [ ! -e Drivers/SP$1 ];then
mkdir Drivers/SP$1
fi
j2 --customize customization.py template/sp.project Config/$1.yml -o Drivers/SP$1/SP$1.project
j2 --customize customization-cmake.py template/SPCMakeLists.txt Config/$1.yml -o Drivers/SP$1/CMakeLists.txt
LDriver=`echo $1|awk '{ print tolower($1) }'`
#j2 --customize customization.py template/pymodbus.py Config/$1.yml -o PyDrivers/Py$1.py 
j2 --customize customization.py template/helper.php Config/$1.yml -o DeviceWeb/application/helpers/device/$LDriver.php
j2 --customize customization.py template/view.php Config/$1.yml -o DeviceWeb/application/views/portal/DevicePage/$LDriver.php
j2 --customize customization.py template/rt_data-template.js  Config/$1.yml -o public/portal/js/rt-data/rt_data-$LDriver.js
# Generate AI
#export vendor=Unicom && j2 --customize customization.py template/yaml.template Config/$1.yml -o DeviceWeb/application/helpers/ini/unicom/$LDriver.yaml
#export vendor=Telecom && j2 --customize customization.py template/yaml.template Config/$1.yml -o DeviceWeb/application/helpers/ini/telecom/$LDriver.yaml
