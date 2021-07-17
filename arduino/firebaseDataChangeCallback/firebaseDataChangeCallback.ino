#include "WiFi.h"
#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"
// Device ID
#define DEVICE_UID "1X"


// Firebase Realtime Database Object
FirebaseData fbdo;
// Firebase Authentication Object
FirebaseAuth auth;
// Firebase configuration Object
FirebaseConfig config;
// Firebase database path
String databasePath = "";
//Define Firebase Data object
FirebaseData stream;



// Firebase Unique Identifier
String fuid = "";

// Stores the elapsed time from device start up
unsigned long elapsedMillis = 0;



// Store device authentication status
bool isAuthenticated = false;


// Pins
static const int led_pin_1 = 2;
static const int led_pin_2 = 12;
static const int usb_pin = 25; //USB

const char *ssid = "CAKE";
const char *password = "0891300633";
// Your Firebase Project Web API Key
#define API_KEY "AIzaSyCG59xubOTJe2gMqg1hz2NmcLvY_0AK-_M"
// Your Firebase Realtime database URL
#define DATABASE_URL "https://esp32-b74d2-default-rtdb.asia-southeast1.firebasedatabase.app/"
int count = 0;



void streamCallback(FirebaseStream data)
{
  Serial.printf("sream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\n\n",
                data.streamPath().c_str(),
                data.dataPath().c_str(),
                data.dataType().c_str(),
                data.eventType().c_str());
  printResult(data); //see addons/RTDBHelper.h
  Serial.println();

  if(data.intData() == 1) {
    digitalWrite(usb_pin,LOW);
  } else if(data.intData() == 0){
    digitalWrite(usb_pin, HIGH);
  }
}

void streamTimeoutCallback(bool timeout)
{
  if (timeout)
    Serial.println("stream timeout, resuming...\n");
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

  if (!Firebase.RTDB.beginStream(&stream, "/status"))
    Serial.printf("sream begin error, %s\n\n", stream.errorReason().c_str());

  Firebase.RTDB.setStreamCallback(&stream, streamCallback, streamTimeoutCallback);
}





void setup() {

  // Configure pin
  pinMode(led_pin_1, OUTPUT);
  pinMode(led_pin_2, OUTPUT);
  pinMode(usb_pin, OUTPUT);

  digitalWrite(usb_pin, HIGH);

  Serial.begin(115200);


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

}
