#!/bin/bash
if [ $# != 1 ];then
  echo "Please give the config file name"
  exit
fi
j2 --customize customization.py template/AITemplate_interface.h Config/$1.yml -o Projects/$1_interface.h
j2 --customize customization.py template/AITemplate.h Config/$1.yml -o Projects/$1.h
j2 --customize customization.py template/AITemplate.cpp Config/$1.yml -o Projects/$1.cpp
if [ ! -e Drivers/SP$1 ];then
mkdir Drivers/SP$1
fi
j2 --customize customization.py template/AI.project Config/$1.yml -o Drivers/SP$1/SP$1.project
j2 --customize customization.py template/AICMakeLists.txt Config/$1.yml -o Drivers/SP$1/CMakeLists.txt
j2 --customize customization.py template/aihelper.php Config/$1.yml -o DeviceWeb/application/helpers/device/${1,,}.php
j2 --customize customization.py template/aiview.php Config/$1.yml -o DeviceWeb/application/views/portal/DevicePage/${1,,}.php
