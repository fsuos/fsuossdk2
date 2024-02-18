SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 4.1.15)
SET(CMAKE_SYSTEM_PROCESSOR ArmA8)

#SET(CMAKE_C_COMPILER   /data/Vertic/bin/arm-gfsu-linux-gnueabihf-gcc)
#SET(CMAKE_CXX_COMPILER /data/Vertic/bin/arm-gfsu-linux-gnueabihf-g++)

SET(CMAKE_C_COMPILER   ${CMAKE_CURRENT_LIST_DIR}/arm-gfsu310-linux-gnueabihf/bin/arm-gfsu310-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER ${CMAKE_CURRENT_LIST_DIR}/arm-gfsu310-linux-gnueabihf/bin/arm-gfsu310-linux-gnueabihf-g++)


# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH  /home/marship/rootfs)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
