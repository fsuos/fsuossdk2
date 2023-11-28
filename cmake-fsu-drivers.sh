#!/bin/bash
BuildDriver () {
    if [ ! -d $1 ]; then
	mkdir $1
    fi
    cd $1
    if [ ! -d $3 ];then
      mkdir $3
    fi
    cd $3
    cmake -G Ninja ../../Drivers/$3 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../../$2
    ninja
    cd ..
}
BuildDriver "cmake-build-Arm-A7-Debug" "gcc/arm-303X-toolchain.cmake" $1
#BuildDriver "cmake-build-Arm-NUC-Debug" "arm-nuc-toolchain.cmake" "SMD303MINI"
#BuildDriver "cmake-build-Arm-A8-Debug" "arm-gfsu-toolchain.cmake" "GFSU"
#BuildDriver "cmake-build-Arm-ZNV-Debug" "arm-znv-toolchain.cmake" "ZNVIG2100"
#BuildDriver "cmake-build-Arm-IG2000-Debug" "arm-ig2000-toolchain.cmake" "ZNVIG2000"
