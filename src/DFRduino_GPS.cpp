/*
 * DFRduino_GPS.cpp - Library For Using DFRduino GPS Shield
 * Created by Michael Lazernik and Dillon Haughton
 * MIT License
 */

/*
 * NOTE: Despite my best efforts, this is hugely memory intensive.
 * Parsing NMEA/GPS messages is just a memory intensive process
 */

#define I2C_CLOCK 100000
#define ADDR 0x42

#include "DFRduino_GPS.h"

/***************************************************************************************
 * Constructor                                                                         *
 * horizAcc is minimum reported horizontal accuracy that will be reported as valid fix *
 * vertAcc is minimum reported vertical accuracy that will be reported as valid fix    *
 ***************************************************************************************/
DFRduino_GPS::DFRduino_GPS(double horizAcc, double vertAcc) {
	valid = false; //no valid data yet
	_horizAcc = horizAcc; //store min horizontal accuracy
	_vertAcc = vertAcc;  //stor min vertical accuracy

	int i; //counter

	Wire.begin(); //initialize I2C
	Wire.setClock(I2C_CLOCK); //set I2C clock

	//GPS Configuration Messages
	//To save SRAM (because these are huge, again, stupid GPS)
	//Store these in progmem (flash)
//----------------------------------------------------------------------------------------------------//
	uint8_t ubxEnable[28] PROGMEM = {0xB5,0x62,0x06,0x00,0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x01,0x00,0x02,0x00,0x00,0x00,0x24,0x8E}; //Enable only UBX messages over I2C and set extended timeout

	uint8_t ubx00Enable[16] PROGMEM = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF1,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x31}; //Enable UBX 00 Message (The only one we'll use)
	
	uint8_t ubx01Disable[16] PROGMEM = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF1,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x32}; //Disable UBX 01 Message

	uint8_t ubx03Disable[16] PROGMEM = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF1,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x40}; //Disable UBX 03 Message

	uint8_t ubx04Disable[16] PROGMEM = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF1,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x47}; //Disable UBX 04 Message
	
	uint8_t fullPower[16] PROGMEM = {0xB5,0x62,0x06,0x86,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x94,0x5A}; //Set Module to Full Power

	uint8_t continuousMode[10] PROGMEM = {0xB5,0x62,0x06,0x11,0x02,0x00,0x08,0x00,0x21,0x91}; //Set Module to Continuous Mode

	uint8_t portable3d[44] PROGMEM = {0xB5,0x62,0x06,0x24,0x24,0x00,0xFF,0xFF,0x00,0x02,0x00,0x00,0x00,0x00,0x10,0x27,0x00,0x00,0x05,0x00,0xFA,0x00,0xFA,0x00,0x64,0x00,0x2C,0x01,0x00,0x00,0x00,0x00,0x10,0x27,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x46,0xEE}; //Set module to portable nav mode with 3d fix only

	uint8_t rate[14] PROGMEM {0xB5,0x62,0x06,0x08,0x06,0x00,0x33,0x00,0x01,0x00,0x01,0x00,0x49,0xEC}; //set refresh rate to 19.61Hz (Maximum for device)
//----------------------------------------------------------------------------------------------------//

	//Send Configuration Messages
//---------------------------------------------------------------------------------------------------//
	Wire.beginTransmission(ADDR);
	for(i = 0; i < 28; i++) {
		Wire.write(pgm_read_byte_near(ubxEnable + i));
	}
	Wire.endTransmission();
	
	Wire.beginTransmission(ADDR);
	for(i = 0; i < 16; i++) {
		Wire.write(pgm_read_byte_near(ubx00Enable + i));
	}
	Wire.endTransmission();

	Wire.beginTransmission(ADDR);
	for(i = 0; i < 16; i++) {
		Wire.write(pgm_read_byte_near(ubx01Disable + i));
	}
	Wire.endTransmission();

	Wire.beginTransmission(ADDR);
	for(i = 0; i < 16; i++) {
		Wire.write(pgm_read_byte_near(ubx03Disable + i));
	}
	Wire.endTransmission();

	Wire.beginTransmission(ADDR);
	for(i = 0; i < 16; i++) {
		Wire.write(pgm_read_byte_near(ubx04Disable + i));
	}
	Wire.endTransmission();

	Wire.beginTransmission(ADDR);
	for(i = 0; i < 16; i++) {
		Wire.write(pgm_read_byte_near(fullPower + i));
	}
	Wire.endTransmission();

	Wire.beginTransmission(ADDR);
	for(i = 0; i < 10; i++) {
		Wire.write(pgm_read_byte_near(continuousMode + i));
	}
	Wire.endTransmission();

	Wire.beginTransmission(ADDR);
	for(i = 0; i < 44; i++) {
		Wire.write(pgm_read_byte_near(portable3d + i));
	}
	Wire.endTransmission();

	Wire.beginTransmission(ADDR);
	for(i = 0; i < 14; i++) {
		Wire.write(pgm_read_byte_near(rate + i));
	}
	Wire.endTransmission();
