
#include "WiFi.h"
#include <Arduino.h>
#include <FirebaseESP32.h>
// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"
// Device ID
#define DEVICE_UID "1X"
// Your Firebase Project Web API Key
#define API_KEY "AIzaSyARWZzmjIfRLHXU-C5xXONkqSArJCBmDxQ"
// Your Firebase Realtime database URL
#define DATABASE_URL "https://esp32-92e64-default-rtdb.asia-southeast1.firebasedatabase.app"

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



// The frequency of sensor updates to firebase, set to 10seconds
unsigned long update_interval = 10000;

// LED rates
static const int rate_1 = 100; // ms
static const int rate_2 = 323; // ms

// Pins
static const int led_pin_1 = 2;
static const int led_pin_2 = 12;

const char *ssid = "CAKE";
const char *password = "0891300633";



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
  // Do nothing
  // setup() and loop() run in their own task with priority 1 in core 1
  // on ESP32
    // Check that 10 seconds has elapsed before, device is authenticated and the
    // firebase service is ready.
    if (millis() - elapsedMillis > update_interval && isAuthenticated &&
        Firebase.ready()) {
      elapsedMillis = millis();
      Serial.println("------------------------------------");
      Serial.println("Set int test...");
      // Specify the key value for our data and append it to our path
      String node = "/value";
      // Send the value our count to the firebase realtime database
      if (Firebase.set(fbdo, node.c_str(), count++)) {
        // Print firebase server response
        Serial.println("PASSED");
        Serial.println("PATH: " + fbdo.dataPath());
        Serial.println("TYPE: " + fbdo.dataType());
        Serial.println("ETag: " + fbdo.ETag());
        Serial.print("VALUE: ");
        printResult(fbdo); // see addons/RTDBHelper.h
        Serial.println("------------------------------------");
        Serial.println();
      } else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
      }
    }
}
