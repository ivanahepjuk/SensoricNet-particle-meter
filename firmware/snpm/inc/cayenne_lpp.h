struct CayenneLPP; // forward declared for encapsulation

struct CayenneLPP* CayenneLPP__create(unsigned char size);
void CayenneLPP__destroy(struct CayenneLPP* self);
void CayenneLPP__reset(struct CayenneLPP* self);
unsigned char CayenneLPP__getSize(struct CayenneLPP* self);
unsigned char *CayenneLPP__getBuffer(struct CayenneLPP* self);
unsigned char CayenneLPP__copy(struct CayenneLPP* self, unsigned char *dst);
unsigned char CayenneLPP__addDigitalInput(struct CayenneLPP* self, unsigned char channel, unsigned char value);
unsigned char CayenneLPP__addDigitalOutput(struct CayenneLPP* self, unsigned char channel, unsigned char value);
unsigned char CayenneLPP__addAnalogInput(struct CayenneLPP* self, unsigned char channel, float value);
unsigned char CayenneLPP__addAnalogOutput(struct CayenneLPP* self, unsigned char channel, float value);
unsigned char CayenneLPP__addLuminosity(struct CayenneLPP* self, unsigned char channel, unsigned short int lux);
unsigned char CayenneLPP__addPresence(struct CayenneLPP* self, unsigned char channel, unsigned char value);
unsigned char CayenneLPP__addTemperature(struct CayenneLPP* self, unsigned char channel, float celsius);
unsigned char CayenneLPP__addRelativeHumidity(struct CayenneLPP* self, unsigned char channel, float rh);
unsigned char CayenneLPP__addAccelerometer(struct CayenneLPP* self, unsigned char channel, float x, float y, float z);
unsigned char CayenneLPP__addBarometricPressure(struct CayenneLPP* self, unsigned char channel, float hpa);
unsigned char CayenneLPP__addGyrometer(struct CayenneLPP* self, unsigned char channel, float x, float y, float z);
unsigned char CayenneLPP__addGPS(struct CayenneLPP* self, unsigned char channel, float latitude, float longitude, float meters);

#define LPP_DIGITAL_INPUT 0         // 1 byte
#define LPP_DIGITAL_OUTPUT 1        // 1 byte
#define LPP_ANALOG_INPUT 2          // 2 bytes, 0.01 signed
#define LPP_ANALOG_OUTPUT 3         // 2 bytes, 0.01 signed
#define LPP_LUMINOSITY 101          // 2 bytes, 1 lux unsigned
#define LPP_PRESENCE 102            // 1 byte, 1
#define LPP_TEMPERATURE 103         // 2 bytes, 0.1°C signed
#define LPP_RELATIVE_HUMIDITY 104   // 1 byte, 0.5% unsigned
#define LPP_ACCELEROMETER 113       // 2 bytes per axis, 0.001G
#define LPP_BAROMETRIC_PRESSURE 115 // 2 bytes 0.1 hPa Unsigned
#define LPP_GYROMETER 134           // 2 bytes per axis, 0.01 °/s
#define LPP_GPS 136                 // 3 byte lon/lat 0.0001 °, 3 bytes alt 0.01 meter

// Data ID + Data Type + Data Size
#define LPP_DIGITAL_INPUT_SIZE 3       // 1 byte
#define LPP_DIGITAL_OUTPUT_SIZE 3      // 1 byte
#define LPP_ANALOG_INPUT_SIZE 4        // 2 bytes, 0.01 signed
#define LPP_ANALOG_OUTPUT_SIZE 4       // 2 bytes, 0.01 signed
#define LPP_LUMINOSITY_SIZE 4          // 2 bytes, 1 lux unsigned
#define LPP_PRESENCE_SIZE 3            // 1 byte, 1
#define LPP_TEMPERATURE_SIZE 4         // 2 bytes, 0.1°C signed
#define LPP_RELATIVE_HUMIDITY_SIZE 3   // 1 byte, 0.5% unsigned
#define LPP_ACCELEROMETER_SIZE 8       // 2 bytes per axis, 0.001G
#define LPP_BAROMETRIC_PRESSURE_SIZE 4 // 2 bytes 0.1 hPa Unsigned
#define LPP_GYROMETER_SIZE 8           // 2 bytes per axis, 0.01 °/s
#define LPP_GPS_SIZE 11                // 3 byte lon/lat 0.0001 °, 3 bytes alt 0.01 meter
