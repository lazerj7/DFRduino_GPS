/*
 * DFRduino_GPS.h - Library For Using DFRduino GPS Shield
 * Uses UBX,00 message to get lat, long, altitude, groundspeed,
 * heading, and vertical velocity data
 * Created by Michael Lazernik and Dillon Haughton
 * MIT License
 */

#ifndef DFRduino_GPS_h
#define DFRduino_GPS_h

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "avr/pgmspace.h"

#undef _SS_MAX_RX_BUFF
#define _SS_MAX_RX_BUFF 600

class DFRduino_GPS {
	public:
		DFRduino_GPS(uint8_t rxPin = 10, uint8_t txPin = 11) : _serial(rxPin, txPin) {};
		void begin(double horizAcc, double vertACC);
		boolean update();
		boolean valid;
		double latitude;
		double longitude;
		double altitude;
		double groundSpeed;
		double heading;
		double verticalVelocity;
		double horizontalAccuracy;
		double verticalAccuracy;
	private:
		void _fill(char* buffer);
		boolean _parse(char* data);
		double _horizAcc;
		double _vertAcc;
		SoftwareSerial _serial;

};

#endif
