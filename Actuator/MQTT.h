#define MQTT_SERVER "broker.emqx.io"
#define MQTT_PORT 1883
#define MQTT_TOPIC_SUB "CE339_Act"
#define MQTT_TOPIC_PUB ""

WiFiClient wifiClient;
PubSubClient client(wifiClient);
bool flag = false;    

void connect_to_broker() {
    while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "IOT";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(MQTT_TOPIC_SUB);
    } else {
      Serial.print("failed");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
    }
  }
}
void callback(char* topic, byte *payload, unsigned int length) {
    Serial.println("Receive message from broker-----");
    Serial.print("topic: ");
    Serial.println(topic);
    Serial.print("message: ");
    Serial.write(payload, length);
    Serial.println();
    String message = "";
    for (int i = 0; i < length; i++) {
      message += (char)payload[i]; // Nối từng ký tự vào chuỗi message
    }
    if (message == "on") flag = true;
}
void publish_message(String message){
    client.publish(MQTT_TOPIC_PUB, message.c_str());
    Serial.println("Send Message to broker-----");
    Serial.print("topic: ");
    Serial.println(MQTT_TOPIC_PUB);
    Serial.print("message: ");
    Serial.println(message); 
}
