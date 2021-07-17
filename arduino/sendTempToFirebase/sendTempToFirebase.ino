
#include "WiFi.h"
#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

#include <Wire.h>
#define LM73_ADDR 0x4D


// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"
// Device ID
#define DEVICE_UID "1X"
// Your Firebase Project Web API Key
#define API_KEY "AIzaSyCG59xubOTJe2gMqg1hz2NmcLvY_0AK-_M"
// Your Firebase Realtime database URL
#define DATABASE_URL "https://esp32-b74d2-default-rtdb.asia-southeast1.firebasedatabase.app/"

// Firebase Realtime Database Object
FirebaseData fbdo;
// Firebase Authentication Object
FirebaseAuth auth;
// Firebase configuration Object
FirebaseConfig config;
// Firebase database path
String databasePath = "";



// Firebase Unique Identifier
String fuid = "";

// Stores the elapsed time from device start up
unsigned long elapsedMillis = 0;

// Dummy counter to test initial firebase updates
int count = 0; 

// Store device authentication status
bool isAuthenticated = false;

unsigned long sendDataPrevMillis = 0;


// The frequency of sensor updates to firebase, set to 10seconds
unsigned long update_interval = 10000;


// Pins
static const int led_pin_1 = 2;
static const int led_pin_2 = 12;

const char *ssid = "CAKE";
const char *password = "0891300633";



int analog_value = 0;
double temp=0;

float readTemperature() {
  Wire1.beginTransmission(LM73_ADDR);
  Wire1.write(0x00); // Temperature Data Register
  Wire1.endTransmission();
 
  uint8_t count = Wire1.requestFrom(LM73_ADDR, 2);
  float temp = 0.0;
  if (count == 2) {
    byte buff[2];
    buff[0] = Wire1.read();
    buff[1] = Wire1.read();
    temp += (int)(buff[0]<<1);
    if (buff[1]&0b10000000) temp += 1.0;
    if (buff[1]&0b01000000) temp += 0.5;
    if (buff[1]&0b00100000) temp += 0.25;
    if (buff[0]&0b10000000) temp *= -1.0;
  }
  return temp;
}


void firebase_init() {
  // configure firebase API Key
  config.api_key = API_KEY;
  // configure firebase realtime database url
  config.database_url = DATABASE_URL;
  // Enable WiFi reconnection
  Firebase.reconnectWiFi(true);
  Serial.println("------------------------------------");
  Serial.println("Sign up new user...");
  // Sign in to firebase Anonymously
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Success");
    isAuthenticated = true;
    // Set the database path where updates will be loaded for this device
    databasePath = "/";
    fuid = auth.token.uid.c_str();
  } else {
    Serial.printf("Failed, %s\n", config.signer.signupError.message.c_str());
    isAuthenticated = false;
  }
  // Assign the callback function for the long running token generation task,
  // see addons/TokenHelper.h
  config.token_status_callback = tokenStatusCallback;
  // Initialise the firebase library
  Firebase.begin(&config, &auth);
}





void setup() {

  // Configure pin
  pinMode(led_pin_1, OUTPUT);
  pinMode(led_pin_2, OUTPUT);

  Serial.begin(115200);
  Wire1.begin(4, 5);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(led_pin_1, HIGH);
    delay(500);
    digitalWrite(led_pin_1, LOW);
    delay(500);
  }
  digitalWrite(led_pin_1, LOW);
  Serial.println("Connected to the WiFi network");
  firebase_init();

 

}

void loop() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > update_interval || sendDataPrevMillis == 0))
  {
    Serial.println(readTemperature());
    sendDataPrevMillis = millis();

    Serial.printf("Set int... %s\n", Firebase.RTDB.setFloat(&fbdo, "/sensor/temp", readTemperature()) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get int... %s\n", Firebase.RTDB.getFloat(&fbdo, "/sensor/temp") ? String(fbdo.intData()).c_str() : fbdo.errorReason().c_str());

 
  } 
}
