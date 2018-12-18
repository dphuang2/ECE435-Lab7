#include <Ethernet.h>
#include <EthernetUdp.h>
#include <mcp_can.h>
#include <SPI.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 1, 177);
IPAddress dest(192, 168, 1, 176);
unsigned int port = 8888;      

MCP_CAN CAN0(10);

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {
  CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_16MHZ);     // init CAN Bus with 250kb/s baudrate at 16MHz with Mask & Filters Disabled
  CAN0.setMode(MCP_NORMAL);                        // Set operation mode to normal so the MCP2515 sends acks to received data.
  pinMode(2, INPUT);                               // Setting pin 2, MCP2515 /INT, to input mode
  Ethernet.begin(mac,ip);                          // Initialize Ethernet
  Udp.begin(port);                            // Initialize the UDP listen port that is currently unused!
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  /*if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK) 
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");*/
  
  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }
}

byte packetData[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
byte canData[8];
char canBuffer[50];

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);

    // send data:  ID = 0x100, Standard CAN Frame, Data length = 8 bytes, 'data' = array of data bytes to send
    byte sndStat = CAN0.sendMsgBuf(0x100, 0, 8, packetBuffer);
    /*if(sndStat == CAN_OK){
      Serial.println("Message Sent Successfully!");
    } else {
      Serial.println("Error Sending Message...");
    }*/
  }

  if(!digitalRead(2))                            // If pin 2 is low, read receive buffer
    {
      unsigned long rxId;
      byte len = 0;
      CAN0.readMsgBuf(&rxId, &len, canData);         // Read Data: rxID = Message ID, len = Data Length, buf = Data Byte(s)

      sprintf(canBuffer, "ID: %.8lX  Data: %.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X\n\r",
              rxId, canData[0], canData[1], canData[2], canData[3], canData[4], canData[5], canData[6], canData[7]);

      Udp.beginPacket(dest, port);
      Udp.write(canBuffer);
      Udp.endPacket();
    }
  delay(10);
}
