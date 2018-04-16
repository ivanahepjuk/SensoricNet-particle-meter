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