//----------------------------------------------------------------------------------------------------//
}

/************************************************
 * GPS update                                   *
 * Gets most current readings from GPS receiver *
 ************************************************/
boolean DFRduino_GPS::update() {
	char buffer[150]; //message buffer
	char message[8] PROGMEM = {'$','P','U','B','X',',','0','0'}; //message identifier we're looking for
	uint16_t available; //number of bytes available in receiver buffer
	int i; //counter
	buffer[7] = '\0';  //initialize 8th character in buffer to null (We'll check this later)

	//get how many bytes available from reveiver
	Wire.beginTransmission(ADDR);
	Wire.write(0xFD);
	Wire.endTransmission();
	Wire.requestFrom(ADDR, 2);
	available = ((uint16_t) Wire.read()) << 8;
	available |= ((uint16_t) Wire.read());

	//if nothing available, just return false
	//We will still consider previous valid data (if it exists) to be valid
	if (available == 0) {
		return false;
	}

	//Loop through every darn byte available
	while(available) {
		i = 0;
		
		//Written for 32 byte I2C buffer
		//Buffer was increased to 44 bytes in header, but we didn't see a need to change this
		//continously fills I2C buffer with 32 bytes in each iteration until we have read
		//everything
		if (available < 32) {
			Wire.requestFrom(ADDR, (int) available);
			available = 0;
		}
		else {
			Wire.requestFrom(ADDR, 32);
			available -= 32;
		}

		//While we have data in I2C buffer
		//reads that data and compares it byte by byte to our target message
		//if we get a message call _fill() to fill the buffer with the message
		//contents, and then keep looping in case there is a more recent message
		while(Wire.available()) {
			buffer[i] = Wire.read();
			if (buffer[i] == (pgm_read_byte_near(message + i))) {
				i++;
			}
			if (i == 7) {
				_fill(buffer, &available);
			}
		}
	}
	
	//Now we've read everything, check if we found our message
	//if we did the 8th value in the buffer should be the character 0
	//if not the 8th value in the buffer should be null
	//again, we'll keep previous valid data if it exists
	if (buffer[7] == '\0') {
		return false;
	}

	//Ok, we didn't return false above, so we must have found our message
	//now we parse that message to get new data in a usable form and 
	//check if that data meets our minimum accuracy requirements
	//in this case, since we check data validity mid way through parsing
	//(the message structure somewhat forces our hand, stupid gps protocol)
	//we will invalidate the partial data we read out in the case of not meeting
	//accuracy requirements
	if(_parse(buffer)) {
		valid = true;
		return true;
	}
	else {
		valid = false;
		latitude = longitude = altitude = groundSpeed = heading = verticalVelocity = 0;
		return false;
	}
}

/*********************************************
 * Function to fill buffer with Message data *
 *********************************************/
void DFRduino_GPS::_fill(char* buffer, uint16_t* available) {
	int i = 7; //start counting at the 8th value of the buffer

	//messages all end with a carriage return and a line feed
	//we want to read everything up to that carriage return
	//Again here we continuously fill the I2C buffer with 32 bytes
	//Didn't see a need to up it to 44 even though that is now possible
	while (buffer[i] != '\r') {
		i++;
		if (Wire.available()) {
			buffer[i] = Wire.read();
		}
		else {
			if (*available < 32) {
				Wire.requestFrom(ADDR, (int) available);
				*available = 0;
				buffer[i] = Wire.read();
			}
			else {
				Wire.requestFrom(ADDR, 32);
				*available -= 32;
				buffer[i] = Wire.read();
			}
		}
	}
	
	//replace that carriage return in the buffer with a null
	//making the buffer a proper C string (Yay!)
	//this actaully doesn't matter after a redesign, but for the
	//sake of keeping with proper C it doesn't hurt to make this
	//a valid string
	buffer[i] = '\0';
}

/************************************************************
 * Function to Parse GPS message into usable data (doubles) *
 ************************************************************/
