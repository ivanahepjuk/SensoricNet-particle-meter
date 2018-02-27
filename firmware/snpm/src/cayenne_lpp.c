/*
 * cayenne_lpp.c
 *
 * Adapted from https://developer.mbed.org/teams/myDevicesIoT/code/Cayenne-LPP/
 * Use of this source code is governed by the MIT license that can be found in the LICENSE file.
 *
 *  Created on: Feb 14, 2018
 *      Author: Jiri Slezka
 */

#include <stdlib.h>
#include <float.h>
#include "cayenne_lpp.h"

struct CayenneLPP
{
	unsigned char *buffer;
	unsigned char maxsize;
	unsigned char cursor;
};

void CayenneLPP__init(struct CayenneLPP* self, unsigned char size)
{
	self->buffer = (unsigned char *)malloc(size);
	self->cursor = 0;
	self->maxsize = 255;
}

struct CayenneLPP* CayenneLPP__create(unsigned char size)
{
	struct CayenneLPP* result = malloc(sizeof(struct CayenneLPP));
	CayenneLPP__init(result, size);
	return result;
}

void CayenneLPP__reset(struct CayenneLPP* self)
{
	self->cursor = 0;
}

void CayenneLPP__destroy(struct CayenneLPP* self) {
	if (self) {
		CayenneLPP__reset(self);
		free(self);
	}
}

unsigned char CayenneLPP__getSize(struct CayenneLPP* self)
{
	return self->cursor;
}

unsigned char *CayenneLPP__getBuffer(struct CayenneLPP* self)
{
	return self->buffer;
}

unsigned char CayenneLPP__copy(struct CayenneLPP* self, unsigned char *dst)
{
	memcpy(dst, self->buffer, self->cursor);
	return self->cursor;
}

unsigned char CayenneLPP__addDigitalInput(struct CayenneLPP* self, unsigned char channel, unsigned char value)
{
	if ((self->cursor + LPP_DIGITAL_INPUT_SIZE) > self->maxsize)
	{
		return 0;
	}
	self->buffer[self->cursor++] = channel;
	self->buffer[self->cursor++] = LPP_DIGITAL_INPUT;
	self->buffer[self->cursor++] = value;

	return self->cursor;
}

unsigned char CayenneLPP__addDigitalOutput(struct CayenneLPP* self, unsigned char channel, unsigned char value)
{
	if ((self->cursor + LPP_DIGITAL_OUTPUT_SIZE) > self->maxsize)
	{
		return 0;
	}
	self->buffer[self->cursor++] = channel;
	self->buffer[self->cursor++] = LPP_DIGITAL_OUTPUT;
	self->buffer[self->cursor++] = value;

	return self->cursor;
}

unsigned char CayenneLPP__addAnalogInput(struct CayenneLPP* self, unsigned char channel, float value)
{
	if ((self->cursor + LPP_ANALOG_INPUT_SIZE) > self->maxsize)
	{
		return 0;
	}

	unsigned short int val = value * 100;
	self->buffer[self->cursor++] = channel;
	self->buffer[self->cursor++] = LPP_ANALOG_INPUT;
	self->buffer[self->cursor++] = val >> 8;
	self->buffer[self->cursor++] = val;

	return self->cursor;
}

unsigned char CayenneLPP__addAnalogOutput(struct CayenneLPP* self, unsigned char channel, float value)
{
	if ((self->cursor + LPP_ANALOG_OUTPUT_SIZE) > self->maxsize)
	{
		return 0;
	}
	unsigned short int val = value * 100;
	self->buffer[self->cursor++] = channel;
	self->buffer[self->cursor++] = LPP_ANALOG_OUTPUT;
	self->buffer[self->cursor++] = val >> 8;
	self->buffer[self->cursor++] = val;

	return self->cursor;
}

unsigned char CayenneLPP__addLuminosity(struct CayenneLPP* self, unsigned char channel, unsigned short int lux)
{
	if ((self->cursor + LPP_LUMINOSITY_SIZE) > self->maxsize)
	{
		return 0;
	}
	self->buffer[self->cursor++] = channel;
	self->buffer[self->cursor++] = LPP_LUMINOSITY;
	self->buffer[self->cursor++] = lux >> 8;
	self->buffer[self->cursor++] = lux;

	return self->cursor;
}

