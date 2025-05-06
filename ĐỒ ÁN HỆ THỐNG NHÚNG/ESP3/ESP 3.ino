#include <painlessMesh.h>

#define MESH_PREFIX "MESH"
#define MESH_PASSWORD "12345678"
#define MESH_PORT 5555

#define LED_PIN 5  // Chân điều khiển đèn (có thể đổi thành chân relay)

// Biến lưu trạng thái đèn
bool isLightOn = false;
unsigned long lightStartTime = 0;

Scheduler userScheduler;
painlessMesh mesh;
HardwareSerial mySerial(2);

void receivedCallback(uint32_t from, String &msg) {
    Serial.printf("Received from %u: %s\n", from, msg.c_str());

    // Gửi dữ liệu qua UART
    mySerial.println(msg);
    Serial.println("Sent to UART: " + msg);

    // Kiểm tra nếu tin nhắn là "motion:1"
    if (msg == "MOTION,1") {
        digitalWrite(LED_PIN, HIGH);  // Bật đèn
        isLightOn = true;
        lightStartTime = millis();  // Ghi lại thời gian bật đèn
        Serial.println("Light ON for 30s");
    }
}

void setup() {
    Serial.begin(115200);
    mySerial.begin(9600, SERIAL_8N1, 16, 17);
    

    // Cấu hình chân LED là OUTPUT
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);  // Đèn tắt ban đầu

    // Khởi tạo WiFi Mesh
    mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
    mesh.onReceive(&receivedCallback);
}

void loop() {
    mesh.update();

    // Kiểm tra nếu đèn đã bật và quá 30 giây thì tắt
    if (isLightOn && millis() - lightStartTime >= 30000) {
        digitalWrite(LED_PIN, LOW);  // Tắt đèn
        isLightOn = false;
        Serial.println("Light OFF after 30s");
    }
}
