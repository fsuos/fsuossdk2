#!/bin/sh
if [ $# != 1 ];then
  echo "Please give the config file name"
  exit
fi
j2 --customize customization.py template/BacnetTemplate_interface.h Config/$1.yml -o SMDSocketProcessor/$1_interface.h
j2 --customize customization.py template/BacnetTemplate.h Config/$1.yml -o SMDSocketProcessor/$1.h
j2 --customize customization.py template/BacnetTemplate.cpp Config/$1.yml -o SMDSocketProcessor/$1.cpp
if [ ! -e Drivers/Socket$1 ];then
mkdir Drivers/Socket$1
fi
j2 --customize customization.py template/socket.project Config/$1.yml -o Drivers/Socket$1/Socket$1.project
j2 --customize customization-cmake.py template/SocketCMakeLists.txt Config/$1.yml -o Drivers/Socket$1/CMakeLists.txt
LDriver=`echo $1|awk '{ print tolower($1) }'`
#j2 --customize customization.py template/pymodbus.py Config/$1.yml -o PyDrivers/Py$1.py 
j2 --customize customization.py template/bacnethelper.php Config/$1.yml -o DeviceWeb/application/helpers/device/$LDriver.php
j2 --customize customization.py template/bacnetview.php Config/$1.yml -o DeviceWeb/application/views/portal/DevicePage/$LDriver.php
j2 --customize customization.py template/rt_data-template.js  Config/$1.yml -o public/portal/js/rt-data/rt_data-$LDriver.js
# Generate AI
j2 --customize customization.py template/unicom_yaml.template Config/$1.yml -o DeviceWeb/application/helpers/ini/unicom/$LDriver.yaml
#export vendor=Telecom && j2 --customize customization.py template/yaml.template Config/$1.yml -o DeviceWeb/application/helpers/ini/telecom/$LDriver.yaml
