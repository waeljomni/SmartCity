#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <SWTFT.h> // Hardware-specific library



#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

SWTFT tft;


static const byte glyph[] = { B00010000, B00110100, B00110000, B00110100, B00010000 };

////////////////////////////////////

RF24 radio(20,21);    // define radio pin, this case mean CE -> 8(Green) & CSI -> 10(Yellow) 
const uint64_t pipe = 0xE8E8F0F0E1LL;    // define pipe of signal, ardresse of pipe
byte data_1[3];      // data receive from transmitter. "humidity" and "Temperature"

void setup(void)
{

    tft.reset();
  
  uint16_t identifier = tft.readID();

  Serial.print(F("LCD driver chip: "));
  Serial.println(identifier, HEX);
    

  tft.begin(identifier);

  tft.fillScreen(BLACK);

  tft.fillRect(0, 0, 240, 20, BLUE);
  tft.fillRect(0, 300, 240, 20, BLUE);




 
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

// Display on LCD Nokia      
   
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


//////////////////////////////////////////






  
}
