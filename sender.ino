#include <ArduinoMqttClient.h>
#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
#include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
#include <WiFi101.h>
#elif defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#endif

char ssid[] = "Hakeem";    // your network SSID (name)
char pass[] = "Hussain@123";    // your network password (use for WPA, or use as key for WEP)

int light = 2;
bool iswave = true; // Variable to control the light behavior

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "mqtt-dashboard.com";
int port = 1883;
const char topic[] = "Abhinav/Azeem";

const long interval = 1000;
unsigned long previousMillis = 0;

int count = 0;

const int trigPin = 2;
const int echoPin = 3;

float duration, distance;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  pinMode(light, OUTPUT);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  Serial.print("Subscribing to topic: ");
  Serial.println(topic);
  Serial.println();

  // subscribe to a topic
  mqttClient.subscribe(topic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(topic);
  Serial.println();
}

void loop() {
  // call poll() regularly to allow the library to send MQTT keep-alives
  mqttClient.poll();

  // to avoid having delays in loop, we'll use the strategy from BlinkWithoutDelay
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = (duration * 0.0343) / 2;
    Serial.print("Distance: ");
    Serial.println(distance);

    if (distance < 12) {
      // send message, the Print interface can be used to set the message contents
      mqttClient.beginMessage(topic);
      mqttClient.print("Abhinav: Wave is detected, ");
      mqttClient.print("Distance: ");
      mqttClient.print(distance);
      mqttClient.endMessage();
      delay(1000);

      // Set the iswave flag to true when a wave is detected
      iswave = true;
    } else {
      // Set the iswave flag to false when no wave is detected
      iswave = false;
    }

    Serial.println();

    count++;
  }
}
  void handleLED(bool isWave) {
  if (isWave) {
    // Continuous blinking for waves
    digitalWrite(light, HIGH);
    delay(700);
    digitalWrite(light, LOW);
    delay(800);
    digitalWrite(light, HIGH);
    delay(700);
    digitalWrite(light, LOW);
    delay(800);
    digitalWrite(light, HIGH);
    delay(700);
    digitalWrite(light, LOW);
    delay(800);
  } else {
    // Blink the LED once for 3 seconds when patted
    digitalWrite(light, HIGH);
    delay(3000);
    digitalWrite(light, LOW);
  }
}