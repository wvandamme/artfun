
#include "WiFiEsp.h"
#include "LiquidCrystal_I2C.h"

char NAME[] = "Martha";
char WIFI_SSID[] = "inabox.be";        
char WIFI_PASS[] = "Qt!Na1357";
char SERVER[] = "192.168.0.190";

LiquidCrystal_I2C lcd(0x20,16,2); 
WiFiEspClient client;

int status = WL_IDLE_STATUS; 
int connected = -1;

void setup() {
  
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

void loop() {
  
  if (status == WL_IDLE_STATUS)
  {
     status = WiFi.begin(WIFI_SSID, WIFI_PASS);
  }
  else if (status == WL_CONNECT_FAILED)
  {
     status = WiFi.begin(WIFI_SSID, WIFI_PASS);
  }  
  else if (status == WL_CONNECTED)
  {
     if (connected == -1)
     {
       delay(3000);
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print(WiFi.localIP());
       connected = 0;
     }
     else if (connected == 0)
     {
       if (client.connect(SERVER, 8084))
       {
          connected = 1;
          lcd.setCursor(0, 1);
          lcd.print("connected   ");
       }
     }
     else
     {
        String json = String("{ \"name\": \"martha\", \"value\": ") + String(analogRead(A3)) + String(" }");
        client.println("POST /api HTTP/1.1"); 
        client.println("Content-Type: application/json");
        client.println("Content-Length: " + String(json.length()));
        client.println();
        client.println(json);
        client.flush();
        if (!client.connected())
        {
          client.stop();
          connected = 0;
          lcd.setCursor(0, 1);
          lcd.print("disconnected");
        }
     }
  }
  else
  {
     lcd.clear();
     lcd.setCursor(0, 0);
     lcd.print("Reconnecting...");
     status = WiFi.begin(WIFI_SSID, WIFI_PASS);
  }
      
}
