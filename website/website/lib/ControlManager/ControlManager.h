#ifndef CONTROL_MANAGER_H
#define CONTROL_MANAGER_H

#include <Arduino.h>
#include <Servo.h>

class ControlManager {
private:
    // Các chân điều khiển
    int ledPin;
    int buzzerPin;
    int servoPin;
    
    // Đối tượng servo
    Servo myServo;
    
    // Trạng thái hiện tại
    bool ledState;
    bool buzzerState;
    int servoAngle;
    
    // Cấu hình
    int servoMinAngle;
    int servoMaxAngle;

public:
    // Constructor
    ControlManager(int ledPin = 13, int buzzerPin = 8, int servoPin = 9);
    
    // Khởi tạo
    void begin();
    
    // Điều khiển LED
    void setLed(bool state);
    void toggleLed();
    void blinkLed(int times = 5, int delayMs = 200);
    
    // Điều khiển Buzzer
    void setBuzzer(bool state);
    void beepBuzzer(int durationMs = 200);
    
    // Điều khiển Servo
    void setServoAngle(int angle);
    void setServoRange(int minAngle, int maxAngle);
    
    // Lấy trạng thái
    bool getLedState() const { return ledState; }
    bool getBuzzerState() const { return buzzerState; }
    int getServoAngle() const { return servoAngle; }
    
    // Xử lý lệnh từ Firebase
    void executeCommand(const String& command, const String& value);
    
    // Debug
    void printStatus();

private:
    // Kiểm tra góc servo hợp lệ
    bool isValidServoAngle(int angle) const;
};

#endif // CONTROL_MANAGER_H