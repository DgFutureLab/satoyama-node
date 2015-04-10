/*************************************************** 
 * This is an example for the Adafruit CC3000 Wifi Breakout & Shield
 * 
 * Designed specifically to work with the Adafruit WiFi products:
 * ----> https://www.adafruit.com/products/1469
 * 
 * Adafruit invests time and resources providing this open source code, 
 * please support Adafruit and open-source hardware by purchasing 
 * products from Adafruit!
 * 
 * Written by Kevin Townsend & Limor Fried for Adafruit Industries.  
 * BSD license, all text above must be included in any redistribution
 ****************************************************/

/*

 This example does a full test of core connectivity:
 * Initialization
 * SSID Scan
 * AP connection
 * DHCP printout
 * DNS lookup
 * Ping
 * Disconnect
 It's a good idea to run this sketch when first setting up the
 module.
 
 */

#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"

//#include <WiFi.h>

// satoyama-chibi-lib includes
// Defines pin numbers for sensors and also a simple format to send
// information to the satoyama edge router
#include <satoyama_config.h>
#include <utilsawesome.h>
#include <chibi.h>
#include <config.h>

// JSON compression
#include <ArduinoJson.h>

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
SPI_CLOCK_DIVIDER); // you can change this clock speed but DI

#define WLAN_SSID       "HWD15_F49FF3ED0D6D"        // cannot be longer than 32 characters!
#define WLAN_PASS       "10ii3ybg21f8317"
#define WLAN_SECURITY   WLAN_SEC_WPA

//#define WLAN_SSID       "iPhone"        // cannot be longer than 32 characters!
//#define WLAN_PASS       "h9ax8ng0o0tv"
//// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
//#define WLAN_SECURITY   WLAN_SEC_WPA2

// Chibi Parameters
//uint8_t tx_buf[TX_LENGTH];
int src_addr;

// JSON
StaticJsonBuffer<150> jsonBuffer;
char jbuffer[150];
// JsonObject& root = jsonBuffer.createObject();
JsonArray& data = jsonBuffer.createArray();
JsonObject& jtemp = data.createNestedObject();
JsonObject& jhumid = data.createNestedObject();  
//JsonObject& jsonar = data.createNestedObject(); 
//JsonObject& jbatt = data.createNestedObject();  


// WiFi Parameters
uint32_t ip;
//#define WEBSITE "satoyamacloud.com"
//#define WEBPAGE "/node/44"
//#define WEBSITE      "www.adafruit.com"
//#define WEBPAGE      "/testwifi/index.html"
#define WEBSITE "128.199.120.30"
#define WEBPAGE "/readings"

#define IDLE_TIMEOUT_MS  5000


/**************************************************************************/
/*!
 @brief  Sets up the HW and the CC3000 module (called automatically
 on startup)
 */
