#include <mcp_can.h>
#include <SPI.h>

MCP_CAN CAN0(10);     // Set CS to pin 10

// defines pins numbers
const int trigPin = 7;
const int echoPin = 6;
const int ledPin = 8;
// defines variables
long duration;
int distance;
void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(ledPin, OUTPUT);
  CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ);
  CAN0.setMode(MCP_NORMAL);
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

byte data[1] = {0x00};

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;

  data[0] = distance;
  byte sndStat = CAN0.sendMsgBuf(0x100, 0, 8, data);
  Serial.print("The distance is ");
  Serial.println(distance);
  delay(2000);
}
