
#include "WiFiEsp.h"
#include "LiquidCrystal_I2C.h"

char NAME[] = "Martha";
char WIFI_SSID[] = "Vivalyte 2.4";        
char WIFI_PASS[] = "Vivalyte1!";
char SERVER[] = "192.168.1.25";
int  SENSOR = 3;

float upperThreashold = 7000;
float lowerThreashold = 0;

LiquidCrystal_I2C lcd(0x20,16,2); 
WiFiEspClient pclient;

String resp = "";
String state = "ok";
int wifi_status = WL_IDLE_STATUS; 
int connection_status = -1;
int raw = 0;
int Vin= 5;
float Vout= 0;
float R1= 1000;
float R2= 0;
float buffer= 0;

void setup() {
    pinMode(2, OUTPUT);
  
    lcd.init();
    lcd.backlight();
    lcd.home();
    
    lcd.setCursor(0, 0);
    lcd.print("Welcome");
    lcd.setCursor(0, 1);
    lcd.print(NAME);   
    
    Serial.begin(115200);
    WiFi.init(&Serial);       
}

void do_connected()
{
    switch (connection_status)
     {
        case -1:     
        {
               delay(2000);
               lcd.clear();
               lcd.setCursor(0, 0);
               lcd.print(WiFi.localIP());
               connection_status = 0;
               break;      
        }
        case 0:
        {
               digitalWrite(2, LOW);
               if (pclient.connect(SERVER, 8084))
               {
                  connection_status = 1;
               }
               break;
        }
        case 1:
        {
               
               if (!pclient.connected())
               {
                   pclient.stop();
                   connection_status = 0;
               }
               
               lcd.setCursor(0, 1);
               lcd.print(state);
               lcd.print("     ");
    
               String json = String("{ \"name\": \"") + String(NAME) + String("\", \"state\": \"") + state + String("\"}"); 
               pclient.flush();
               pclient.println("POST /api HTTP/1.1");       
               pclient.println("Content-Type: application/json");
               pclient.println("Content-Length: " + String(json.length()));
               pclient.println();
               pclient.print(json);
               resp = "";
               connection_status = 2;
               break;
               
        }
        case 2:
        {
               if (!pclient.connected())
               {
                   pclient.stop();
                   connection_status = 0;
               }
               
               if (pclient.available() > 0)
               {
                   char c = pclient.read();
                   if ((c == '\r') || (c == '\n'))
                   {
                       connection_status = 1;
                       if (resp.indexOf("201") > 0)
                       {
                          digitalWrite(2, HIGH);
                       }
                       else
                       {
                          digitalWrite(2, LOW);
                       }
                   }
                   else
                   {
                       resp += c;
                   }
               }
               break;
        }        
        default:
        {
               break;
        }
    }
}


void loop() 
{
  raw = analogRead(SENSOR);
  if(raw) 
  {
    buffer= raw * Vin;
    Vout= (buffer)/1024.0;
    buffer= (Vin/Vout) -1;
    R2= R1 * buffer;
    if(R2 > upperThreashold)
    {
      state = "thirsty";
    }
    else if (R2 < lowerThreashold)
    {
      state = "drowning";
    }
    else
    {
      state = "ok"; 
    }
  }
                                              
  if (wifi_status == WL_IDLE_STATUS)
  {
     connection_status = -1;
     wifi_status = WiFi.begin(WIFI_SSID, WIFI_PASS);
  }
  else if (wifi_status == WL_CONNECT_FAILED)
  {
     connection_status = -1;
     wifi_status = WiFi.begin(WIFI_SSID, WIFI_PASS);
  }  
  else if (wifi_status == WL_CONNECTED)
  {
     do_connected();
  }
  else
  {
     connection_status = -1;
     lcd.clear();
     lcd.setCursor(0, 0);
     lcd.print("Reconnecting...");
     wifi_status = WiFi.begin(WIFI_SSID, WIFI_PASS);
  }
  
}