/**************************************************************************/
void setup(void)
{

  Serial.begin(115200);

  // Chibi initialization
  chibiInit();
  chibiSetShortAddr(AGGREGATOR_SHORT_ADDRESS);


  Serial.println(F("Hello, CC3000!\n")); 

  //  displayDriverMode();
  //  Serial.print("Free RAM: "); 
  //  Serial.println(getFreeRam(), DEC);

  /* Initialise the module */
  //  Serial.println(F("\nInitialising the CC3000 ..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Unable to initialise the CC3000! Check your wiring?"));
    while(1);
  }

  /* Optional: Update the Mac Address to a known value */
  /*
  uint8_t macAddress[6] = { 0x08, 0x00, 0x28, 0x01, 0x79, 0xB7 };
   if (!cc3000.setMacAddress(macAddress))
   {
   Serial.println(F("Failed trying to update the MAC address"));
   while(1);
   }
   */

  uint16_t firmware = checkFirmwareVersion();
  if (firmware < 0x113) {
    Serial.println(F("Wrong firmware version!"));
    for(;;);
  } 

  //  displayMACAddress();

  /* Optional: Get the SSID list (not available in 'tiny' mode) */
  //#ifndef CC3000_TINY_DRIVER
  //  listSSIDResults();
  //#endif

  /* Delete any old connection data on the module */
  //  Serial.println(F("\nDeleting old connection profiles"));
  if (!cc3000.deleteProfiles()) {
    Serial.println(F("Failed!"));
    while(1);
  }

  /* Optional: Set a static IP address instead of using DHCP.
   Note that the setStaticIPAddress function will save its state
   in the CC3000's internal non-volatile memory and the details
   will be used the next time the CC3000 connects to a network.
   This means you only need to call the function once and the
   CC3000 will remember the connection details.  To switch back
   to using DHCP, call the setDHCP() function (again only needs
   to be called once).
   */
  /*
  uint32_t ipAddress = cc3000.IP2U32(192, 168, 1, 19);
   uint32_t netMask = cc3000.IP2U32(255, 255, 255, 0);
   uint32_t defaultGateway = cc3000.IP2U32(192, 168, 1, 1);
   uint32_t dns = cc3000.IP2U32(8, 8, 4, 4);
   if (!cc3000.setStaticIPAddress(ipAddress, netMask, defaultGateway, dns)) {
   Serial.println(F("Failed to set static IP!"));
   while(1);
   }
   */
  /* Optional: Revert back from static IP addres to use DHCP.
   See note for setStaticIPAddress above, this only needs to be
   called once and will be remembered afterwards by the CC3000.
   */
  /*
  if (!cc3000.setDHCP()) {
   Serial.println(F("Failed to set DHCP!"));
   while(1);
   }
   */


}

void loop(void)
{

  /* Attempt to connect to an access point */
  char *ssid = WLAN_SSID;             /* Max 32 chars */
  //  Serial.print(F("\nAttempting to connect to ")); 
  Serial.println(ssid);

  /* NOTE: Secure connections are not available in 'Tiny' mode!
   By default connectToAP will retry indefinitely, however you can pass an
   optional maximum number of retries (greater than zero) as the fourth parameter.
   
   ALSO NOTE: By default connectToAP will retry forever until it can connect to
   the access point.  This means if the access point doesn't exist the call
   will _never_ return!  You can however put in an optional maximum retry count
   by passing a 4th parameter to the connectToAP function below.  This should
   be a number of retries to make before giving up, for example 5 would retry
   5 times and then fail if a connection couldn't be made.
   */
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }

  Serial.println(F("Connected!"));

  /* Wait for DHCP to complete */
  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }  

  /* Display the IP address DNS, Gateway, etc. */
  //  while (! displayConnectionDetails()) {
  //    delay(1000);
  //  }

  //////// For PING  
  //#ifndef CC3000_TINY_DRIVER
  //  /* Try looking up satoyamacloud.com */
  //  uint32_t ip = 0;
  //  Serial.print(F("satoyamacloud.com -> "));
  //  while  (ip  ==  0)  {
  //    if  (!  cc3000.getHostByName("satoyamacloud.com", &ip))  {
  //      Serial.println(F("Couldn't resolve!"));
  //    }
  //    delay(500);
  //  }  
  //  cc3000.printIPdotsRev(ip);
  //  
  //  /* Do a quick ping test on adafruit.com */  
  //  Serial.print(F("\n\rPinging ")); cc3000.printIPdotsRev(ip); Serial.print("...");  
  //  uint8_t replies = cc3000.ping(ip, 5);
  //  Serial.print(replies); Serial.println(F(" replies"));
  //  if (replies)
  //    Serial.println(F("Ping successful!"));
  //#endif

  // Read sensors and POST
  //  src_addr = chibiGetSrcAddr();
  src_addr = 44;
  //  char addr[2];
  //  itoa(src_addr, addr, 10);
  // Get a reading
  read_sensors();
  //  Serial.println((char*) tx_buf);

  //    getData();
  postData();
  //  getData();

  /* You need to make sure to clean up after yourself or the CC3000 can freak out */
  /* the next time you try to connect ... */
  Serial.println(F("\n\nClosing the connection"));
  cc3000.disconnect();

  // Clear buffer
  //  free(tx_buf);
  free(jbuffer);
  jbuffer[0] = (char)0;


  delay(10000);
}

