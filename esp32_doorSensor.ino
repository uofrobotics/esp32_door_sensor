#define DOOR_SENSOR_PIN  19  // ESP32 pin GPIO19 connected to door sensor's pin
#define LED_BUILTIN  2 // Pin for the LED on the chip
#include <WiFi.h>
#include <WiFiUdp.h>
#include <secrets.h>

const char* ssid = "uofrGuest"; // Connect to wifi using uofrGuest
const char* password = ""; // Replace with your Wi-Fi password

const char* udpAddress = RASPBERRY_PI_ADDRESS; // Replace with the Raspberry Pi's IP address
const int udpPort = 5005; // Port to send packets over

WiFiUDP Udp;

int doorState;
int lastState;

void setup() {
  Serial.begin(9600); // initialize serial
  // initialize the LED and door sensor pins
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(DOOR_SENSOR_PIN, INPUT_PULLUP);
  
  // connect to uofrGuest
  WiFi.begin(ssid, password);
  // Update the console with the connection status until connected
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
  }

  // successfully connected
  Serial.println("Connected to WiFi");
  Udp.begin(udpPort);
}

void loop() {
  // read the state of the door sensor
  // this is a 0 for closed and a 1 for open
  doorState = digitalRead(DOOR_SENSOR_PIN);
  // if the state of the door sensor is the same as last loop, exit out
  // this line ensures that a packet is only sent once the state CHANGES
  if(doorState == lastState) return;
  // print the state of the door to the console
  Serial.println(doorState);
  // Change the LED based on the status of the sensor
  if (doorState == LOW){
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
  }

  // Send the packet over UDP
  Udp.beginPacket(udpAddress, udpPort);
  Udp.write(doorState);
  Udp.endPacket();
  Serial.println("Packet sent");

  // Change the last state to the current state for the nect loop to use
  lastState = doorState;
}