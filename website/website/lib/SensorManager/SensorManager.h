#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>

class SensorManager {
private:
    // Cảm biến pins
    int tempPin;
    int lightPin;
    int motionPin;
    
    // Giá trị cảm biến
    float temperature;
    int lightLevel;
    bool motionDetected;
    
    // Cấu hình
    float tempOffset;
    int lightThreshold;
    
    // Thời gian
    unsigned long lastReadTime;
    unsigned long readInterval;

public:
    // Constructor
    SensorManager(int tempPin = A0, int lightPin = A1, int motionPin = 2);
    
    // Khởi tạo
    void begin();
    
    // Đọc cảm biến
    void readSensors();
    
    // Lấy giá trị
    float getTemperature() const { return temperature; }
    int getLightLevel() const { return lightLevel; }
    bool isMotionDetected() const { return motionDetected; }
    
    // Cấu hình
    void setTempOffset(float offset) { tempOffset = offset; }
    void setLightThreshold(int threshold) { lightThreshold = threshold; }
    void setReadInterval(unsigned long interval) { readInterval = interval; }
    
    // Kiểm tra cảnh báo
    bool isTempHigh() const;
    bool isTempLow() const;
    bool isLightLow() const;
    
    // Debug
    void printSensorData();
};

#endif // SENSOR_MANAGER_H