/**************************************************************************/
/*!
 @brief  Displays the driver mode (tiny of normal), and the buffer
 size if tiny mode is not being used
 
 @note   The buffer size and driver mode are defined in cc3000_common.h
 */
/**************************************************************************/
void displayDriverMode(void)
{
#ifdef CC3000_TINY_DRIVER
  Serial.println(F("CC3000 is configure in 'Tiny' mode"));
#else
  Serial.print(F("RX Buffer : "));
  Serial.print(CC3000_RX_BUFFER_SIZE);
  Serial.println(F(" bytes"));
  Serial.print(F("TX Buffer : "));
  Serial.print(CC3000_TX_BUFFER_SIZE);
  Serial.println(F(" bytes"));
#endif
}

/**************************************************************************/
/*!
 @brief  Tries to read the CC3000's internal firmware patch ID
 */
/**************************************************************************/
uint16_t checkFirmwareVersion(void)
{
  uint8_t major, minor;
  uint16_t version;

#ifndef CC3000_TINY_DRIVER  
  if(!cc3000.getFirmwareVersion(&major, &minor))
  {
    Serial.println(F("Unable to retrieve the firmware version!\r\n"));
    version = 0;
  }
  else
  {
    Serial.print(F("Firmware V. : "));
    Serial.print(major); 
    Serial.print(F(".")); 
    Serial.println(minor);
    version = major; 
    version <<= 8; 
    version |= minor;
  }
#endif
  return version;
}

/**************************************************************************/
/*!
 @brief  Tries to read the 6-byte MAC address of the CC3000 module
 */
/**************************************************************************/
void displayMACAddress(void)
{
  uint8_t macAddress[6];

  if(!cc3000.getMacAddress(macAddress))
  {
    Serial.println(F("Unable to retrieve MAC Address!\r\n"));
  }
  else
  {
    Serial.print(F("MAC Address : "));
    cc3000.printHex((byte*)&macAddress, 6);
  }
}


/**************************************************************************/
/*!
 @brief  Tries to read the IP address and other connection details
 */
/**************************************************************************/
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;

  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); 
    cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); 
    cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); 
    cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); 
    cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); 
    cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}

/**************************************************************************/
/*!
 @brief  Begins an SSID scan and prints out all the visible networks
 */
/**************************************************************************/

void listSSIDResults(void)
{
  uint32_t index;
  uint8_t valid, rssi, sec;
  char ssidname[33]; 

  if (!cc3000.startSSIDscan(&index)) {
    Serial.println(F("SSID scan failed!"));
    return;
  }

  Serial.print(F("Networks found: ")); 
  Serial.println(index);
  Serial.println(F("================================================"));

  while (index) {
    index--;

    valid = cc3000.getNextSSID(&rssi, &sec, ssidname);

    Serial.print(F("SSID Name    : ")); 
    Serial.print(ssidname);
    Serial.println();
    Serial.print(F("RSSI         : "));
    Serial.println(rssi);
    Serial.print(F("Security Mode: "));
    Serial.println(sec);
    Serial.println();
  }
  Serial.println(F("================================================"));

  cc3000.stopSSIDscan();
}

