/**
 * Created by K. Suwatchai (Mobizt)
 * 
 * Email: k_suwatchai@hotmail.com
 * 
 * Github: https://github.com/mobizt
 * 
 * Copyright (c) 2021 mobizt
 *
*/

//This example shows how to send JSON payload FCM to a recipient via legacy API (requires server key).

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>



#define WIFI_SSID "CAKE"
#define WIFI_PASSWORD "0891300633"

#define FIREBASE_FCM_SERVER_KEY "AAAAufycENY:APA91bFGgZLqcWx2x43nfmE611l6mv1fJkKizw0hq69jJUSfXU3n3oFaP50aWnOWHvirv4zN1RqE6neQ93qEbnkdZ4dBZO4pKi7VSUobd-ujGTOWMGgoElCg1TKyrlACWcm6-5tyqFaW"
#define DEVICE_REGISTRATION_ID_TOKEN "dqVtYXkXz0SzCVJMHUK9wn:APA91bFB-8yfzTeilssXqA9WODPMNH17CM84qP9HzX45YZR-G-Q4-o0hI3iJqtKqY8548HA2gAG04C29nCy4uJ3fgSL735PSGTZcQBBoCQGeVX6c0rNs1UilsafuBGNCsXK7YFVTQPL4"




//Define Firebase Data object
FirebaseData fbdo;

unsigned long lastTime = 0;

int count = 0;

void sendMessage();

void setup()
{

    Serial.begin(115200);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    //required for legacy HTTP API
    Firebase.FCM.setServerKey(FIREBASE_FCM_SERVER_KEY);

    Firebase.reconnectWiFi(true);

    sendMessage();
}

void loop()
{

    if (millis() - lastTime > 60 * 1000)
    {
        lastTime = millis();

        sendMessage();
    }
}

void sendMessage()
{

    Serial.print("Send Firebase Cloud Messaging... ");

    //Read more details about legacy HTTP API here https://firebase.google.com/docs/cloud-messaging/http-server-ref
    FCM_Legacy_HTTP_Message msg;

    msg.targets.to = DEVICE_REGISTRATION_ID_TOKEN;

    msg.options.time_to_live = "1000";
    msg.options.priority = "high";

    msg.payloads.notification.title = "Notification title";
    msg.payloads.notification.body = "Notification body";
    msg.payloads.notification.icon = "myicon";
    msg.payloads.notification.click_action = "OPEN_ACTIVITY_1";

    FirebaseJson json;
    String payload;

    //all data key-values should be string
    json.add("temp", "28");
    json.add("unit", "celsius");
    json.add("timestamp", "1609815454");
    json.toString(payload);
    msg.payloads.data = payload.c_str();

    if (Firebase.FCM.send(&fbdo, &msg)) //send message to recipient
        Serial.printf("ok\n%s\n\n", Firebase.FCM.payload(&fbdo).c_str());
    else
        Serial.println(fbdo.errorReason());

    count++;
}
