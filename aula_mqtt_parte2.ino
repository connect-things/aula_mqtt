#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h> //5.7.0


const char* ssid     = "Iot";
const char* password = "12345678";

unsigned long lastTime = 0;

const char* MQTT_SERVER = "192.168.0.111";
WiFiClient CLIENT;
PubSubClient MQTT(CLIENT);

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  MQTT.setServer(MQTT_SERVER, 1883);
}

void reconnect() {
  while (!MQTT.connected()) {
    Serial.println("Conectando ao Broker MQTT.");
    if (MQTT.connect("ESP8266-Francisco")) {
      Serial.println("Conectado com Sucesso ao Broker");
    } else {
      Serial.print("Falha ao Conectador, rc=");
      Serial.print(MQTT.state());
      Serial.println(" tentando se reconectar...");
      delay(3000);
    }
  }
}


void loop() {
  if (!MQTT.connected()) {
    reconnect();
  }
  MQTT.loop();

  unsigned long now = millis();
  if((lastTime + 5000) < now){
    lastTime = now;

    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["temp"] = 20;
    root["humid"] = 70;
    root["time"] = 123;
    root["id"] = "Francisco";
    String msg;
    root.printTo(msg);
    Serial.println(msg);
    MQTT.publish("temp/random", msg.c_str());
    

  }
}
