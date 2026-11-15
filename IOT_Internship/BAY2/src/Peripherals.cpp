#include <Arduino.h>
#include <DHT.h>
#include "State.h"
#include "Peripherals.h"
#include "config.h"


DHT   dht(DHT_PIN , DHT_TYPE);

float mapFloat(long x, long inmin, long inmax, float outmin, float outmax)
{
    return (float)(x - inmin) * (outmax - outmin) / (float)(inmax - inmin) + outmin;
}

void sample_sensor(void)
{
    int raw_current = analogRead(CURRENT_PIN); // 0 to 4095
    int raw_voltage = analogRead(VOLTAGE_PIN); // 0 to 4095
    //to read temperature 
   
    voltage = mapFloat(raw_voltage, 0, 4095, 0 , 250);

   if(bayStatus == "CHARGING")
   {

    current = mapFloat(raw_current, 0, 4095, 0 , 32);
   
   }

    power = voltage * current;

    float t = dht.readTemperature(DHT_PIN );
    if(!(isnan(t)))
    {
        temperature = t;
    }
    
}

float recentAvgCurrent(void)
{
   float sum = 0;
   for (int i = 0; i < 5; i++)
   {
      sum = sum + current;
   }
   return sum / 5;
}

bool plugin_flag_once = 1;
bool plugout_flag_once = 1;

void plug_status(void)
{
   bool pluginReading = digitalRead(BTN_PLUGIN);
   // detect the sw is pressed
   if(pluginReading == LOW && plugin_flag_once == 1)
   {
      sessionStartMs = millis();

      plugin_flag_once = 0;

      if(bayStatus == "FREE")
      {
         bayStatus = "CHARGING";
         Serial.println("Bay1 plugin detected, Bay is Charging");
         // update leds
         digitalWrite(RELAY_PIN, HIGH);
      }

}
if( pluginReading == HIGH)
   {
      plugin_flag_once = 1;
   }

   bool plugoutReading = digitalRead(BTN_PLUGOUT);
if(plugoutReading == LOW && plugout_flag_once)
   {
      plugout_flag_once = 0;
      if(bayStatus == "CHARGING")
      {
         bayStatus = "FREE";
         Serial.println("Bay1 plugout detected, Bay is Free");
         // update leds
      }
   }
   if( plugoutReading == HIGH)
   {
      plugout_flag_once = 1;
   }
}

/*void update_led_status(void)
{
   if(bayStatus == "FREE")
   {
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(LED_RED, LOW);
   }
   else if(bayStatus == "CHARGING")
   {
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_YELLOW, HIGH);
      digitalWrite(LED_RED, LOW);
   }

}*/