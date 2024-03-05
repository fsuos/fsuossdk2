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
    cp DeviceWeb/application/helpers/device/$DSTFILE.php $PACK_PATH$4/application/helpers/device/
    cp DeviceWeb/application/views/portal/DevicePage/$DSTFILE.php $PACK_PATH$4/application/views/portal/DevicePage/
    if [[ $DSTFILE =~ "419"$ ]];then
      cd $PACK_PATH$4;./pack_419.sh $DSTFILE;cd -
    else
      cd $PACK_PATH$4;./pack.sh $DSTFILE;cd -
    fi
}
BuildDriver "cmake-build-Arm-A7-Debug" "gcc/arm-303X-toolchain.cmake" $1 "SMD303X"
#BuildDriver "cmake-build-Arm-NUC-Debug" "gcc/arm-NUC-toolchain.cmake" $1 "SMD303MINI"
#BuildDriver "cmake-build-Arm-A8-Debug" "gcc/arm-GFSU-toolchain.cmake" $1  "GFSU"
#BuildDriver "cmake-build-Arm-ZNV-Debug" "gcc/arm-ZNV-IG2100-toolchain.cmake" $1 "ZNVIG2100"
#BuildDriver "cmake-build-Arm-IG2000-Debug" "arm-ig2000-toolchain.cmake" "ZNVIG2000"
