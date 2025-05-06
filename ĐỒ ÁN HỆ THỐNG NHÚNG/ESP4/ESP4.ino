#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <HardwareSerial.h>

#define led1 15
#define led2 4

HardwareSerial mySerial(2);  // UART2 (RX2: GPIO16, TX2: GPIO17)

const char* ssid = "Redmi 10";
const char* password = "05102003";
const char* mqtt_server = "192.168.180.26";
const int mqtt_port = 1885;
const char* mqtt_username = "qwer";
const char* mqtt_password = "123";
const char* mqtt_topics[] = {"sensor", "led"};

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsgTime = 0;
const long interval = 10000;

void setup_wifi() {
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
    Serial.println("\nWiFi connected");
    Serial.print("IP address: ");
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
        digitalWrite(led1, HIGH);
        digitalWrite(led2, HIGH);
        return;
    }
    if (messageTemp == "all_off") {
        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        return;
    }

    StaticJsonDocument<300> doc;
    DeserializationError error = deserializeJson(doc, messageTemp);
    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.f_str());
        return;
    }

    if (doc.containsKey("led1")) {
        const char* led1State = doc["led1"];
        digitalWrite(led1, strcmp(led1State, "on") == 0 ? HIGH : LOW);
    }

    if (doc.containsKey("led2")) {
        const char* led2State = doc["led2"];
        digitalWrite(led2, strcmp(led2State, "on") == 0 ? HIGH : LOW);
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
    Serial.begin(115200);
    mySerial.begin(9600, SERIAL_8N1, 16, 17);  // UART2 với RX2 = 16, TX2 = 17
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);

    setup_wifi();
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
}

void sendSensorData(String sensorData) {
    StaticJsonDocument<200> doc;
    
    // Tách dữ liệu dựa trên dấu phẩy
    int firstComma = sensorData.indexOf(',');
    int secondComma = sensorData.indexOf(',', firstComma + 1);

    if (firstComma == -1 || secondComma == -1) {
        Serial.println("Invalid sensor data format!");
        return;
    }

    String temperatureStr = sensorData.substring(0, firstComma);
    String humidityStr = sensorData.substring(firstComma + 1, secondComma);
    String lightStr = sensorData.substring(secondComma + 1);
    doc["temperature"] = temperatureStr;
    doc["humidity"] = humidityStr;
    doc["light"] = lightStr;

    char jsonBuffer[200];
    serializeJson(doc, jsonBuffer);

    client.publish("sensor", jsonBuffer);
    Serial.print("Published: ");
    Serial.println(jsonBuffer);
}


void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    if (mySerial.available()) {
        String receivedData = mySerial.readStringUntil('\n');  // Đọc dữ liệu từ UART
        receivedData.trim();  // Loại bỏ ký tự xuống dòng hoặc khoảng trắng
        if (receivedData.length() > 0) {
            Serial.print("Received from UART: ");
            Serial.println(receivedData);
            sendSensorData(receivedData);
        }
    }

    unsigned long now = millis();
    if (now - lastMsgTime > interval) {
        lastMsgTime = now;
        sendSensorData("No new data");
    }
}