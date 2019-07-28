#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>


static const byte glyph[] = { B00010000, B00110100, B00110000, B00110100, B00010000 };

////////////////////////////////////

RF24 radio(9,10);    // define radio pin, this case mean CE -> 8(Green) & CSI -> 10(Yellow) 
const uint64_t pipe = 0xE8E8F0F0E1LL;    // define pipe of signal, ardresse of pipe
byte data_1[3];      // data receive from transmitter. "humidity" and "Temperature"

void setup(void)
{
  Serial.begin(9600);
  radio.begin();  // Start up radio of NRF24
  radio.openReadingPipe(1,pipe);  // open a pipe for read a radio
  radio.startListening();  // start listening all data transmitted by client
  pinMode(1,OUTPUT);
}

void loop()   
{
  if ( radio.available() )
  {
     bool done = false;  // define done in boolin type for a loop while
     digitalWrite(2,HIGH);
    while (!done)  // not done
    {
      done = radio.read( data_1, sizeof(data_1) );

 
   
       Serial.print("******Menu****** \n");   // Print or display "MenuTuto" on first line of LCD
   
        Serial.print("Temperature :\n");
        Serial.print(data_1[0]);
       Serial.println(" C");
      
       Serial.print("Humidity :\n");
       Serial.print(data_1[1]);
       Serial.println(" %");
        
      delay(10);
    }
  }
  digitalWrite(2,LOW);

  
}
