SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 2.6.39)
SET(CMAKE_SYSTEM_PROCESSOR ArmIG2000)

SET(CMAKE_C_COMPILER   ${CMAKE_CURRENT_LIST_DIR}/gcc/ZNV-IG2000/bin/arm-jimglobal-linux-gnueabi-gcc)
SET(CMAKE_CXX_COMPILER ${CMAKE_CURRENT_LIST_DIR}/gcc/ZNV-IG2000/bin/arm-jimglobal-linux-gnueabi-g++)

# where is the target environment 

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
