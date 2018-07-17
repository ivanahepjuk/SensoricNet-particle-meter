# SensoricNet-particle-meter
This is repo for sensoric net, project backed by Vodafone. Nb-Iot stuff.


# Firmware
Please go to the firmware folder

# Hardware
please go to HW folder


# Install

Fedora:

(tested on Fedora release 26+)

Install openocd

```
sudo yum install openocd
sudo udevadm control -R
```

get the repo version of GNU Arm Embedded toolchains

```
sudo yum install arm-none-eabi-gdb arm-none-eabi-binutils-cs arm-none-eabi-gcc-cs arm-none-eabi-newlib
```

alternatively you can particular version of GNU Arm Embedded toolchains by

```
wget "https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-rm/6-2017q2/gcc-arm-none-eabi-6-2017-q2-update-linux.tar.bz2"
tar xvjf gcc-arm-none-eabi-6-2017-q2-update-linux.tar.bz2
```

and add path to .bash_profile

```
PATH=$PATH:/home/user/gcc-arm-none-eabi-6-2017-q2-update/bin
```

```
source .bash_profile
```

Get libopencm3

```
mkdir ~/git
cd ~/git
git clone https://github.com/libopencm3/libopencm3.git
```

and compile it

```
cd ~/git/libopencm3
make
```

Get the SensoricNet stuff

```
cd ~/git
git clone https://github.com/ivanahepjuk/SensoricNet-particle-meter.git
```

connect device, compile and flash the firmware

```
cd ~/git/SensoricNet-particle-meter/firmware/snpm/
make clean ; make flash
```

or probably the smarter way is using simple provisioning utility

- rename sensoricnet_lora_ids_example.csv file to sensoricnet_lora_ids.csv and modify it (add your data)
- compile and flash the stuff by running 

```
cd ~/git/SensoricNet-particle-meter/firmware/snpm/
./make_lora.sh <your_sensor_dev_id>
```


#  Debian 9
code:
```
git clone ivanahepjuk/SensoricNet...
```
libopencm3 library:
```
cd SensoricNet.../firmware
git clone libopencm3
```
toolchain:
```


go to your home folder and make dedicated folder for arm toolchain:
mkdir ~/ARM_toolchain

download toolchain file into this ARM_toolchain folder:
https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads/6-2017-q2-update

extract it:
tar -xf <toolchain_file>

go to your home folder 
cd ~/

make a new file, name it .bash_profile
nano .bash_profile

and define path top your toolchain here (add this line with path to your toolchain files)
PATH=$PATH:/home/ia/ARM_toolchain/gcc-arm-none-eabi-6-2017-q2-update/bin

save it by pressing ctrl+x and then load it:
source ~/.bash_profile

test it by writing this into terminal:
arm-none-eabi-gcc --version

you should get this:
arm-none-eabi-gcc (GNU Tools for ARM Embedded Processors 6-2017-q2-update) 6.3.1 20170620 (release) [ARM/embedded-6-branch revision 249437]
Copyright (C) 2016 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
now installing openOCD:


