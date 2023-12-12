#!/bin/bash
if [ $# != 1 ];then
  echo "Please give the config file name"
  exit
fi
j2 --customize customization.py template/DITemplate_interface.h Config/$1.yml -o Projects/$1_interface.h
j2 --customize customization.py template/DITemplate.h Config/$1.yml -o Projects/$1.h
j2 --customize customization.py template/DITemplate.cpp Config/$1.yml -o Projects/$1.cpp
if [ ! -e Drivers/DI$1 ];then
mkdir Drivers/DI$1
fi
j2 --customize customization.py template/di.project Config/$1.yml -o Drivers/DI$1/DI$1.project
j2 --customize customization-cmake.py template/DICMakeLists.txt Config/$1.yml -o Drivers/DI$1/CMakeLists.txt
j2 --customize customization.py template/dihelper.php Config/$1.yml -o DeviceWeb/application/helpers/device/${1,,}.php
j2 --customize customization.py template/diview.php Config/$1.yml -o DeviceWeb/application/views/portal/DevicePage/${1,,}.php
