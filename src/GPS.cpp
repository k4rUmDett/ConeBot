#include "GPS.h"

/**
 * @brief Constructor for the GPS class.
 * @param serial Reference to the hardware serial interface for GPS communication.
 * @param baud The baud rate for GPS communication (default: 9600).
 */
GPS::GPS(HardwareSerial &serial, uint32_t baud)
    : gpsSerial(serial), gpsBaud(baud), latitude(""), longitude(""),
      altitude(""), utcTime(""), fixStatus("") {}

/**
 * @brief Initialize the GPS module by starting the serial communication.
 */
void GPS::begin()
{
    gpsSerial.begin(gpsBaud);
}

/**
 * @brief Update the GPS data by reading and parsing an NMEA sentence.
 * @return True if a valid NMEA sentence was parsed; otherwise, false.
 */
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

/**
 * @brief Parse an NMEA sentence to extract GPS data fields.
 * @param nmea The NMEA sentence to parse.
 */
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

/**
 * @brief Get the latitude as a string.
 * @return The latitude in NMEA format.
 */
String GPS::getLatitude()
{
    return latitude;
}

/**
 * @brief Get the longitude as a string.
 * @return The longitude in NMEA format.
 */
String GPS::getLongitude()
{
    return longitude;
}

/**
 * @brief Get the UTC time as a string.
 * @return The UTC time in NMEA format.
 */
String GPS::getUTC()
{
    return utcTime;
}

/**
 * @brief Get the GPS fix status as a string.
 * @return The fix status from the GPS module.
 */
String GPS::getFixStatus()
{
    return fixStatus;
}

/**
 * @brief Get the altitude as a string.
 * @return The altitude in meters as reported by the GPS module.
 */
String GPS::getAltitude()
{
    return altitude;
}
