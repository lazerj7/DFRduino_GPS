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
#include "Wire.h"
#include "avr/pgmspace.h"

//Increase I2C buffer to 44 bytes because config messages are long.....
#undef BUFFER_LENGTH
#define BUFFER_LENGTH 44
#undef TWI_BUFFER_LENGTH
#define TWI_BUFFER_LENGTH 44

class DFRduino_GPS {
	public:
		DFRduino_GPS(double horizAcc, double vertAcc);
		boolean update();
		boolean valid;
		double latitude;
		double longitude;
		double altitude;
		double groundSpeed;
		double heading;
		double verticalVelocity;
		//TODO remove below
		double horizontalAccuracy;
		double verticalAccuracy;
		//TODO remove above
	private:
		void _fill(char* buffer, uint16_t* available);
		boolean _parse(char* data);
		double _horizAcc;
		double _vertAcc;

};

#endif
