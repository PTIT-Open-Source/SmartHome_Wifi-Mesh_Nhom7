#include <painlessMesh.h>
#include <DHT.h>

#define DHTPIN 18
#define DHTTYPE DHT11
#define PIR_PIN 19  

DHT dht(DHTPIN, DHTTYPE);

// Cấu hình mạng Mesh
#define MESH_PREFIX "MESH"
#define MESH_PASSWORD "12345678"
#define MESH_PORT 5555

Scheduler userScheduler;
painlessMesh mesh;

uint32_t esp2_id = 882471281;  // Lưu ID của ESP2

void sendSensorData();
Task taskSendData(TASK_SECOND * 10, TASK_FOREVER, &sendSensorData);

int lastMotionState = 0;
bool motionDetected = false;
unsigned long motionStartTime = 0;
const unsigned long motionDuration = 10000; 

// Xử lý khi nhận dữ liệu từ các node khác
void receivedCallback(uint32_t from, String &msg) {
    Serial.printf("ESP2 received from %u: %s\n", from, msg.c_str());
}

void sendSensorData() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    int light = random(100, 1000);  // Giá trị ánh sáng ngẫu nhiên

    String msg = String(temperature) + "," + String(humidity) + "," + String(light);
        mesh.sendSingle(esp2_id, msg);
        Serial.println("Sent to ESP2: " + msg);
}

void checkMotion() {
    int motion = digitalRead(PIR_PIN);

    if (motion == 1 && lastMotionState == 0) {  
        motionDetected = true;
        motionStartTime = millis();
        String msg = "MOTION,1";
        mesh.sendSingle(esp2_id, msg);
        Serial.println("Sent motion to ESP2: " + msg);
    }
    if (motionDetected && millis() - motionStartTime > motionDuration) {
        motionDetected = false;
        String msg = "MOTION,0";
        mesh.sendSingle(esp2_id, msg);
        Serial.println("Sent motion to ESP2: " + msg);
    }

    lastMotionState = motion;
}

void setup() {
    Serial.begin(115200);
    dht.begin();
    pinMode(PIR_PIN, INPUT);  
    randomSeed(analogRead(0)); 

    // Khởi tạo WiFi Mesh
    mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
    mesh.onReceive(&receivedCallback);

    userScheduler.addTask(taskSendData);
    taskSendData.enable();
}

void loop() {
    mesh.update();
    checkMotion();  // Kiểm tra motion liên tục
}
