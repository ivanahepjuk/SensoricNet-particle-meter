# SensoricNet-particle-meter
This is repo for sensoric net, project backed by Vodafone. Nb-Iot stuff.

# Firmware
Please go to the firmware folder

# Hardware
please go to HW folder


# Install

Fedora:

```
sudo yum install openocd
sudo udevadm control -R
```

```
wget "https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-rm/6-2017q2/gcc-arm-none-eabi-6-2017-q2-update-linux.tar.bz2"
tar xvjf gcc-arm-none-eabi-6-2017-q2-update-linux.tar.bz2
```

add path to .bash_profile

```
PATH=$PATH:/home/user/gcc-arm-none-eabi-6-2017-q2-update/bin
```

```
source .bash_profile
```

Get libopencm3

```
git clone https://github.com/libopencm3/libopencm3.git
```

and compile it

```
cd libopencm3
make
```

Get the SensoricNet stuff

```
git clone https://github.com/ivanahepjuk/SensoricNet-particle-meter.git
```
connect device

```
cd SensoricNet-particle-meter/firmware/snpm/
make flash
```

