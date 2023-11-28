SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 4.1.15)
SET(CMAKE_SYSTEM_PROCESSOR ArmA7)
SET(CMAKE_C_COMPILER   ${CMAKE_CURRENT_LIST_DIR}/arm-jgcx41-linux/bin/arm-jgcx41-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER ${CMAKE_CURRENT_LIST_DIR}/arm-jgcx41-linux/bin/arm-jgcx41-linux-gnueabihf-g++)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
