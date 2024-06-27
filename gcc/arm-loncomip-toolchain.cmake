SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 3.14.0)
SET(CMAKE_SYSTEM_PROCESSOR ArmLoncomip)

SET(CMAKE_C_COMPILER   /data2/loncomip/host/bin/arm-linux-gnueabihf-gcc --sysroot /data2/loncomip/host/arm-buildroot-linux-gnueabihf/sysroot)
SET(CMAKE_CXX_COMPILER /data2/loncomip/host/bin/arm-linux-gnueabihf-g++ --sysroot /data2/loncomip/host/arm-buildroot-linux-gnueabihf/sysroot)

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH  /home/marship/rootfs)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
