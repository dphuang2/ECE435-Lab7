#include <Ethernet.h>
#include <EthernetUdp.h>
#include <mcp_can.h>
#include <SPI.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 1, 177);
IPAddress dest(192, 168, 1, 176);
unsigned int port = 8888;
EthernetUDP Udp;

long unsigned int rxId;
unsigned char len = 0;
byte recv[8];
char msgString[128];                        // Array to store serial string

MCP_CAN CAN0(10);                               // Set CS to pin 10

void setup()
{
  Ethernet.begin(mac,ip);                          // Initialize Ethernet
  Udp.begin(port);    
  Serial.begin(9600);
  
  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  
  CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.
  
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

void loop()
{
    if(!digitalRead(2)){ 
    CAN0.readMsgBuf(&rxId, &len, recv);      // Read data: len = data length, buf = data byte(s)
    Serial.println(recv[0]);

    Udp.beginPacket(dest, port);
    //char ReplyBuffer[] = "acknowledged";     
    Udp.write(recv[0]);
    Udp.endPacket();
    }
    delay(1000);
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
