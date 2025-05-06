#include <painlessMesh.h>

#define MESH_PREFIX "MESH"
#define MESH_PASSWORD "12345678"
#define MESH_PORT 5555

#define LED_PIN 2  // GPIO2 (D4 trên một số board NodeMCU)

Scheduler userScheduler;
painlessMesh mesh;

uint32_t esp3_id = 3078376273; // ID của ESP3

unsigned long lastReceivedTime = 0;  // thời gian nhận gói tin cuối
const unsigned long TIMEOUT = 10000; // 10 giây

// Xử lý khi nhận dữ liệu từ ESP1
void receivedCallback(uint32_t from, String &msg) {
    Serial.printf("Received from %u: %s\n", from, msg.c_str());

    lastReceivedTime = millis();  // cập nhật thời gian nhận cuối

    if (esp3_id != 0) {
        mesh.sendSingle(esp3_id, msg);
        Serial.printf("Forwarded to ESP3 (%u): %s\n", esp3_id, msg.c_str());
    } else {
        Serial.println("ESP3 ID not found, cannot forward data.");
    }
}

void setup() {
    Serial.begin(115200);

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW); // tắt đèn lúc đầu

    mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
    mesh.onReceive(&receivedCallback);

    lastReceivedTime = millis();  // khởi tạo thời gian ban đầu
}

void loop() {
    mesh.update();

    if (millis() - lastReceivedTime > TIMEOUT) {
        digitalWrite(LED_PIN, HIGH);  // bật đèn nếu quá thời gian
    } else {
        digitalWrite(LED_PIN, LOW);   // tắt đèn nếu có nhận dữ liệu
    }
}
