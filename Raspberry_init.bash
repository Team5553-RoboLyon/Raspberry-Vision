#!/bin/bash

echo =======================================================================================
echo ======================== Ensure that Raspbian is all up-to-date =======================
echo =======================================================================================

sudo apt-get -y update
sudo apt-get -y upgrade


echo
echo =======================================================================================
echo ================================== Required Packages ==================================
echo =======================================================================================

# Required Packages for Opencv
sudo apt-get -y install build-essential
sudo apt-get -y install git cmake pkg-config
sudo apt-get -y install libgtk2.0-dev libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get -y install libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev

# Samba pour pouvoir accéder au nom d'hôte du Raspberry sous Windows
sudo apt-get -y install samba


echo
echo =======================================================================================
echo ============================== OpenCV - build from source =============================
echo =======================================================================================

cd ~

OPENCV_VER="3.4.3"
sudo wget https://github.com/opencv/opencv/archive/${OPENCV_VER}.zip
unzip ${OPENCV_VER}.zip

cd opencv-${OPENCV_VER}/
mkdir build
cd build

cmake -DENABLE_NEON=ON \
	-DENABLE_VFPV3=ON \
	-DCMAKE_BUILD_TYPE=RELEASE \
	-DBUILD_SHARED_LIBRARY=ON \
	-DBUILD_opencv_java=OFF \
	-DBUILD_JAVA=OFF \
	-DBUILD_opencv_python2=OFF \
	-DBUILD_opencv_python3=OFF \
	-DBUILD_EXAMPLES=OFF \
	-DBUILD_TESTS=OFF \
	-DBUILD_PERF_TESTS=OFF \
	..

make -j5
sudo make install

cd ~
rm -f ${OPENCV_VER}.zip


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
git clone https://github.com/wpilibsuite/allwpilib
cd allwpilib
mkdir buildcmake
cd buildcmake

cmake .. -DWITHOUT_JAVA=ON
make -j5
sudo make install
