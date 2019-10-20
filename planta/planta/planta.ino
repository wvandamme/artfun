
#include "WiFiEsp.h"
#include "LiquidCrystal_I2C.h"

LiquidCrystal_I2C lcd(0x20,16,2); 

int status = WL_IDLE_STATUS; 

void setup() {
    lcd.init();
    lcd.backlight();
    lcd.home();
    
    lcd.setCursor(0, 0);
    lcd.print("Connecting...");    
    
    Serial.begin(115200);
    WiFi.init(&Serial);

    while ( status != WL_CONNECTED) {
      status = WiFi.begin("inabox.be", "Qt!Na1357");
    }
    




    
}

void loop() {
  
    IPAddress ip = WiFi.localIP();
    lcd.setCursor(0, 1);
    lcd.print(ip);
    
}
