#include <DFRduino_GPS.h>

//Create an instance of the GPS Shield
//Arguments are (double <minimum horizontal accuracy>, double <minimum vertical accuracy>)
//Accuracies are in meters
DFRduino_GPS gps(10.0, 10.0);

void setup() {
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
