#include "SensorManager.h"
#include <config.h>

SensorManager::SensorManager(int tempPin, int lightPin, int motionPin) 
    : tempPin(tempPin), lightPin(lightPin), motionPin(motionPin),
      temperature(0), lightLevel(0), motionDetected(false),
      tempOffset(TEMP_OFFSET), lightThreshold(LIGHT_THRESHOLD),
      lastReadTime(0), readInterval(1000) {
}

void SensorManager::begin() {
    pinMode(motionPin, INPUT);
    
    #if DEBUG_MODE
    Serial.println("SensorManager initialized");
    Serial.print("Temperature pin: "); Serial.println(tempPin);
    Serial.print("Light pin: "); Serial.println(lightPin);
    Serial.print("Motion pin: "); Serial.println(motionPin);
    #endif
}

void SensorManager::readSensors() {
    unsigned long currentTime = millis();
    
    // Kiểm tra thời gian đọc
    if (currentTime - lastReadTime < readInterval) {
        return;
    }
    
    // Đọc nhiệt độ (LM35)
    int tempRaw = analogRead(tempPin);
    temperature = (tempRaw * 5.0 / 1024.0) * 100.0 + tempOffset;
    
    // Đọc ánh sáng
    lightLevel = analogRead(lightPin);
    
    // Đọc cảm biến chuyển động
    motionDetected = digitalRead(motionPin) == HIGH;
    
    lastReadTime = currentTime;
    
    #if DEBUG_MODE
    printSensorData();
    #endif
}

bool SensorManager::isTempHigh() const {
    return temperature > TEMP_HIGH_THRESHOLD;
}

bool SensorManager::isTempLow() const {
    return temperature < TEMP_LOW_THRESHOLD;
}

bool SensorManager::isLightLow() const {
    return lightLevel < LIGHT_LOW_THRESHOLD;
}

void SensorManager::printSensorData() {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("°C, Light: ");
    Serial.print(lightLevel);
    Serial.print(", Motion: ");
    Serial.println(motionDetected ? "Detected" : "None");
}