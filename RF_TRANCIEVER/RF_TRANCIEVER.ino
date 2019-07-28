#include <SPI.h>
#include <nRF24L01.h>      // define NRF24L01's library (download from référence) 
#include <RF24.h>
#include <RF24_config.h>
#include <dht.h>           // Define DHT11 temperature sensor's library (Download from référence)
#define dht_dpin A0        // Confifuration defaut "A0". It's possible to change your analog pin since A0 to A5. Make your best choice

RF24 radio(9,10);           // Define radio pin RF24 radio(CE,CSN), this case mean CE -> 8 & CSN -> 10. Hope it's a best choice. Or your can change like you want
const uint64_t pipe = 0xE8E8F0F0E1LL;
byte data_1[3];            // Declare table of data type BYTE. That for Humidity's data and Temperature's data       

dht DHT;

void setup(void)
{
  Serial.begin(9600);      // Setup serial monitor'rate to 9600. Defaut setting 
  delay(300);              // Delay 0.3s before next step
  
  radio.begin();           // Turn on RF24L01
  radio.openWritingPipe(pipe);
  radio.openReadingPipe(1,pipe);
  
  Serial.println("Humidity and temperature\n\n");  // Display "Humidity and temperature\n\n" Arduino Serial Monitor (Tools --> Serial Monitor)
  delay(700);
}
  
void loop(void)
{
  DHT.read11(dht_dpin);  // Set DHT11 to read dht_dpin. My case is "A0"
  data_1[0]  = DHT.temperature;  // Set temperature acquisition's data to data's slot 0
  data_1[1]  = DHT.humidity;     // set temperature acquisition's data to data's slot 1

// Setting up serial monotor display:
  Serial.print("Current humidity = ");
  Serial.print(DHT.humidity);
  Serial.print(" %  ");
  Serial.print("temperature = ");
  Serial.print(DHT.temperature); 
  Serial.println(" C  ");
  Serial.print(" --> sending : ");
  
  radio.write(data_1,sizeof(data_1));  // Radio sending's command
  
// Radio sending's command
 
  Serial.print(" --> succesful\n");
  
  delay(400);
  
  radio.powerDown();  // Save power by powerdown mode
  delay(500);
  radio.powerUp();    // Turn back to online's mode
}
