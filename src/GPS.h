#ifndef GPS_H
#define GPS_H

#include <Arduino.h>

class GPS
{
    public:
        GPS(HardwareSerial &serial, uint32_t baud = 9600);
        void begin();                   // Initialize the GPS
        bool update();                  // Read and parse NMEA sentence
        String getLatitude();           // Get latitude as a string
        String getLongitude();          // Get longitude as a string
        String getUTC();                // Get UTC time
        String getFixStatus();          // Get fix status
        String getAltitude();           // Get altitude

    private:
        HardwareSerial &gpsSerial;      // Reference to the serial port used for GPS
        uint32_t gpsBaud;               // Baud rate for GPS communication
        String nmeaBuffer;              // Buffer to hold NMEA sentence
        String latitude, longitude, altitude, utcTime, fixStatus;

        void parseNMEA(const String &nmea); // Parse the NMEA sentence
};

#endif
