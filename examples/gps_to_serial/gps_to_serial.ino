#include <DFRduino_GPS.h>

/*
 * Create an instance of the GPS Shield
 * Arguments are (int receive_pin, int transmit_pin)
 * Uses software serial so ensure pins support pin change interrupts
 */
DFRduino_GPS gps(10, 11);

void setup() {
  /*
   * Initialize GPS
   * Arguments are (double horizontal_accuracy, double vertical_accuracy)
   * representing minimum accuracy (in meters) that will be considered valid
   */
  gps.begin(10.0, 10.0);
  Serial.begin(9600);
}

void loop() {
  if (gps.update()) {
    Serial.print("latitude: ");
    Serial.println(gps.latitude);
    Serial.print("longitude: ");
    Serial.println(gps.longitude);
    Serial.print("altitude: ");
    Serial.println(gps.altitude);
    Serial.print("ground speed: ");
    Serial.println(gps.groundSpeed);
    Serial.print("heading: ");
    Serial.println(gps.heading);
    Serial.print("vertical velocity: ");
    Serial.println(gps.verticalVelocity);
    Serial.print("horizontal accuracy: ");
    Serial.println(gps.horizontalAccuracy);
    Serial.print("vertical accuracy: ");
    Serial.println(gps.verticalAccuracy);
    Serial.println();
    Serial.println();
    Serial.println();
  }
  delay(1000);
}
