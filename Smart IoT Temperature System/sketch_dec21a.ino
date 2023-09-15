/*

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  String warning;
  CloudTemperatureSensor temperature;
  int threshold;
  CloudRelativeHumidity humidity;
  bool led;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/
#include "DHT.h"
#include <Servo.h>

// Code generated by Arduino IoT Cloud, DO NOT EDIT.

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char DEVICE_LOGIN_NAME[]  = "80f844ff-6b99-4eaa-88b0-62af49d2ad00";

const char SSID[]               = "Wifi";    // Network SSID (name)
const char PASS[]               = "PAssword";    // Network password (use for WPA, or use as key for WEP)
const char DEVICE_KEY[]  = "0XM2HOIEETAEZOR2L1RV";    // Secret device password

void onWarningChange();
void onThresholdChange();
void onHumidityChange();
void onLedChange();

String warning;
CloudTemperatureSensor temperature;
int threshold;
CloudRelativeHumidity humidity;
bool led;

void initProperties(){

  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(warning, READWRITE, ON_CHANGE, onWarningChange);
  ArduinoCloud.addProperty(temperature, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(threshold, READWRITE, ON_CHANGE, onThresholdChange);
  ArduinoCloud.addProperty(humidity, READWRITE, ON_CHANGE, onHumidityChange);
  ArduinoCloud.addProperty(led, READWRITE, ON_CHANGE, onLedChange);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);


#define DHTPIN 14// Digital pin connected to the DHT sensor
// ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --

#define Warning_Buzzer 4
#define Warning_Led 5
#define Button 12

#define DHTTYPE DHT11   // Defining the DHT 11 Temp sensor

DHT dht(DHTPIN, DHTTYPE); //Declaring DHT variable

//   Local Variables used for our logic
float l = 20;
float room_temp = 0;
int ctrl_var = 0;

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {

  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);
  //Initializing the DHT sensor
  dht.begin();

  //Setting up the pins for either input or output
  pinMode(Warning_Buzzer, OUTPUT);
  pinMode(Warning_Led, OUTPUT);

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
  */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}



void loop() {
  ArduinoCloud.update();      //updating values to the Cloud using their own Thing Variables


  float t = dht.readTemperature();    //Saving temperature data from dht sensor

  //Saving room temperature
  if (ctrl_var == 3) {
    room_temp = t;
  }

  // Check if any reads failed and exit early (to try again).
  if (isnan(t) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
  }


  Serial.println(t);
  // Serial.println("test"); //Printing on Serial Monitor
  temperature = t;   //Uploading on Dashboard of ArduinoIotCloud
digitalWrite(Warning_Buzzer, HIGH);
     digitalWrite(Warning_Led, HIGH);

  //Main logic to upload warnings and control Buzzer and Led
  if (t >= l) {
     // servo code
     digitalWrite(Warning_Buzzer, HIGH);
     digitalWrite(Warning_Led, HIGH);

    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }
      // servo code end

    
    
    warning = "WARNING! Upper temperature limit reached! Get to a safe distance as mentioned before!";

  } else if ( t < l - 3 && t > l - 5 ) {
    digitalWrite(Warning_Buzzer, LOW);
    digitalWrite(Warning_Led, HIGH);
    warning = "Temp is high but within safe limit!";
    
  } else if (t - room_temp <= 0.5) {
    digitalWrite(Warning_Led, LOW);
  
    }

    ctrl_var++;

    delay(1000);


  }

  /*
    Since Limit is READ_WRITE variable, onLimitChange() is
    executed every time a new value is received from IoT Cloud.
  */


  /*
    Since Warn is READ_WRITE variable, onWarnChange() is
    executed every time a new value is received from IoT Cloud.
  */

  /*
    Since Threshold is READ_WRITE variable, onThresholdChange() is
    executed every time a new value is received from IoT Cloud.
  */
  void onThresholdChange()  {
    // Add your code here to act upon Threshold change
    l=threshold;
  }
  /*
    Since Humidity is READ_WRITE variable, onHumidityChange() is
    executed every time a new value is received from IoT Cloud.
  */
  void onHumidityChange()  {
    // Add your code here to act upon Humidity change
  }
  /*
    Since Led is READ_WRITE variable, onLedChange() is
    executed every time a new value is received from IoT Cloud.
  */
  void onLedChange()  {
    // Add your code here to act upon Led change
  }


  /*
    Since Warning is READ_WRITE variable, onWarningChange() is
    executed every time a new value is received from IoT Cloud.
  */
  void onWarningChange()  {
    // Add your code here to act upon Warning change

  }



