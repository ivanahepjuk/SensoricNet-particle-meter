# SensoricNet-particle-meter
This is repo for sensoric net, project backed by Vodafone. Nb-Iot stuff.
# FW
Please go to the firmware folder
# HW
please go to HW folder


Install

Fedora:

sudo yum install openocd
sudo udevadm control -R

wget "https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-rm/6-2017q2/gcc-arm-none-eabi-6-2017-q2-update-linux.tar.bz2"

tar xvjf gcc-arm-none-eabi-6-2017-q2-update-linux.tar.bz2

add path to .bash_profile

PATH=$PATH:/home/user/gcc-arm-none-eabi-6-2017-q2-update/bin

source .bash_profile

git clone https://github.com/dron23/SensoricNet-particle-meter.git --recursive

cd SensoricNet-particle-meter/FW/libopencm3/
make

connect device

cd SensoricNet-particle-meter/FW/SNPM/
make flash


