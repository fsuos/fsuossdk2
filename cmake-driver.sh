BUILD_TYPE="Debug"
BUILD_DIR="cmake-build-"$BUILD_TYPE
DRIVER_DIR="Drivers"
if [ ! -d $BUILD_DIR ]; then
    mkdir $BUILD_DIR
fi
if [ ! -d $BUILD_DIR/output ]; then
    mkdir $BUILD_DIR/output
fi
dir=$1
if [ -d Drivers/$dir ] && [ ! -d $BUILD_DIR/$dir ]; 
then
  mkdir $BUILD_DIR/$dir
fi
cd $BUILD_DIR/$dir 
cmake -G Ninja ../../$DRIVER_DIR/$dir  -DCMAKE_BUILD_TYPE=$BUILD_TYPE
#make -j 8 $@
ninja
cd ../..
