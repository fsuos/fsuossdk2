#!/bin/bash
if [ $# != 1 ];then
  echo "Please give the config file name"
  exit
fi
j2 --customize customization.py template/PMBusTemplate_interface.h Config/$1.yml -o SMDSPProcessor/$1_interface.h
j2 --customize customization.py template/PMBusTemplate.h Config/$1.yml -o SMDSPProcessor/$1.h
j2 --customize customization.py template/PMBusTemplate.cpp Config/$1.yml -o SMDSPProcessor/$1.cpp
if [ ! -e Drivers/SP$1 ];then
mkdir Drivers/SP$1
fi
j2 --customize customization.py template/sp.project Config/$1.yml -o Drivers/SP$1/SP$1.project
j2 --customize customization-cmake.py template/SPCMakeLists.txt Config/$1.yml -o Drivers/SP$1/CMakeLists.txt
j2 --customize customization.py template/pmbus_helper.php Config/$1.yml -o DeviceWeb/application/helpers/device/${1,,}.php
j2 --customize customization.py template/view.php Config/$1.yml -o DeviceWeb/application/views/portal/DevicePage/${1,,}.php
