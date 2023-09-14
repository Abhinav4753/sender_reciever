// Include necessary libraries based on the Arduino board
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

// Define your network credentials
char ssid[] = "Hakeem";         // Your network SSID (name)
char pass[] = "Hussain@123";    // Your network password (for WPA) or key (for WEP)

int light = 2; // Define the pin for the LED

WiFiClient wifiClient;         // Initialize a WiFi client
MqttClient mqttClient(wifiClient); // Initialize an MQTT client

const char broker[] = "mqtt-dashboard.com"; // MQTT broker address
int port = 1883;               // MQTT broker port
const char topic[] = "Abhinav/Azeem";      // MQTT topic

void setup() {
  // Initialize serial communication and wait for port to open:
  Serial.begin(9600);
  pinMode(light, OUTPUT);

  // Wait for the serial port to connect (needed for native USB port only)
  while (!Serial) {
    ;
  }

  // Attempt to connect to the WiFi network:
  Serial.print("Connecting to WPA SSID: ");
  Serial.println(ssid);

  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // Connection failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("Connected to the network");
  Serial.println();

  // Attempt to connect to the MQTT broker:
  Serial.print("Connecting to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("Connected to the MQTT broker!");
  Serial.println();

  // Subscribe to a topic:
  Serial.print("Subscribed to topic: ");
  Serial.println(topic);
  Serial.println();

  mqttClient.subscribe(topic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(topic);
  Serial.println();
}

void loop() {
  int messageSize = mqttClient.parseMessage();
  if (messageSize) {
    // Received a message, print the topic and contents
    Serial.print("Received a message with topic '");
    Serial.print(mqttClient.messageTopic());
    Serial.print("', length ");
    Serial.print(messageSize);
    Serial.println(" bytes:");

    // Use the Stream interface to print the message contents
    while (mqttClient.available()) {
      Serial.print((char)mqttClient.read());
    }
    Serial.println();
    
    // Blink the LED to indicate a received message
    blinkLED();
    
    Serial.println();
  }
}

// Function to blink the LED
void blinkLED() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(light, HIGH);   // Turn the LED on
    delay(200);                   // Wait for a moment
    digitalWrite(light, LOW);    // Turn the LED off
    delay(200);                   // Wait for a moment
  }
}