#include "GPS.h"

GPS::GPS(HardwareSerial &serial, uint32_t baud)
    : gpsSerial(serial), gpsBaud(baud), latitude(""), longitude(""),
      altitude(""), utcTime(""), fixStatus("") {}

void GPS::begin()
{
    gpsSerial.begin(gpsBaud);
}

bool GPS::update()
{
    while (gpsSerial.available()) {
        char c = gpsSerial.read();
        if (c == '\n') { // End of NMEA sentence
            parseNMEA(nmeaBuffer);
            nmeaBuffer = ""; // Clear the buffer for the next sentence
            return true;
        } else {
            nmeaBuffer += c;
        }
    }
    return false; // No complete sentence received
}

void GPS::parseNMEA(const String &nmea)
{
    if (nmea.startsWith("$GPGGA")) { // Parse $GPGGA sentence
        int fieldIndex = 0;
        String field;
        for (int i = 0; i < nmea.length(); i++) {
            char c = nmea[i];
            if (c == ',' || i == nmea.length() - 1) {
                if (fieldIndex == 1) utcTime = field;         // UTC time
                if (fieldIndex == 2) latitude = field;        // Latitude
                if (fieldIndex == 4) longitude = field;       // Longitude
                if (fieldIndex == 6) fixStatus = field;       // Fix status
                if (fieldIndex == 9) altitude = field;        // Altitude
                field = "";
                fieldIndex++;
            } else {
                field += c;
            }
        }
    }
}

String GPS::getLatitude()
{
    return latitude;
}

String GPS::getLongitude()
{
    return longitude;
}

String GPS::getUTC()
{
    return utcTime;
}

String GPS::getFixStatus()
{
    return fixStatus;
}

String GPS::getAltitude()
{
    return altitude;
}
