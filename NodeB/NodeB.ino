    #include <PubSubClient.h>
    #include <DHT.h> 
    #include <Wire.h>    
    #include <Adafruit_BMP280.h>
    #include "WiFi_Manager.h"
    #include "MQTT.h" 
            
    const int DHTPIN = 33;       //Đọc dữ liệu từ DHT11 ở chân 2 trên mạch Arduino
    const int DHTTYPE = DHT11;  //Khai báo loại cảm biến, có 2 loại là DHT11 và DHT22
    DHT dht(DHTPIN, DHTTYPE);
        
    WiFiManager wm;
    Adafruit_BMP280 bmp; // I2C

    void Wifi_MQTT_Connect(){     
        Serial.setTimeout(500);
        client.setServer(MQTT_SERVER, MQTT_PORT);
        client.setCallback(callback);
        connect_to_broker();
  }
  
    void setup() {
      Serial.begin(115200);
      Serial.setTimeout(500);
      dht.begin(); 
      bmp.begin(0x76);
      bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
     setup_WiFiManager(wm);
     Wifi_MQTT_Connect();
    }

    void loop() {
      if(checkWiFiConnection(wm))
      {
         if (!client.connected()) connect_to_broker();
         else {
            client.loop();
            float h = dht.readHumidity();    //Đọc độ ẩm
            float t = dht.readTemperature(); //Đọc nhiệt độ
            
            String Temp = "Temperature: " + String(t);
            String Hum = "Humidity: " + String(h);
            String message = Temp + "\n" + Hum;
      
            float Tempbmp = bmp.readTemperature();
            float Pressbmp = bmp.readPressure();
            String Temp_bmp = "Temperature BMP: " + String(Tempbmp);
            String Press_bmp = "Pressure BMP: " + String(Pressbmp);
            
            message += "\n" + Temp_bmp + "\n" + Press_bmp;
            publish_message(message);
            delay(5000);
            }
      }
    }
