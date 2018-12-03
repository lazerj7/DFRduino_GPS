# DFRduino_GPS

This library interfaces an arduino with the DFRobot DFRduino LEA-5H GPS shield using I2C. Latitude, Longitude, Altitude,  Ground Speed, Heading, and Vertical Velocity are all reported as doubles. Data validity is checked based on user supplied minimum horizontal and vertical accuracies (in meters).

## Contents

1. [Installing The Library](#installing-the-library)
   * [Linux](#linux)
     * [Command Line](#command-line)
     * [Download Zip](#download-zip)
   * [Mac](#mac)
   * [Windows](#windows)
1. [Using The Library](#using-the-library)
   * [Example Sketch](#example-sketch)
   * [Library Functions And Variables](#library-functions-and-variables)

## Installing The Library

### Linux

#### Command Line

1. Locate your Sketchbook Directory and the libraries Directory Inside
   * Usually your sketchbook is located in your home directory and called either Arduino or Sketchbook.
  
   * To find you sketchbook open the Arduino IDE, click File, then Preferences. On the Settings tab you should see your Sketchbook location.
  
   * Your sketchbook directory should have a libraries Directory inside. If not, make one. Note the path to the libraries Directory, i.e.:
     ```
     ~/Arduino/libraries
     ```
     or
     ```
     /home/<user name>/Arduino/libraries
     ```
  
1. Navigate to you libraries directory in a terminal. i.e.:

   ```
   cd ~/Arduino/libraries
   ```

1. Clone the repository:

   ```
   git clone https://github.com/lazerj7/DFRduino_GPS
   ```

1. That's it! You should now have a directory callled DFRduino_GPS in your libraries directory with all the library files inside.

#### Download Zip

1. Locate your Sketchbook Directory and the libraries Directory Inside
   * Usually your sketchbook is located in your home directory and called either Arduino or Sketchbook.
  
   * To find you sketchbook open the Arduino IDE, click File, then Preferences. On the Settings tab you should see your Sketchbook location.
  
   * Your sketchbook directory should have a libraries Directory inside. If not, make one. Note the path to the libraries Directory.
  
1. In the upper right of the repository screen on GitHub, click 'Clone or Download', and then click 'Download ZIP'

1. Extract the .zip file into your libraries Directory.

1. Done!

### Mac

1. Locate your Sketchbook Directory and the libraries Directory Inside

   * Usually your sketchbook is located in your Documents directory and called Arduino. It should have a libraries Directory inside. If not, make one.

   * To find you sketchbook open the Arduino IDE, click File, then Preferences. On the Settings tab you should see your Sketchbook location.
  
   * Your sketchbook directory should have a libraries Directory inside. If not, make one. Note the path to the libraries Directory.
  
1. In the upper right of the repository screen on GitHub, click 'Clone or Download', and then click 'Download ZIP'

1. Extract the .zip file into your libraries Directory.

1. Done!

### Windows

1. Locate your Sketchbook Directory and the libraries Directory Inside

   * Usually your sketchbook is located in your Documents directory and called Arduino. It should have a libraries Directory inside. If not, make one.

   * To find you sketchbook open the Arduino IDE, click File, then Preferences. On the Settings tab you should see your Sketchbook location.
  
   * Your sketchbook directory should have a libraries Directory inside. If not, make one. Note the path to the libraries Directory.
  
1. In the upper right of the repository screen on GitHub, click 'Clone or Download', and then click 'Download ZIP'

1. Extract the .zip file into your libraries Directory.

1. Done!

## Using The Library

### Example Sketch

 The Examples Folder contains a simple implementation of the library, which simply prints the GPS data over Serial. This sketch illustrates how to use the Library. The code is duplicated below:

  ```cpp
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
  ```

### Library Functions And Variables

  ```cpp
  DFRduino_gps <name>(double horizontal_accuracy, double vertical_accuracy);
  ```

Creates an instance of the GPS Shield. <name> is the name of the variable you will use to refer to the new instance. Takes two arguments, representing the minimum horizontal and vertical accuracies to be considered valid GPS data (in meters).
  
  ```cpp
  boolean update();
  ```
  
Updates GPS data. Returns true if data was updated, or false if data was not updated, either due to no new data being available or due to new data not meeting accuracy requirements. Takes no arguments. If no new data was available, previous data will remain if present and valid will remain true. If new data was present but did not meet accuracy requirements, previous data is destroyed and valid is false.
  
  ```cpp
  boolean valid;
  ```
  
  Boolean variable indicating whether data is valid. If data is valid, this is true. If data is invalid, this is false.
  
  ```cpp
  double latitude;
  double longitude;
  double altitude;
  double groundSpeed;
  double heading;
  double verticalVelocity;
  ```
  
  Double variables representing latitude, longitude, altitude, ground speed, heading, and vertical velocity respectively. Updated with update(). Validity of these variables indicated by valid.