void read_sensors(){
  Serial.println("Reading sensors...");

  //memset(tx_buf, 0, TX_LENGTH);       
  //Serial.println("Temp done");
  //long duration, inches, cm;

  //  char str[80];
  //  strcpy(str, "");  
  //  strcat(str, "^");
  //  strcat(str,"44");
  //  strcat(str, "@");
  //  strcat((char *)tx_buf, (char *)str);

  //  // Read temperature
  //  float temperature = DHT.temperature;  
  //  if (temperature > 0) {
  //  JsonObject& jtemp = data.createNestedObject();
  float temperature = 25;  
  //  Reading temp = {
  //    "temperature", temperature, millis()          };
  //  add_to_tx_buf_new(tx_buf, &temp);
  jtemp["alias"] = "temperature";
  jtemp["value"] = temperature;
  jtemp["timestamp"] = millis();

  Serial.println("Temp done");
  //  }
  //
  //  // Read humidity
  //  float humidity = DHT.humidity;
  //  if (humidity > 0) {
//    JsonObject& jhumid = data.createNestedObject();  
//    float humidity = 50;
//  //  //  Reading hum = {
//  //  //    "humidity", humidity , millis()          };
//  //  //  add_to_tx_buf_new(tx_buf, &hum);
//    jhumid["alias"] = "humidity";
//    jhumid["value"] = humidity;
//    jhumid["timestamp"] = millis();
//    Serial.println("Humid done");
  //  }
  //
  //  // Read sonar distance
  //  float distance = sonar.ping() / US_ROUNDTRIP_CM; 
  //  
  //  if (distance > 0) {
  //  JsonObject& jsonar = data.createNestedObject();  
  //  float distance = 30;
  //  //  Reading dist = {
  //  //    "distance", distance, millis()          };
  //  //  add_to_tx_buf_new(tx_buf, &dist);
  //  jsonar["alias"] = "distance";
  //  jsonar["value"] = distance;
  //  jsonar["timestamp"] = millis();
  //  Serial.println("Sonar done");
  //  //  }
  //  //
  //  //  // Read battery voltage
  //  //  float vbat = read_vbat();
  //  JsonObject& jbatt = data.createNestedObject();  
  //  float vbat = 3.3;
  //  //  Reading battery_voltage = {
  //  //    "vbat", vbat, millis()          };
  //  //  add_to_tx_buf_new(tx_buf, &battery_voltage);
  //  jbatt["alias"] = "vbat";
  //  jbatt["value"] = vbat;
  //  jbatt["timestamp"] = millis();
  //  Serial.println("Battery done");
  // Debug print

  //    char jbuffer[256];
  data.printTo(jbuffer, sizeof(jbuffer));

  Serial.println(jbuffer);

  //  Serial.println("Transmitting data...");
  //Send data stored on "tx_buf" to aggregator (Satoyama edge router)
  //  chibiTx(AGGREGATOR_SHORT_ADDRESS, tx_buf, TX_LENGTH);

  //Wait
  // delay(TX_INTERVAL);
  //  free(tx_buf);
  //  Serial.println("Done transmitting...");
}



void postData() {
  // Combine yourdatacolumn header (yourdata=) with the data recorded from your arduino
  // (yourarduinodata) and package them into the String yourdata which is what will be
  // sent in your POST request
  //  yourdata = yourdatacolumn + yourarduinodata;

  // Try looking up the website's IP address
  //  ip = 0;
  //  Serial.print(WEBSITE); 
  //  Serial.print(F(" -> "));
  //  while (ip == 0) {
  //    if (! cc3000.getHostByName(WEBSITE, &ip)) {
  //      Serial.println(F("Couldn't resolve!"));
  //    }
  //    delay(500);
  //  }

  ip = cc3000.IP2U32(128, 199, 120, 30);


  cc3000.printIPdotsRev(ip);

  Adafruit_CC3000_Client client = cc3000.connectTCP(ip, 80);

  //  char data[TX_LENGTH];
  //  memcpy(data, tx_buf, TX_LENGTH); 
  Serial.print("Posting ");
  //  Serial.println((char*) tx_buf);

  // If there's a successful connection, send the HTTP POST request
  if (client.connect(ip, 80)) {
    Serial.println("connecting...");

    // EDIT: The POST 'URL' to the location of your insert_mysql.php on your web-host
    //    client.println("POST /insert_mysql.php HTTP/1.1");
    //
    //    // EDIT: 'Host' to match your domain
    //    client.println("Host: satoyamacloud.com");
    //    client.println("User-Agent: Arduino/1.0");
    //    client.println("Connection: close");
    //    client.println("Content-Type: application/x-www-form-urlencoded;");
    //    //    client.print("Content-Length: ");
    //    //    client.println(data.length());
    //    client.println();
    //    client.println(*tx_buf); 

    client.fastrprint(F("POST "));
    //    Serial.println("POST");
    client.fastrprint(WEBPAGE);
    //    Serial.println(WEBPAGE);
    client.fastrprint(F(" HTTP/1.1\r\n"));
    //    Serial.println(" HTTP/1.1\r\n");
    client.fastrprint(F("Host: ")); 
    //    Serial.println("Host: ");
    client.fastrprint(WEBSITE); 
    //    Serial.println(WEBSITE);
    client.fastrprint(F("\r\n"));
    //    Serial.println("\r\n");
    client.fastrprint(F("User-Agent: Arduino/1.0"));
    //    Serial.println("User-Agent: Arduino/1.0");
    client.fastrprint(F("\r\n"));        
    //    Serial.println("\r\n");
    client.fastrprint(F("Connection: close"));
//    client.fastrprint(F("Content-Length: "));
//    client.fastrprint(F("150"));
    //    client.fastrprint(F("Content-Type: application/x-www-form-urlencoded;"))
    //    Serial.println("Connection: close");
    client.fastrprint(F("\r\n"));
    //    Serial.println("\r\n");
    //    client.println();
    //    client.println((char*) tx_buf); 
    //    Serial.println();
    client.println(jbuffer);  // !!! There seems to be a limit to the MAX length of the message !!!
//            client.println("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");     
    //    Serial.println((char *)jbuffer);

    client.println();

    Serial.println("Finished posting");

    Serial.println(F("-------------------------------------"));

    /* Read data until either the connection is closed, or the idle timeout is reached. */
    unsigned long lastRead = millis();
    while (client.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS)) {
      while (client.available()) {
        char c = client.read();
        Serial.print(c);
        lastRead = millis();
      }
    }
    client.close();

  } 
  else {
    // If you couldn't make a connection:
    Serial.println("Connection failed");
    Serial.println("Disconnecting.");
    client.stop();
  }

  Serial.println("Success");
  client.close();

}


