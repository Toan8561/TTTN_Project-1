#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <config.h>
#include "SensorManager.h"
#include "FirebaseManager.h"
#include "ControlManager.h"

// Khởi tạo các đối tượng
SensorManager sensors(TEMP_SENSOR_PIN, LIGHT_SENSOR_PIN, MOTION_SENSOR_PIN);
FirebaseManager firebase(FIREBASE_HOST);
ControlManager controls(LED_PIN, BUZZER_PIN, SERVO_PIN);

// Biến thời gian
unsigned long lastAlertCheck = 0;
const unsigned long ALERT_CHECK_INTERVAL = 10000; // 10 giây
unsigned long lastUpdateTime = 0;
unsigned long lastCommandCheckTime = 0;

void checkAlerts();
void printSystemStatus();
void initializeNetwork();
void handleDHCPEvents();

void setup() {
    // Khởi tạo Serial
    Serial.begin(SERIAL_BAUD_RATE);
    while (!Serial) {;} // Chờ cổng Serial sẵn sàng
    
    Serial.println("\n=== HỆ THỐNG GIÁM SÁT IOT ===");
    Serial.println("Chế độ mạng: DHCP");
    Serial.println("=============================");
    
    // Khởi tạo Ethernet với DHCP
    initializeNetwork();
    
    // Khởi tạo các module
    sensors.begin();
    firebase.begin();
    controls.begin();
    
    Serial.println("Hệ thống đã sẵn sàng!");
}

void loop() {
    // Xử lý sự kiện DHCP
    handleDHCPEvents();
    
    // Đọc cảm biến và gửi dữ liệu
    if (millis() - lastUpdateTime >= UPDATE_INTERVAL) {
        sensors.readSensors();
        if (firebase.sendSensorData(sensors.getTemperature(), 
                                  sensors.getLightLevel(), 
                                  sensors.isMotionDetected())) {
            lastUpdateTime = millis();
        }
    }
    
    // Kiểm tra lệnh điều khiển
    if (millis() - lastCommandCheckTime >= COMMAND_CHECK_INTERVAL) {
        firebase.checkControlCommands();
        lastCommandCheckTime = millis();
    }
    
    // Kiểm tra cảnh báo
    checkAlerts();
    
    // In trạng thái hệ thống
    static unsigned long lastStatusPrint = 0;
    if (millis() - lastStatusPrint > 30000) {
        printSystemStatus();
        lastStatusPrint = millis();
    }
    
    delay(10);
}

/*** Khởi tạo kết nối DHCP ***/
void initializeNetwork() {
    Serial.println("Đang kết nối mạng...");
    
    byte mac[] = MAC_ADDRESS;
    
    // Thử kết nối DHCP
    if (Ethernet.begin(mac) == 0) {
        Serial.println("Lỗi: Không thể kết nối DHCP!");
        Serial.println("Kiểm tra cáp mạng hoặc cấu hình router");
        while (true); // Dừng chương trình nếu không kết nối được
    }
    
    Serial.println("Kết nối DHCP thành công!");
    Serial.print("IP: "); Serial.println(Ethernet.localIP());
    Serial.print("Subnet: "); Serial.println(Ethernet.subnetMask());
    Serial.print("Gateway: "); Serial.println(Ethernet.gatewayIP());
}

/*** Xử lý sự kiện DHCP ***/
void handleDHCPEvents() {
    switch (Ethernet.maintain()) {
        case 1: // Renew failed
            Serial.println("Cảnh báo: Gia hạn DHCP thất bại");
            break;
            
        case 2: // Renew success
            Serial.print("DHCP gia hạn thành công. IP mới: ");
            Serial.println(Ethernet.localIP());
            break;
            
        case 3: // Rebind failed
            Serial.println("Lỗi: Không thể rebind DHCP");
            break;
            
        case 4: // Rebind success
            Serial.print("DHCP rebind thành công. IP mới: ");
            Serial.println(Ethernet.localIP());
            break;
    }
}

/*** Kiểm tra cảnh báo ***/
void checkAlerts() {
    if (millis() - lastAlertCheck < ALERT_CHECK_INTERVAL) return;
    
    if (sensors.isTempHigh()) {
        String msg = "CẢNH BÁO: Nhiệt độ cao " + String(sensors.getTemperature()) + "°C";
        firebase.sendAlert("warning", msg);
    }
    
    if (sensors.isTempLow()) {
        String msg = "CẢNH BÁO: Nhiệt độ thấp " + String(sensors.getTemperature()) + "°C";
        firebase.sendAlert("warning", msg);
    }
    
    if (sensors.isLightLow()) {
        firebase.sendAlert("info", "Ánh sáng yếu: " + String(sensors.getLightLevel()));
    }
    
    if (sensors.isMotionDetected()) {
        firebase.sendAlert("alert", "Phát hiện chuyển động!");
    }
    
    lastAlertCheck = millis();
}

/*** In trạng thái hệ thống ***/
void printSystemStatus() {
    Serial.println("\n--- TRẠNG THÁI HỆ THỐNG ---");
    Serial.print("Nhiệt độ: "); Serial.print(sensors.getTemperature()); Serial.println("°C");
    Serial.print("Ánh sáng: "); Serial.println(sensors.getLightLevel());
    Serial.print("Chuyển động: "); Serial.println(sensors.isMotionDetected() ? "CÓ" : "KHÔNG");
    Serial.print("IP hiện tại: "); Serial.println(Ethernet.localIP());
    Serial.println("--------------------------\n");
}