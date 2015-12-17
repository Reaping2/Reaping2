#!/bin/sh
export REAPING_ROOT=`pwd`/../
cd ../deps/glfw-3.0.3
rm -rf build
mkdir build
cd build
cmake ..
make
cd ../../zlib-1.2.8/
rm -rf build
mkdir build
cd build
cmake ..
make
cp zconf.h ..
cd ../../lpng166
rm -rf build
mkdir build
cd build
export ZLIB_INCLUDE_DIR=`pwd`/../../zlib-1.2.8/
export ZLIB_LIBRARY=$ZLIB_INCLUDE_DIR/build/libz.so
cmake -DZLIB_LIBRARY:FILEPATH=$ZLIB_LIBRARY -DZLIB_INCLUDE_DIR:PATH=$ZLIB_INCLUDE_DIR ..
make
cd ../../libogg-1.3.1
./configure --prefix=`pwd`/build/
make
make install
cd ../libvorbis-1.3.3/
./configure --prefix=`pwd`/bin/ --with-ogg=`pwd`/../libogg-1.3.1/build/
make
make install
cd ../portaudio_v19
rm -rf build_dir
mkdir build_dir
./configure --prefix=`pwd`/build_dir/
make
make install
echo "" > build_dir/include/pa_options_cmake.h
cd $REAPING_ROOT/deps/glew-1.10.0/
make
mkdir $REAPING_ROOT/bin
mkdir $REAPING_ROOT/bin/libs
find $REAPING_ROOT/deps -name lib*.so* -exec cp "{}" $REAPING_ROOT/bin/libs/ \;