void getData() {
  // Combine yourdatacolumn header (yourdata=) with the data recorded from your arduino
  // (yourarduinodata) and package them into the String yourdata which is what will be
  // sent in your POST request
  //  yourdata = yourdatacolumn + yourarduinodata;

  // Try looking up the website's IP address
  //  ip = 0;
  //  Serial.print(WEBSITE); 
  //  Serial.print(F(" -> "));
  //  while (ip == 0) {
  //    if (! cc3000.getHostByName(WEBSITE, &ip)) {
  //      Serial.println(F("Couldn't resolve!"));
  //    }
  //    delay(500);
  //  }

  ip = cc3000.IP2U32(128, 199, 120, 30);

  cc3000.printIPdotsRev(ip);

  Adafruit_CC3000_Client client = cc3000.connectTCP(ip, 80);

  //  char data[TX_LENGTH];
  //  memcpy(data, tx_buf, TX_LENGTH); 


  //  // If there's a successful connection, send the HTTP POST request
  //  if (client.connect(ip, 80)) {
  //    Serial.println("connecting...");
  //
  //    // EDIT: The POST 'URL' to the location of your insert_mysql.php on your web-host
  //    client.println("GET /node/44 HTTP/1.1");
  //
  //    // EDIT: 'Host' to match your domain
  //    client.println("Host: satoyamacloud.com");
  //    client.println("User-Agent: Arduino/1.0");
  //  } 
  //  else {
  //    // If you couldn't make a connection:
  //    Serial.println("Connection failed");
  //    Serial.println("Disconnecting.");
  //    client.stop();
  //  }

  if (client.connected()) {
    client.fastrprint(F("GET "));
    client.fastrprint(WEBPAGE);
    client.fastrprint(F(" HTTP/1.1\r\n"));
    client.fastrprint(F("Host: ")); 
    client.fastrprint(WEBSITE); 
    client.fastrprint(F("\r\n"));
    client.fastrprint(F("\r\n"));
    client.println();
  } 
  else {
    Serial.println(F("Connection failed"));    
    return;
  }


  Serial.println(F("-------------------------------------"));

  /* Read data until either the connection is closed, or the idle timeout is reached. */
  unsigned long lastRead = millis();
  while (client.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS)) {
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
      lastRead = millis();
    }
  }
  client.close();

}


