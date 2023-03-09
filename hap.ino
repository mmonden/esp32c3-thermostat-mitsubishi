#include "HomeSpan.h"
// #include "DEV_LED.h"
// #include "esp_wifi.h"
#include "WiFi.h"
#include "DEV_Thermostat.h"

void setup() 
{
  Serial.begin(115200);

  // const char *wifi_name = "Super Internet van Telenet !";
  // const char *PASSW = "Krista123";pzfhfh3zjhmB
  // WiFi.mode(WIFI_AP);
  // WiFi.softAP(wifi_name, PASSW);

//  Setup HomeSpan
  homeSpan.begin(Category::Thermostats,"Thermostat ESP32C3");
  
  new SpanAccessory(); 
  new Service::AccessoryInformation(); 
  new Characteristic::Identify();

  new DEV_Thermostat(A0, (unsigned long) 1000); // Variable update time?
}

void loop() 
{
  homeSpan.poll();
}
