#include <WiFi.h>
#include <PubSubClient.h>
#include "Model.h"
#include "WiFi_Manager.h"

WiFiManager wm;

#define HOSTNAME "mqttx_MQTT"

const char *MQTT_HOST = "broker.emqx.io";
const int MQTT_PORT = 1883;
const char *MQTT_USER = "TanTan"; // có thể để trống nếu không yêu cầu xác thực
const char *MQTT_PASS = "12345567899"; 
const char MQTT_SUB_TOPIC_A[] = "CE339_NodeA";
const char MQTT_PUB_TOPIC_A[] = "CE339_Gateway/A";
const char MQTT_SUB_TOPIC_B[] = "CE339_NodeB";
const char MQTT_PUB_TOPIC_B[] = "CE339_Gateway/B";
const char MQTT_SUB_TOPIC_Act[] = "CE339_Gateway/Act";
const char MQTT_PUB_TOPIC_Act[] = "CE339_Act";
WiFiClient wifiClient;
PubSubClient client(wifiClient);

String Pub_mess_A = "";
String Pub_mess_B = "";
String Pub_mess_Act = "";
bool flag_A = true;
bool flag_B = true;
bool flag_Act = true;

void connect_to_broker() 
{
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(HOSTNAME, MQTT_USER, MQTT_PASS)) {
      Serial.println("connected");
      client.subscribe(MQTT_SUB_TOPIC_A);
      client.subscribe(MQTT_SUB_TOPIC_B);
      client.subscribe(MQTT_SUB_TOPIC_Act);
    } else {
      Serial.print("failed, status code =");
      Serial.print(client.state());
      Serial.println("try again in 3 seconds");
      delay(3000);
    }
  }
}

void callbackA(char* topic, byte *payload, unsigned int length) {
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");
  // Serial.write(payload, length);
  // Serial.println();
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i]; // Nối từng ký tự vào chuỗi message
  }
  //Serial.println(message); // In ra chuỗi message

  float temperature = message.substring(message.indexOf("Temperature:") + 12, message.indexOf("\n")).toFloat();
  float humidity = message.substring(message.indexOf("Humidity:") + 9, message.indexOf("\n", message.indexOf("Humidity:"))).toFloat();
  float temperatureBMP = message.substring(message.indexOf("Temperature BMP:") + 16, message.indexOf("\n", message.indexOf("Temperature BMP:"))).toFloat();
  float pressureBMP = message.substring(message.indexOf("Pressure BMP:") + 13).toFloat();

  // In ra kết quả
  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.print("Temperature BMP: ");
  Serial.println(temperatureBMP);
  Serial.print("Pressure BMP: ");
  Serial.println(pressureBMP);

  float Temperature = (temperature + temperatureBMP)/2.0;
  Pub_mess_A = "Temperature: " + String(Temperature) + "\n" + "Humidity: " + String(humidity) + "\n" + "Pressure: " + String(pressureBMP);
  flag_A = false; 
}
void callbackB(char* topic, byte *payload, unsigned int length) {
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i]; // Nối từng ký tự vào chuỗi message
  }
  //Serial.println(message); // In ra chuỗi message

  float temperature = message.substring(message.indexOf("Temperature:") + 12, message.indexOf("\n")).toFloat();
  float humidity = message.substring(message.indexOf("Humidity:") + 9, message.indexOf("\n", message.indexOf("Humidity:"))).toFloat();
  float temperatureBMP = message.substring(message.indexOf("Temperature BMP:") + 16, message.indexOf("\n", message.indexOf("Temperature BMP:"))).toFloat();
  float pressureBMP = message.substring(message.indexOf("Pressure BMP:") + 13).toFloat();

  // In ra kết quả
  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.print("Temperature BMP: ");
  Serial.println(temperatureBMP);
  Serial.print("Pressure BMP: ");
  Serial.println(pressureBMP);

  float Temperature = (temperature + temperatureBMP)/2.0;
  Pub_mess_B = "Temperature: " + String(Temperature) + "\n" + "Humidity: " + String(humidity) + "\n" + "Pressure: " + String(pressureBMP);
  flag_B = false; 
}
void callbackAct(char* topic, byte *payload, unsigned int length) {
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");
  // Serial.write(payload, length);
  // Serial.println();
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i]; // Nối từng ký tự vào chuỗi message
  }

  //Decode cái chuỗi mã hóa nhận được server
  Pub_mess_Act = message;
  
  Serial.print("Message to Actuator: ");
  Serial.println(Pub_mess_Act);

  flag_Act = false; 
}
void callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, MQTT_SUB_TOPIC_A) == 0) {
    // Thực hiện xử lý cho topic MQTT_SUB_TOPIC_A
    callbackA(topic, payload, length);
  } 
  else if (strcmp(topic, MQTT_SUB_TOPIC_B) == 0) {
    // Thực hiện xử lý cho topic MQTT_SUB_TOPIC_B
    callbackB(topic, payload, length);
  }
  else if (strcmp(topic, MQTT_SUB_TOPIC_Act) == 0) {
    callbackAct(topic, payload, length);
  }
}
String vigenere_encode(const String& inputString, const String& key) {
    String encodedString;
    size_t inputLength = inputString.length();
    size_t keyLength = key.length();

    for (size_t i = 0; i < inputLength; ++i) {
        char encodedChar = (inputString.charAt(i) + key.charAt(i % keyLength)) % 128;
        encodedString += encodedChar;
    }

    return encodedString;
}
String vigenere_decode(const String& encodedString, const String& key) {
    String decodedString;
    size_t encodedLength = encodedString.length();
    size_t keyLength = key.length();

    for (size_t i = 0; i < encodedLength; ++i) {
        char decodedChar = (encodedString.charAt(i) - key.charAt(i % keyLength) + 128) % 128;
        decodedString += decodedChar;
    }

    return decodedString;
}

void setup() 
{
  Serial.begin(115200);
  setup_WiFiManager(wm);
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(callback);
  connect_to_broker();
}
int a = 0;
void loop() 
{
  if(checkWiFiConnection(wm)){
  {
    if (!client.connected())
    {
      connect_to_broker();
    }
    else
    {
      client.loop();
    }
  }
  if(!flag_A)
  {
    client.publish(MQTT_PUB_TOPIC_A, vigenere_encode(Pub_mess_A, "CE339").c_str());
    flag_A = true;
  }
  if(!flag_B)
  {
    client.publish(MQTT_PUB_TOPIC_B, vigenere_encode(Pub_mess_B, "CE339").c_str());
    flag_B = true;
  }
  if(!flag_Act)
  {
    client.publish(MQTT_PUB_TOPIC_Act, vigenere_decode(Pub_mess_Act, "CE339").c_str());
    flag_Act = true;
  }
  }
}
