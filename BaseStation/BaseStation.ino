//***************************Master or Receiver code*****************
/*This code was used for a video tutorial on the ForceTronics YouTube Channel
 * This code is free and open for anybody to use and modify at your own risk
*/

#include <SPI.h> //Call SPI library so you can communicate with the nRF24L01+
#include <nRF24L01.h> //nRF2401 libarary found at https://github.com/tmrh20/RF24/
#include <RF24.h> //nRF2401 libarary found at https://github.com/tmrh20/RF24/
#include <LiquidCrystal_I2C.h>

//LCD 1602
const int  en = 2, rw = 1, rs = 0, d4 = 4, d5 = 5, d6 = 6, d7 = 7, bl = 3;
const int i2c_addr = 0x27;
LiquidCrystal_I2C lcd(i2c_addr, en, rw, rs, d4, d5, d6, d7, bl, POSITIVE);


const uint8_t pinCE = 9; //This pin is used to set the nRF24 to standby (0) or active mode (1)
const uint8_t pinCSN = 10; //This pin is used for SPI comm chip select
RF24 wirelessSPI(pinCE, pinCSN); // Declare object from nRF24 library (Create your wireless SPI) 
const uint64_t rAddress = 0xB00B1E50C3LL;  //Create pipe address for the network and notice I spelled boobies because I am mature, the "LL" is for LongLong type
const uint8_t rFChan = 89; //Set channel frequency default (chan 84 is 2.484GHz to 2.489GHz)
const int buzzer = 2; 
const unsigned long interval = 5000;// 5s;
unsigned long last_clear;  

//Create a structure to hold fake sensor data and channel data  
struct PayLoad {
  uint8_t chan;
  uint8_t sensor;
};

PayLoad payload; //create struct object

void setup() {
  Serial.begin(115200);  //serial port to display received data
  lcd.begin(16,2);
  lcd.home();
  lcd.setCursor(2,0);
  lcd.print("INIT...");
         
  wirelessSPI.begin();  //Start the nRF24 module
  wirelessSPI.setChannel(rFChan); //set communication frequency channel
  wirelessSPI.openReadingPipe(1,rAddress);  //This is receiver or master so we need to be ready to read data from transmitters
  wirelessSPI.startListening();    // Start listening for messages
  
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer,LOW); 
  Serial.println("Network master is online...");
}

void loop() {
   
   
  if(wirelessSPI.available()){ //Check if recieved data
  
     wirelessSPI.read(&payload, sizeof(payload)); //read packet of data and store it in struct object

     Serial.print("ping from node: ");
     Serial.println(payload.chan); //print node number or channel
     
     if (payload.sensor == 1){
      Alarm();
     }
  }

  InitDisplay();
}

void Alarm()
{
   tone(buzzer, 1000);
   lcd.clear();
   lcd.setCursor(2,0);
   lcd.print("PANIC BUTTON ");
   lcd.setCursor(8,1);
   lcd.print(payload.chan);
}

void InitDisplay()
{
    unsigned long now = millis();
   if (now - last_clear >= interval ) {
      last_clear = now;
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("STAND BY...");
      noTone(buzzer); 
   }
}

