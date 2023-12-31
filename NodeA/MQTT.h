#define MQTT_SERVER "broker.emqx.io"
#define MQTT_PORT 1883
#define MQTT_TOPIC_PUB "CE339_NodeA"

WiFiClient wifiClient;
PubSubClient client(wifiClient);
    
void connect_to_broker() {
    while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "IOT";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
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
}
void publish_message(String message){
    client.publish(MQTT_TOPIC_PUB, message.c_str());
    Serial.println("Send Message to broker-----");
    Serial.print("topic: ");
    Serial.println(MQTT_TOPIC_PUB);
    Serial.print("message: ");
    Serial.println(message); 
}