boolean DFRduino_GPS::_parse(char* buffer) {
	int i, j = 0; //counters
	uint8_t degrees, minutes; //temporary holders for degree and minute values for lat and long
	double number = 0.0; //temporary holder for floating point values

	//message data is comma seperated, what we're interested in starts at
	//the fourth block, so skip to the fourth block by counting commas
	for(i = 0; i < 3; i++) {
		while(buffer[j] != ',') {
			j++;
		}
	}

	//parse latitude into double and update it
	degrees = (10 * (((uint8_t) buffer[++j]) - 0x30));
	degrees += (((uint8_t) buffer[++j]) - 0x30);

	minutes = (10 * (((uint8_t) buffer[++j]) - 0x30));
        minutes += (((uint8_t) buffer[++j]) - 0x30);

	i = -1;

	while(buffer[++j] != ',') {
		number += (pow(10, i)) * ((double) (((uint8_t) buffer[j]) - 0x30));
		i--;
	}
	
	latitude = ((double) degrees) + ((((double) minutes) + number) / 60.0);

	//We will use the +/- convention instead of northings and southings,
	//so if we have a southing make it negative
	if(buffer[++j] == 'S') {
		latitude *= -1.0;
	}

	//skip over the comma
	j++;

	//parse longitude into double and update it
	degrees = (100 * (((uint8_t) buffer[++j]) - 0x30));
	degrees += (10 * (((uint8_t) buffer[++j]) - 0x30));
	degrees += (((uint8_t) buffer[++j]) - 0x30);

	minutes = (10 * (((uint8_t) buffer[++j]) - 0x30));
	minutes += (((uint8_t) buffer[++j]) - 0x30);

	i = -1;
	number = 0.0;

	while (buffer[++j] != ',') {
		number += (pow(10, i)) * ((double) (((uint8_t) buffer[j]) - 0x30));
		i--;
	}

	longitude = ((double) degrees) + ((((double) minutes) + number) / 60.0);

	//again we will use +/- instead of eastings and westings,
	//so if we have a westing make it negative
	if(buffer[++j] == 'W') {
		longitude *= -1.0;
	}
	
	//skip over the comma
	j++;

	//parse altitude into double and update it
	i = 1;
	number = (double) (((uint8_t) buffer[++j]) - 0x30);

	while(buffer[++j] != '.') {
		number *= pow(10, i);
		number += (double) (((uint8_t) buffer[j]) - 0x30);
		i++;
	}

	i = -1;

	while(buffer[++j] != ',') {
		number += (pow(10, i)) * ((double) (((uint8_t) buffer[j]) - 0x30));
		i--;
	}

	altitude = number;

	//First validity check, it kind of sucks that this isn't at the beginning,
	//but so be it. If we have NF(No Fix) or TT(Time Only) then our data is 
	//not valid, so return false
	if((buffer[++j] == 'N' || buffer[j] == 'T') && (buffer[++j] == 'F' || buffer[j] == 'T')) {
		return false;
	}

	i = 1;
	number = (double) (((uint8_t) buffer[++j]) - 0x30);

	//Second validity check, parse horizontal accuracy into double and compare
	//it to our minimum horizontal accuracy value. If data is not accurate
	//enough, return false
	while(buffer[++j] != '.') {
		number *= pow(10, i);
		number += (double) (((uint8_t) buffer[j]) - 0x30);
		i++;
	}

	i = -1;

	while(buffer[++j] != ',') {
		number += (pow(10, i)) * ((double) (((uint8_t) buffer[j]) - 0x30));
		i--;
	}

	if(number > _horizAcc) {
		return false;
	}

	//TODO remove below
	horizontalAccuracy = number;
	//TODO remove above

	//Third and final validity check, parse vertical accuracy into double and compare
	//it to our minimum vertical accuracy value. If data is not accurate enough,
	//return false
	i = 1;
	number = (double) (((uint8_t) buffer[++j]) - 0x30);

	while(buffer[++j] != '.') {
		number *= pow(10, i);
		number += (double) (((uint8_t) buffer[j]) - 0x30);
		i++;
	}

	i = -1;

	while(buffer[++j] != ',') {
		number += (pow(10, i)) * ((double) (((uint8_t) buffer[j]) - 0x30));
		i--;
	}

	if(number > _vertAcc) {
		return false;
	}

	//TODO remove below
	verticalAccuracy = number;
	//TODO remove above

	//Parse ground speed into double and store it
	i = 1;
	number = (double) (((uint8_t) buffer[++j]) - 0x30);

	while(buffer[++j] != '.') {
		number *= pow(10, i);
		number += (double) (((uint8_t) buffer[j]) - 0x30);
		i++;
	}

	i = -1;

	while(buffer[++j] != ',') {
		number += (pow(10, i)) * ((double) (((uint8_t) buffer[j]) - 0x30));
		i--;
	}

	groundSpeed = number;

	//parse heading into double and store it
	i = 1;
	number = (double) (((uint8_t) buffer[++j]) - 0x30);

	while(buffer[++j] != '.') {
		number *= pow(10, i);
		number += (double) (((uint8_t) buffer[j]) - 0x30);
		i++;
	}

	i = -1;

	while(buffer[++j] != ',') {
		number += (pow(10, i)) * ((double) (((uint8_t) buffer[j]) - 0x30));
		i--;
	}

	heading = number;

	//We parsed all or our data, and if we made it here it is valid,
	//so return true
	return true;
}
