Requirements
============

* CMake
* OpenCV 2.4 (http://opencv.org)
* PiBits ServoBlaster (https://github.com/richardghirst/PiBits/tree/master/ServoBlaster)
* mmal

Install
=======

* install cmake
    * Arch linux: pacman -S cmake
* build/install opencv
    * Arch linux: pacman -S opencv
* build/install mmal
    * Arch linux: pacman -S raspberrypi-firmware-tools
* build servoblaster
    * cd <pibits dir>/ServoBlaster/user/
    * make
* build raspicam
    * cd <mechanical_eyes dir>/libs/raspicam-0.0.5/
    * mkdir build && cd build
    * cmake ..
    * make && make install
* build mechanical\_eyes
    * cd <mechanical_eyes dir>
    * mkdir build && cd build
    * cmake ..
    * make && make install


Run
===

* start servod from servoblaster directory
* start mechanical\_eyes
