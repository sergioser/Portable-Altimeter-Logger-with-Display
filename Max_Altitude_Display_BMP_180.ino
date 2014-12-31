// Finds and displays the maximum altitude on display (in FEET)

#include <SFE_BMP180.h>
#include <Wire.h>
#include "SevSeg.h"
#include <SevSeg.h>


unsigned long previousMillis = 0;  
int currentAltitude;
int maxAltitude = 0;
char tempStringMaximum[10];

SFE_BMP180 sensor;
SevSeg myDisplay;
double baseline; 

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);

  if(sensor.begin()){ // if connected succesfuly pin 13 LED will turn on
    Serial.println("Connected Succesfully to the BMP 180!");
    digitalWrite(13,HIGH); 
  }
  else 
    while(true); // DID NOT INITIALIZE RIGHT GREEN LIGHT IS OFF and LOOP FOREVER

  displaySetup(); //set up display
  delay(500);
  baseline = getPressure(); //get baseline pressure
  delay(500);
  currentAltitude = getAndRecordData(); // get first altitude measurement
}

void loop()
{
  // grab the current time in milliseconds 
  unsigned long currentMillis = millis();
  // convert the double to a string that the display can read
  sprintf(tempStringMaximum, "%4d", maxAltitude);
  // write the current maximum altitude to display
  myDisplay.DisplayString(tempStringMaximum,0);
  // grab the new altitude every 300 milliseconds 
  if(currentMillis - previousMillis > 300 ) { 
    //get the current altitude from the pressure sensor
    currentAltitude = getAndRecordData(); 
    // if the current altitude is grater than our previously recorded maximum, set the current altitude as the new maximum
    if(currentAltitude > maxAltitude){
      maxAltitude = currentAltitude;
    }
    // reset the timer
    previousMillis = currentMillis;
  }
}

int getAndRecordData(){ // returns altitude in feet
  double a,P;
  P = getPressure();
  // convert pressure to feet considering baseline
  a = (sensor.altitude(P,baseline) * 3.28084) ;
  Serial.print("Relative altitude ");
  Serial.print(a,1);
  Serial.println(" feet");
  int i = a + 0.5; // convert float to rounded out intiger
  return(i);    
}

double getPressure()
{
  char status;
  double T,P,p0,a;

  // You must first get a temperature measurement to perform a sensor reading.

  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = sensor.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:

    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Use '&T' to provide the address of T to the function.
    // Function returns 1 if successful, 0 if failure.

    status = sensor.getTemperature(T);
    if (status != 0)
    {
      // Start a sensor measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = sensor.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed sensor measurement:
        // Note that the measurement is stored in the variable P.
        // Use '&P' to provide the address of P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of sensor measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = sensor.getPressure(P,T);
        if (status != 0)
        {
          return(P);
        }
        else Serial.println("error retrieving sensor measurement\n");
      }
      else Serial.println("error starting sensor measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
}

void displaySetup(){
  int displayType = COMMON_CATHODE; //Your display is either common cathode or common anode

  //This pinout is for a bubble dispaly
  //Declare what pins are connected to the GND pins (cathodes)
  int digit1 = 8; //Pin 1
  int digit2 = 5; //Pin 10
  int digit3 = 11; //Pin 4
  int digit4 = 16; //Pin 6

    //Declare what pins are connected to the segments (anodes)
  int segA = 14; //Pin 12
  int segB = 6; //Pin 11
  int segC = 10; //Pin 3
  int segD = 3; //Pin 8
  int segE = 9; //Pin 2
  int segF = 4; //Pin 9
  int segG = 15; //Pin 7
  int segDP= 12; //Pin 5

  int numberOfDigits = 4; //Do you have a 1, 2 or 4 digit display?

  myDisplay.Begin(displayType, numberOfDigits, digit1, digit2, digit3, digit4, segA, segB, segC, segD, segE, segF, segG, segDP);

  myDisplay.SetBrightness(100); //Set the display to 100% brightness level

}








