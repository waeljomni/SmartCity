
/** RF24Mesh_Example.ino by TMRh20
 *
 * This example sketch shows how to manually configure a node via RF24Mesh, and send data to the
 * master node.
 * The nodes will refresh their network address as soon as a single write fails. This allows the
 * nodes to change position in relation to each other and the master node.
 */


#include "RF24.h"
#include "RF24Network.h"
#include "RF24Mesh.h"
#include <SPI.h>
#include <EEPROM.h>
#include "DHT.h"
//#include <printf.h>


/**** Configure the nrf24l01 CE and CS pins ****/
RF24 radio(9, 10);
RF24Network network(radio);
RF24Mesh mesh(radio, network);

/**
 * User Configuration: nodeID - A unique identifier for each radio. Allows addressing
 * to change dynamically with physical changes to the mesh.
 *
 * In this example, configuration takes place below, prior to uploading the sketch to the device
 * A unique value from 1-255 must be configured for each node.
 * This will be stored in EEPROM on AVR devices, so remains persistent between further uploads, loss of power, etc.
 *
 **/
#define nodeID 3

#define DHTPIN 2     // what digital pin we're connected to
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11


uint32_t displayTimer = 0;

struct payload_t {
  unsigned long ms;
  unsigned long counter;
};

void setup() {

  Serial.begin(115200);
  //printf_begin();
  // Set the nodeID manually
  mesh.setNodeID(nodeID);
  // Connect to the mesh
  Serial.println(F("Connecting to the mesh..."));
  mesh.begin();
  dht.begin();
}




void loop() {
  void sendOverRadio() {
    //power on radio
    radio.powerUp();

    //prepare
    uint8_t data1 = 0;
    bool timeout = 0;
    uint16_t nodeID = pipes[0] & 0xff;
    char outBuffer[32] = "";
    unsigned long send_time, rtt = 0;

    // Stop listening and write to radio
    radio.stopListening();

    // Send to hub
    if ( radio.write(&p, sizeof(p)) ) {
      //Serial.println("Send successful\n\r");
      send_time = millis();
    } else {
      //Serial.println("Send failed\n\r");
    }

    //wait response
    radio.startListening();
    delay(20);
    while ( radio.available() && !timeout ) {
      uint8_t len = radio.getDynamicPayloadSize();
      radio.read( receivePayload, len);

      receivePayload[len] = 0;
      // Compare receive payload with outBuffer
      if ( ! strcmp(outBuffer, receivePayload) ) {
        rtt = millis() - send_time;
        //Serial.println("inBuffer --> rtt:");
        //Serial.println(rtt);
      }

      // Check for timeout and exit the while loop
      if ( millis() - send_time > radio.getMaxTimeout() ) {
        //Serial.println("Timeout!!!");
        timeout = 1;
      }

      delay(10);
    } // End while
  }

  void readSensor() {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print(f);
    Serial.print(" *F\t");
    Serial.print("Heat index: ");
    Serial.print(hic);
    Serial.print(" *C ");
    Serial.print(hif);
    Serial.println(" *F");
  }


  void sendData() {

    mesh.update();

    // Send to the master node every second
    if (millis() - displayTimer >= 1000) {
      displayTimer = millis();

      // Send an 'M' type message containing the current millis()
      if (!mesh.write(&displayTimer, 'M', sizeof(displayTimer))) {

        // If a write fails, check connectivity to the mesh network
        if ( ! mesh.checkConnection() ) {
          //refresh the network address
          Serial.println("Renewing Address");
          mesh.renewAddress();
        } else {
          Serial.println("Send fail, Test OK");
        }
      } else {
        Serial.print("Send OK: ");
        Serial.println(displayTimer);
      }
    }

    while (network.available()) {
      RF24NetworkHeader header;
      payload_t payload;
      network.read(header, &payload, sizeof(payload));
      Serial.print("Received packet #");
      Serial.print(payload.counter);
      Serial.print(" at ");
      Serial.println(payload.ms);
    }
  }






