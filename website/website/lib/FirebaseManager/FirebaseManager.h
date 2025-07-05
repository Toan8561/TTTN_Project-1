#ifndef FIREBASE_MANAGER_H
#define FIREBASE_MANAGER_H

#include <Arduino.h>
#include <Ethernet.h>
#include <ArduinoJson.h>

class FirebaseManager {
private:
    EthernetClient client;
    const char* host;
    
    // Thời gian
    unsigned long lastUpdateTime;
    unsigned long updateInterval;
    
    // Trạng thái kết nối
    bool isConnected;

public:
    // Constructor
    FirebaseManager(const char* firebaseHost);
    
    // Khởi tạo
    void begin();
    
    // Gửi dữ liệu
    bool sendSensorData(float temperature, int lightLevel, bool motionDetected);
    bool sendAlert(const String& type, const String& message);
    
    // Nhận lệnh điều khiển
    bool checkControlCommands();
    
    // Kiểm tra kết nối
    bool isFirebaseConnected() const { return isConnected; }
    
    // Cấu hình
    void setUpdateInterval(unsigned long interval) { updateInterval = interval; }
    
    // Debug
    void printConnectionStatus();

private:
    // Parse JSON commands
    void parseControlCommands(const String& response);
    
    // Xóa lệnh đã thực thi
    bool deleteCommand(const char* commandKey);
    
    // Tạo HTTP request
    bool sendHttpRequest(const String& method, const String& path, const String& data = "");
};

#endif // FIREBASE_MANAGER_H