unsigned char CayenneLPP__addPresence(struct CayenneLPP* self, unsigned char channel, unsigned char value)
{
	if ((self->cursor + LPP_PRESENCE_SIZE) > self->maxsize)
	{
		return 0;
	}
	self->buffer[self->cursor++] = channel;
	self->buffer[self->cursor++] = LPP_PRESENCE;
	self->buffer[self->cursor++] = value;

	return self->cursor;
}

unsigned char CayenneLPP__addTemperature(struct CayenneLPP* self, unsigned char channel, float celsius)
{
	if ((self->cursor + LPP_TEMPERATURE_SIZE) > self->maxsize)
	{
		return 0;
	}
	unsigned short int val = celsius * 10;
	self->buffer[self->cursor++] = channel;
	self->buffer[self->cursor++] = LPP_TEMPERATURE;
	self->buffer[self->cursor++] = val >> 8;
	self->buffer[self->cursor++] = val;

	return self->cursor;
}

unsigned char CayenneLPP__addRelativeHumidity(struct CayenneLPP* self, unsigned char channel, float rh)
{
	if ((self->cursor + LPP_RELATIVE_HUMIDITY_SIZE) > self->maxsize)
	{
		return 0;
	}
	self->buffer[self->cursor++] = channel;
	self->buffer[self->cursor++] = LPP_RELATIVE_HUMIDITY;
	self->buffer[self->cursor++] = rh * 2;

	return self->cursor;
}

unsigned char CayenneLPP__addAccelerometer(struct CayenneLPP* self, unsigned char channel, float x, float y, float z)
{
	if ((self->cursor + LPP_ACCELEROMETER_SIZE) > self->maxsize)
	{
		return 0;
	}
	unsigned short int vx = x * 1000;
	unsigned short int vy = y * 1000;
	unsigned short int vz = z * 1000;

	self->buffer[self->cursor++] = channel;
	self->buffer[self->cursor++] = LPP_ACCELEROMETER;
	self->buffer[self->cursor++] = vx >> 8;
	self->buffer[self->cursor++] = vx;
	self->buffer[self->cursor++] = vy >> 8;
	self->buffer[self->cursor++] = vy;
	self->buffer[self->cursor++] = vz >> 8;
	self->buffer[self->cursor++] = vz;

	return self->cursor;
}

unsigned char CayenneLPP__addBarometricPressure(struct CayenneLPP* self, unsigned char channel, float hpa)
{
	if ((self->cursor + LPP_BAROMETRIC_PRESSURE_SIZE) > self->maxsize)
	{
		return 0;
	}
	unsigned short int val = hpa * 10;

	self->buffer[self->cursor++] = channel;
	self->buffer[self->cursor++] = LPP_BAROMETRIC_PRESSURE;
	self->buffer[self->cursor++] = val >> 8;
	self->buffer[self->cursor++] = val;

	return self->cursor;
}

unsigned char CayenneLPP__addGyrometer(struct CayenneLPP* self, unsigned char channel, float x, float y, float z)
{
	if ((self->cursor + LPP_GYROMETER_SIZE) > self->maxsize)
	{
		return 0;
	}
	unsigned short int vx = x * 100;
	unsigned short int vy = y * 100;
	unsigned short int vz = z * 100;

	self->buffer[self->cursor++] = channel;
	self->buffer[self->cursor++] = LPP_GYROMETER;
	self->buffer[self->cursor++] = vx >> 8;
	self->buffer[self->cursor++] = vx;
	self->buffer[self->cursor++] = vy >> 8;
	self->buffer[self->cursor++] = vy;
	self->buffer[self->cursor++] = vz >> 8;
	self->buffer[self->cursor++] = vz;

	return self->cursor;
}

unsigned char CayenneLPP__addGPS(struct CayenneLPP* self, unsigned char channel, float latitude, float longitude, float meters)
{
	if ((self->cursor + LPP_GPS_SIZE) > self->maxsize)
	{
		return 0;
	}
	unsigned long lat = latitude * 10000;
	unsigned long lon = longitude * 10000;
	unsigned long alt = meters * 100;

	self->buffer[self->cursor++] = channel;
	self->buffer[self->cursor++] = LPP_GPS;

	self->buffer[self->cursor++] = lat >> 16;
	self->buffer[self->cursor++] = lat >> 8;
	self->buffer[self->cursor++] = lat;
	self->buffer[self->cursor++] = lon >> 16;
	self->buffer[self->cursor++] = lon >> 8;
	self->buffer[self->cursor++] = lon;
	self->buffer[self->cursor++] = alt >> 16;
	self->buffer[self->cursor++] = alt >> 8;
	self->buffer[self->cursor++] = alt;

	return self->cursor;
}
