#!/bin/bash

echo  
echo =======================================================================================
echo ======================== Ensure that Raspbian is all up-to-date =======================
echo =======================================================================================

sudo apt-get -y update
sudo apt-get -y upgrade
cd ~

echo  
echo =======================================================================================
echo ========================= Miscellaneous distribution installs =========================
echo =======================================================================================

sudo apt-get -y install g++
sudo apt-get -y install git
sudo apt-get -y install git-gui
sudo apt-get -y install geany
sudo apt-get -y install ftp
sudo apt-get -y install libboost1.62-all
sudo apt-get -y install libgtk2.0-dev
sudo apt-get -y install pkg-config
sudo apt-get -y install ntp
sudo apt-get -y install cmake


echo  
echo =======================================================================================
echo ============================== OpenCV - build from source =============================
echo =======================================================================================

cd ~

OPENCV_VER="3.4.3"
sudo wget https://github.com/opencv/opencv/archive/${OPENCV_VER}.zip
mv ${OPENCV_VER}.zip opencv.zip

unzip opencv.zip
cd opencv-${OPENCV_VER}/

mkdir build
cd build

cmake -DENABLE_NEON=ON -DENABLE_VFPV3=ON -D CMAKE_BUILD_TYPE=RELEASE -D BUILD_PERF_TESTS=OFF -D BUILD_SHARED_LIBRARY=ON -D BUILD_TESTS=OFF -D BUILD_opencv_java=OFF -D BUILD_opencv_python2=OFF -D BUILD_opencv_python3=OFF ~/opencv-${OPENCV_VER}
make -j5
sudo make install

cd ../..
rm -f opencv.zip


echo  
echo =======================================================================================
echo ================= Cmake wpilib : Includes cscore, ntcore, and wpiutil =================
echo =======================================================================================

### NOTE:  wpilib build must occur after the opencv build, since cscore is dependent upon opencv
###
### This builds the wpi library ntcore, cscore and wpiutil components,
### builds them using cmake and installs the files to:
###
###   /usr/local/wpilib

cd ~
mkdir wpilib_cmake
pushd wpilib_cmake
git clone --recursive https://github.com/ThadHouse/CmakeWpilib.git .

mkdir build
cd build
cmake .. -DWITHOUT_JAVA=ON
make -j5
sudo make install
popd
