#include <SPI.h>
#include <RH_NRF24.h>

#define REDPIN 4
#define GREENPIN 5
#define BLUEPIN 16

// Singleton instance of the radio driver
//RH_NRF24 nrf24;
RH_NRF24 nrf24(10, 15); // use this to be electrically compatible with Mirf
// RH_NRF24 nrf24(8, 10);// For Leonardo, need explicit SS pin
// RH_NRF24 nrf24(8, 7); // For RFM73 on Anarduino Mini

void setup()
{
  Serial.begin(115200);
  while (!Serial); // wait for serial port to connect. Needed for Leonardo only
  if (!nrf24.init())
  Serial.println("init failed"); // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm

  if (!nrf24.setChannel(1))
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");
}

void loop()
{
  if (nrf24.available())
  {
    //Buffer with a length as long as the max message length from NRF24
    uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    //If we are receiving an message
    if (nrf24.recv(buf, &len))
    {
      //Create a string that contains what's in the message
      String str((char*)buf);

      for(int i = 0; i<len; i++){
        //Then we test the first char to determine whether it's a message for the red, green or blue color
        if(buf[i] == 'r'){
          Serial.print("rouge = ");
          analogWrite(REDPIN, (str.substring(i+1,i+4)).toInt());
          Serial.println(str.substring(i+1,i+4)); //debug print
        }if(buf[i] == 'g'){
          Serial.print("vert = ");
          analogWrite(GREENPIN, (str.substring(i+1,i+4)).toInt());
          Serial.println(str.substring(i+1,i+4)); //debug print
        }if(buf[i] == 'b'){
          Serial.print("bleu = ");
          analogWrite(BLUEPIN, (str.substring(i+1,i+4)).toInt());
          Serial.println(str.substring(i+1,i+4)); //debug print
        }
      }
    }
    else //If the received failed, then we display a message to the user
    {
      Serial.println("recv failed");
    }
  }
}
