#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>
#define led1 2
#define led2 4

const int DHTPIN = 18;
const int DHTTYPE = DHT11;

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Redmi 10";
const char* password = "05102003";
const char* mqtt_server = "172.20.10.2";
const int mqtt_port = 1885;
const char* mqtt_username = "qwer";
const char* mqtt_password = "123";
const char* mqtt_topics[] = {"sensor", "led"};

float temperature;
float humidity;
float light;

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsgTime = 0;
const long interval = 10000;
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect to WiFi.");
    return;
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }
   Serial.println(messageTemp);
  if (messageTemp == "all_on") {
    digitalWrite(led1, 1);
    digitalWrite(led2, 1);
    return;
  }
  if (messageTemp == "all_off") {
    digitalWrite(led1, 0);
    digitalWrite(led2, 0);
    return;
  }
  StaticJsonDocument<300> doc;
  DeserializationError error = deserializeJson(doc, messageTemp);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return;
  }
  const char* led1State = doc["led1"];
  const char* led2State = doc["led2"];
  // Điều khiển các LED dựa trên trạng thái từ JSON
  if (doc.containsKey("led1")) {
    const char* led1State = doc["led1"];
     if(strcmp(led1State, "on") == 0 ) digitalWrite(led1,1); else digitalWrite(led1,0);
  }
  
  if (doc.containsKey("led2")) {
    const char* led2State = doc["led2"];
     if(strcmp(led2State, "on") == 0 ) digitalWrite(led2,1); else digitalWrite(led2,0);
  }
}
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client", mqtt_username, mqtt_password)) {
      Serial.println("connected");
      for (int i = 0; i < 2; i++) {
        client.subscribe(mqtt_topics[i]);
        Serial.print("Subscribed to: ");
        Serial.println(mqtt_topics[i]);
      }
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  dht.begin();
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}
void sendSensorData() {
  StaticJsonDocument<200> doc;
  String temperatureStr = String(temperature, 1) + "C";
  String humidityStr = String(humidity, 1) + "%";
  String lightStr = String(light, 1) + "lux";
  doc["temperature"] = temperatureStr;
  doc["humidity"] = humidityStr;
  doc["light"] = lightStr;
  char jsonBuffer[200];
  serializeJson(doc, jsonBuffer);  // Chuyển dữ liệu thành chuỗi JSON

  // Gửi dữ liệu tới topic "sensor"
  client.publish("sensor", jsonBuffer);
  Serial.print("Published: ");
  Serial.println(jsonBuffer);
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsgTime > interval) {
    lastMsgTime = now;
    sendSensorData();
  }
}