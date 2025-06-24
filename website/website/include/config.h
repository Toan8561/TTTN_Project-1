#ifndef CONFIG_H
#define CONFIG_H

// Cấu hình mạng (DHCP)
#define MAC_ADDRESS { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED } // Giữ nguyên MAC address
// Bỏ các dòng IP tĩnh (IP, Gateway, Subnet, DNS) vì DHCP sẽ tự động cấp

// Cấu hình Firebase
#define FIREBASE_HOST "arduino-monitoring-e48ae-default-rtdb.firebaseio.com"

// Các chân cảm biến
#define TEMP_SENSOR_PIN A0
#define LIGHT_SENSOR_PIN A1
#define MOTION_SENSOR_PIN 2

// Các chân điều khiển
#define LED_PIN 13
#define BUZZER_PIN 8
#define SERVO_PIN 9

// Cấu hình thời gian
#define UPDATE_INTERVAL 5000  // 5 giây
#define COMMAND_CHECK_INTERVAL 1000  // 1 giây

// Cấu hình cảm biến
#define TEMP_OFFSET 0.0  // Độ lệch nhiệt độ
#define LIGHT_THRESHOLD 500  // Ngưỡng ánh sáng

// Cấu hình cảnh báo
#define TEMP_HIGH_THRESHOLD 35.0  // Nhiệt độ cao
#define TEMP_LOW_THRESHOLD 10.0   // Nhiệt độ thấp
#define LIGHT_LOW_THRESHOLD 200   // Ánh sáng thấp

// Cấu hình debug
#define DEBUG_MODE true
#define SERIAL_BAUD_RATE 57600

// Cấu hình JSON
#define JSON_DOCUMENT_SIZE 1024

#endif // CONFIG_H