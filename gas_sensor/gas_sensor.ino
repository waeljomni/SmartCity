#include <SPI.h>
#include <nRF24L01.h>      // define NRF24L01's library (download from référence) 
#include <RF24.h>
#include <RF24_config.h>


RF24 radio(9, 10);          // Define radio pin RF24 radio(CE,CSN), this case mean CE -> 8 & CSN -> 10. Hope it's a best choice. Or your can change like you want
const uint64_t pipe = 0xE8E8F0F0E1LL;
byte data_1[3];            // Declare table of data type BYTE. That for Humidity's data and Temperature's data



void setup(void)
{
  Serial.begin(9600);      // Setup serial monitor'rate to 9600. Defaut setting
  delay(300);              // Delay 0.3s before next step

  radio.begin();           // Turn on RF24L01
  radio.openWritingPipe(pipe);
  radio.openReadingPipe(1, pipe);

  Serial.println("Gas sensor\n\n");  // Display "Humidity and temperature\n\n" Arduino Serial Monitor (Tools --> Serial Monitor)
  delay(700);
}

void loop(void)
{

  float sensor_volt;
  float RS_gas; // Get value of RS in a GAS
  float ratio; // Get ratio RS_GAS/RS_air
  int sensorValue = analogRead(A0);
  sensor_volt=(float)sensorValue/1024*5.0;
 // RS_gas = (5.0-sensor_volt)/sensor_volt; // omit *RL
 
  /*-Replace the name "R0" with the value of R0 in the demo of First Test -*/
  //ratio = RS_gas/R0;  // ratio = RS/R0 
  /*-----------------------------------------------------------------------*/
  data_1[0]  = sensor_volt;
  
  Serial.print("sensor_volt = ");
  Serial.println(sensor_volt);
  Serial.print("RS_ratio = ");
  Serial.println(RS_gas);
  Serial.print("Rs/R0 = ");
  Serial.println(ratio);
 
  Serial.print("\n\n");
  Serial.print(" --> sending : ");
  delay(1000);
  
  // Radio sending's command
  radio.write(data_1,sizeof(data_1));  // Radio sending's command

  Serial.print(" --> succesful\n");
  
  delay(400);

  radio.powerDown();  // Save power by powerdown mode
  delay(500);
  radio.powerUp();    // Turn back to online's mode
}
