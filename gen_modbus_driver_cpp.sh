#!/bin/sh
if [ $# != 1 ];then
  echo "Please give the config file name"
  exit
fi
j2 --customize customization.py template/ModbusTemplate_interface.h Config/$1.yml -o SMDSPProcessor/$1_interface.h
j2 --customize customization.py template/ModbusTemplate.h Config/$1.yml -o SMDSPProcessor/$1.h
j2 --customize customization.py template/ModbusTemplate.cpp Config/$1.yml -o SMDSPProcessor/$1.cpp
