SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 3.14.25)
SET(CMAKE_SYSTEM_PROCESSOR MipsDGM)

SET(CMAKE_C_COMPILER   /data2/dgm/buildroot-2020.08/output/host/bin/mipsel-openwrt-linux-gnu-gcc)
SET(CMAKE_CXX_COMPILER /data2/dgm/buildroot-2020.08/output/host/bin/mipsel-openwrt-linux-gnu-g++)

# where is the target environment 

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
