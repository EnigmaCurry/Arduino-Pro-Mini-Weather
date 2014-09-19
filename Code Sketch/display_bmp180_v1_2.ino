#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_PCD8544.h"
#include <SFE_BMP180.h>
#include <Wire.h>
#include "LowPower.h"
#include "DHT.h"
 
//GFX, DHT & PCD8544 can be found in adafruits github.
//SFE_BMP180 can be found on Sparkfun's github.


// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

SFE_BMP180 pressure;

//#define ALTITUDE 15.0 //  Altitude at location
#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup()   
{

Serial.begin(9600);

  // Initalize the BMP180 Sensor
  if (pressure.begin())

    dht.begin();
    display.begin();
  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(57);
  display.clearDisplay();   // clears the screen and buffer

}


void loop() {

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  int h = dht.readHumidity();
  // Read temperature as Celsius
  int t = dht.readTemperature();
  // Read temperature as Fahrenheit
  int f = dht.readTemperature(true);
  
  // Compute heat index
  // Must send in temp in Fahrenheit!
  int hi = dht.computeHeatIndex(f, h);

  
  //  Display Names On Left Side Of Display And Horizontal Lines
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("T");
  display.setCursor(6,3);
  display.print("E");
  display.setCursor(12,5);
  display.print("M");
  display.setCursor(18,7);
  display.print("P");
  display.setCursor(0,18);
  display.print("P");
  display.setCursor(6,20);
  display.print("R");
  display.setCursor(12,22);
  display.print("E");
  display.setCursor(18,24);
  display.print("S");
  display.setCursor(0,35);
  display.print("H");
  display.setCursor(7,37);
  display.print("U");
  display.setCursor(14,39);
  display.print("M");
  display.drawLine(0,15,84,15,1);
  display.drawLine(0,32,84,32,1);

  
  char status;
  double T,P,p0,a;

  // Loop here getting pressure readings every 10 seconds.
  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {

      // Print out the measurement:
      display.setCursor(24,0);
      display.setTextSize(2);
      display.print((9.0/5.0)*T+32.0,0);
      display.print("'F");

      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          display.setCursor(24,17);
          display.print(P*0.0295333727,2);
          display.setCursor(24,34); 
          display.print(h+2);
          display.print("%");
          display.setTextSize(1);
          display.setCursor(60,34);
          display.print("H.I.");
          display.setCursor(60,41);
          display.print(hi);
          display.print("F");
          display.display();
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");

    // sleep mode code

    // Enter power down state for 8 s with ADC and BOD module disabled
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  

}




