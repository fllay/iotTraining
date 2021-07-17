#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

#define LM73_ADDR 0x4D


// Pins
static const int led_pin_1 = 2; //WiFI
static const int led_pin_2 = 12; //IoT
static const int usb_pin = 25; //USB

const char* ssid = "CAKE";
const char* password = "0891300633";
const char* mqtt_server = "203.150.243.118";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

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

void setup_wifi() {

    delay(10);
    Wire1.begin(4, 5);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      digitalWrite(led_pin_1, HIGH);
      digitalWrite(led_pin_2, HIGH);
      delay(500);
      digitalWrite(led_pin_2, LOW);
      digitalWrite(led_pin_1, LOW);
      delay(500);


    }
  
    digitalWrite(led_pin_1, LOW);
    Serial.println("Connected to the WiFi network");
    
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
    
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    if(strcmp((char *) topic, "/usb") == 0){
      payload[length] = '\0';
      String s = String((char*)payload);
      int f = s.toInt();
      for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
      } 
      if(f == 1){
        digitalWrite(usb_pin, LOW);
        Serial.print("1");
      }else {
        digitalWrite(usb_pin, HIGH);
        Serial.print("0");
      } 
    }
    Serial.println();
}

void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("ESP32Client")) {
            Serial.println("connected");
            // Once connected, publish an announcement...
            client.publish("/temp", 0);
            // ... and resubscribe
            client.subscribe("/usb");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void setup() {
      // Configure pin
    pinMode(led_pin_1, OUTPUT);
    pinMode(led_pin_2, OUTPUT);
    pinMode(usb_pin, OUTPUT);

    digitalWrite(usb_pin, HIGH);
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);

    if (!client.connected()) {

      digitalWrite(led_pin_2, HIGH);
      delay(500);
      digitalWrite(led_pin_2, LOW);
      delay(500);
     }

    digitalWrite(led_pin_2, LOW);
    client.setCallback(callback);
}


void loop() {

    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    long now = millis();
    if (now - lastMsg > 2000) {
        lastMsg = now;
        ++value;
        //snprintf (msg, 75,  value);
        sprintf(msg,"%2.2f",readTemperature());
        //Serial.print("Publish message: ");
        //Serial.println(msg);
        client.publish("/temp", msg);
    }
}
