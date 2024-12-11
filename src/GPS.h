#ifndef GPS_H
#define GPS_H

#include <Arduino.h>

/**
 * @class GPS
 * @brief A class for interfacing with a GPS module using NMEA sentences.
 * 
 * The GPS class provides methods to read and parse GPS data, including latitude,
 * longitude, altitude, UTC time, and fix status. It communicates with the GPS
 * module via a hardware serial interface.
 */
class GPS
{
public:
    /**
     * @brief Constructor for the GPS class.
     * @param serial Reference to the hardware serial interface for GPS communication.
     * @param baud The baud rate for GPS communication (default: 9600).
     */
    GPS(HardwareSerial &serial, uint32_t baud = 9600);

    /**
     * @brief Initialize the GPS module by setting up the serial communication.
     */
    void begin();

    /**
     * @brief Update the GPS data by reading and parsing NMEA sentences.
     * @return True if a valid NMEA sentence was parsed; otherwise, false.
     */
    bool update();

    /**
     * @brief Get the latitude as a string.
     * @return The latitude in NMEA format.
     */
    String getLatitude();

    /**
     * @brief Get the longitude as a string.
     * @return The longitude in NMEA format.
     */
    String getLongitude();

    /**
     * @brief Get the UTC time as a string.
     * @return The UTC time in NMEA format.
     */
    String getUTC();

    /**
     * @brief Get the GPS fix status as a string.
     * @return The fix status from the GPS module.
     */
    String getFixStatus();

    /**
     * @brief Get the altitude as a string.
     * @return The altitude in meters as reported by the GPS module.
     */
    String getAltitude();

private:
    HardwareSerial &gpsSerial; /**< Reference to the serial port used for GPS communication. */
    uint32_t gpsBaud;          /**< Baud rate for GPS communication. */
    String nmeaBuffer;         /**< Buffer to hold the current NMEA sentence. */
    String latitude, longitude, altitude, utcTime, fixStatus; /**< Parsed GPS data fields. */

    /**
     * @brief Parse an NMEA sentence to extract GPS data.
     * @param nmea The NMEA sentence to parse.
     */
    void parseNMEA(const String &nmea);
};

#endif
