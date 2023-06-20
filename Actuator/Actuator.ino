#include <WiFi.h>
#include <PubSubClient.h>
#include "WiFi_Manager.h"
#include "MQTT.h"

#define buzzer 32 
const int freq = 5000;  // tần số xung
const int ledChannel = 1; // kênh PWM
const int resolution = 8; // độ phân giải 8bit
WiFiManager wm;

void Wifi_MQTT_Connect(){
      Serial.setTimeout(500);
      client.setServer(MQTT_SERVER, MQTT_PORT);
      client.setCallback(callback);
      connect_to_broker();
}
void setup(){
    Serial.begin(115200);
    ledcSetup(ledChannel, freq, resolution);
    ledcAttachPin(buzzer, ledChannel);

    Serial.setTimeout(500);
    setup_WiFiManager(wm);
    Wifi_MQTT_Connect();
}

void loop(){
    if(checkWiFiConnection(wm))
    {
       if (!client.connected()) connect_to_broker();
       else {
           client.loop();
           if (flag) {          
              ledcWrite(ledChannel, 50);
              delay(500);
              ledcWrite(ledChannel, 150);
              delay(1000);
              ledcWrite(ledChannel, 250);
              delay(500);
              ledcWrite(ledChannel, 0);
              flag = false; 
            }
       }
    }
}
