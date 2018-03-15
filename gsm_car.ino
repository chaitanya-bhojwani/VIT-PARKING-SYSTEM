#include "Adafruit_FONA.h"
#include <SoftwareSerial.h>
#include <string.h>
#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4
//#define FONA_RI 7


const byte s1 = 7;    
const byte s2 = 6;
const byte s3 = 5;


int p1;
int p2;
int p3;

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);

uint8_t type;

int gprs(int);
void entry();

void flushSerial() {
  while (Serial.available())
    Serial.read();
}

void setup() {
  // put your setup code here, to run once:

 

  while (!Serial);

  Serial.begin(9600);
  Serial.println(F("FONA basic test"));
  Serial.println(F("Initializing....(May take 3 seconds)"));

  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }
  type = fona.type();
  Serial.println(F("FONA is OK"));
  Serial.print(F("Found "));
  switch (type) {
    case FONA800L:
      Serial.println(F("FONA 800L")); break;
    case FONA800H:
      Serial.println(F("FONA 800H")); break;
    case FONA808_V1:
      Serial.println(F("FONA 808 (v1)")); break;
    case FONA808_V2:
      Serial.println(F("FONA 808 (v2)")); break;
    case FONA3G_A:
      Serial.println(F("FONA 3G (American)")); break;
    case FONA3G_E:
      Serial.println(F("FONA 3G (European)")); break;
    default: 
      Serial.println(F("???")); break;

      
  // Print module IMEI number.
  char imei[15] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("Module IMEI: "); Serial.println(imei);
  }
//000
  // Optionally configure a GPRS APN, username, and password.
  // You might need to do this to access your network's GPRS/data
  // network.  Contact your provider for the exact APN, username,
  // and password values.  Username and password are optional and
  // can be removed, but APN is required.
  fona.setGPRSNetworkSettings(F("www"), F(""), F(""));

  // Optionally configure HTTP gets to follow redirects over SSL.
  // Default is not to follow SSL redirects, however if you uncomment
  // the following line then redirects over SSL will be followed.
  fona.setHTTPSRedirect(true);

  
  }
}
int gprs(int s) {

  if( s == 0) {
          // turn GPRS off
          
        if (!fona.enableGPRS(false))
         { Serial.println(F("Failed to turn off"));
          return 0; }
      else { return 1; }
    
  }
  else if( s==1 ) {
       // turn GPRS on
        if (!fona.enableGPRS(true))
        {  Serial.println(F("Failed to turn on"));
          return 0;}
          else { Serial.println(F("**********turned on")); return 1; }
    }
}

void entry() {
  
        // read website URL
        uint16_t statuscode;
        int16_t length;
        

        flushSerial();
        Serial.println(F("NOTE: in beta! Use small webpages to read!"));
        Serial.println(F("URL to read (e.g. www.adafruit.com/testwifi/index.html):"));
        Serial.print(F("http://"));
        //readline(url, 79);
        

        char url[80];
        char dp[10];
        strcpy(url,"http://api.pushingbox.com/pushingbox?devid=v62CE85367BF94E7&p1=");
        itoa(p1, dp, 10);
        strcat(url,dp);

        strcat(url,"&p2=");
        itoa(p2, dp, 10);
        strcat(url,dp);

        strcat(url,"&p3=");
        itoa(p3, dp, 10);
        strcat(url,dp);
        



        Serial.println(url);
        
        Serial.println(F("****"));
        if (!fona.HTTP_GET_start(url, &statuscode, (uint16_t *)&length)) {
          Serial.println("Failed!");
          return;
         // break;
        }
        while (length > 0) {
          while (fona.available()) {
            char c = fona.read();

            // Serial.write is too slow, we'll write directly to Serial register!
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
            loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
            UDR0 = c;
#else
            Serial.write(c);
#endif
            length--;
            if (! length) break;
          }
        }
        Serial.println(F("\n****"));
        fona.HTTP_GET_end();
  }


void loop() {
   p1=digitalRead(s1);
   p2=digitalRead(s2);
   p3=digitalRead(s3);   
       
/*// send an SMS!
        String sendto1="7405577075";
        char sendto[21];
        
        char msg[141];
        char dp[10];
        strcpy(msg,"  Ph=");
        itoa(Po, dp, 10);
        strcat(msg,dp);

        strcat(msg,"  Tu=");
        itoa(T1, dp, 10);
        strcat(msg,dp);

        strcat(msg,"  T=");
        itoa(Temp1, dp, 10);
        strcat(msg,dp);
        Serial.println(msg);

        
        
        
       sendto1.toCharArray(sendto,21);
        
        
  while (Serial.available())
    Serial.read();

        Serial.println(sendto);
       if(i==1)
        {
          if (!fona.sendSMS(sendto, msg)) {
            Serial.println(F("Failed"));
          } else {
            Serial.println(F("Sent!"));
          }
          i=i-1;
        }
*/
  char command;
  do
  {

while (! Serial.available() ) {
    if (fona.available()) {
      Serial.write(fona.read());
    }
  }
    
    command = Serial.read();
    Serial.println(command);
    if(command == 'G')
    {
     gprs(1);
    }
    else if(command == 'g')
    {
     gprs(0);  
    }
    else if(command == 'e')
    {
      entry();
    }
  }while(command != 'e');
        
        delay(100000000);

}
