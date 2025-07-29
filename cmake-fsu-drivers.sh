#!/bin/sh
PACK_PATH="/home/marship/Projects/xjdh/DeviceWeb/"
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
    DSTFILE=`grep OUTPUT_NAME ../../Drivers/$3/CMakeLists.txt|sed -n 's/^.*OUTPUT_NAME \"\([a-z0-9_]*\)\".*/\1/p'`
    strip ../output/$DSTFILE.so
    if [[ $3 =~ ^"DI" ]];then
      cp ../output/$DSTFILE.so $PACK_PATH$4"/public/DIPlugins"
    elif [[ $3 =~ ^"AI" ]];then
      cp ../output/$DSTFILE.so $PACK_PATH$4"/public/AIPlugins"
    elif [[ $3 =~ ^"SP" ]];then
      cp ../output/$DSTFILE.so $PACK_PATH$4"/public/SPPlugins"
    elif [[ $3 =~ ^"Socket" ]];then
      cp ../output/$DSTFILE.so $PACK_PATH$4"/public/SocketPlugins"
    fi
    cd ../..
    cp DeviceWeb/$DSTFILE.config $PACK_PATH$4/
    cp DeviceWeb/application/helpers/device/$DSTFILE.php $PACK_PATH$4/application/helpers/device/
    cp public/portal/js/rt-data/rt-data-$DSTFILE.js $PACK_PATH$4/public/portal/js/rt-data/
    cp DeviceWeb/application/helpers/ini/telecom/$DSTFILE.yaml $PACK_PATH$4/application/helpers/ini/telecom/
    cp DeviceWeb/application/helpers/ini/telecom/$DSTFILE.ini $PACK_PATH$4/application/helpers/ini/telecom/
    cp DeviceWeb/application/helpers/ini/unicom/$DSTFILE.yaml $PACK_PATH$4/application/helpers/ini/unicom/
    cp DeviceWeb/application/helpers/ini/unicom/$DSTFILE.ini $PACK_PATH$4/application/helpers/ini/unicom/
    cp DeviceWeb/application/views/portal/DevicePage/$DSTFILE.php $PACK_PATH$4/application/views/portal/DevicePage/
    if [[ $DSTFILE =~ "419"$ ]];then
      cd $PACK_PATH$4;./pack_419.sh $DSTFILE;cd -
    else
      cd $PACK_PATH$4;./pack.sh $DSTFILE;cd -
    fi
    cd $PACK_PATH$4;./sync.sh;cd -
}
BuildDriver "cmake-build-Arm-A7-Debug" "gcc/arm-303X-toolchain.cmake" $1 "SMD303X"
BuildDriver "cmake-build-Arm-Loncomip-Debug" "gcc/arm-loncomip-toolchain.cmake" $1 "Loncomip"
BuildDriver "cmake-build-Arm-NUC-Debug" "gcc/arm-303MINI-toolchain.cmake" $1 "SMD303MINI"
BuildDriver "cmake-build-Arm-A8-Debug" "gcc/arm-GFSU-toolchain.cmake" $1  "GFSU"
BuildDriver "cmake-build-Arm-ZNV-Debug" "gcc/arm-ZNV-IG2100-toolchain.cmake" $1 "ZNVIG2100"
BuildDriver "cmake-build-Arm-IG2000-Debug" "gcc/arm-ig2000-toolchain.cmake" $1 "ZNVIG2000"
BuildDriver "cmake-build-Arm-EISU-Debug" "gcc/arm-eisu-toolchain.cmake" $1 "ZNVEISU"
BuildDriver "cmake-build-MIPS-DGM-Debug" "gcc/mips-dgm-toolchain.cmake" $1 "DGM"

