#include "ControlManager.h"
#include <config.h>

ControlManager::ControlManager(int ledPin, int buzzerPin, int servoPin)
    : ledPin(ledPin), buzzerPin(buzzerPin), servoPin(servoPin),
      ledState(false), buzzerState(false), servoAngle(90),
      servoMinAngle(0), servoMaxAngle(180) {
}

void ControlManager::begin() {
    // Khởi tạo các chân
    pinMode(ledPin, OUTPUT);
    pinMode(buzzerPin, OUTPUT);
    
    // Khởi tạo servo
    myServo.attach(servoPin);
    myServo.write(servoAngle);
    
    // Đặt trạng thái ban đầu
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
    
    #if DEBUG_MODE
    Serial.println("ControlManager initialized");
    Serial.print("LED pin: "); Serial.println(ledPin);
    Serial.print("Buzzer pin: "); Serial.println(buzzerPin);
    Serial.print("Servo pin: "); Serial.println(servoPin);
    #endif
}

void ControlManager::setLed(bool state) {
    ledState = state;
    digitalWrite(ledPin, state ? HIGH : LOW);
    
    #if DEBUG_MODE
    Serial.print("LED set to: ");
    Serial.println(state ? "ON" : "OFF");
    #endif
}

void ControlManager::toggleLed() {
    setLed(!ledState);
}

void ControlManager::blinkLed(int times, int delayMs) {
    for (int i = 0; i < times; i++) {
        setLed(true);
        delay(delayMs);
        setLed(false);
        delay(delayMs);
    }
}

void ControlManager::setBuzzer(bool state) {
    buzzerState = state;
    digitalWrite(buzzerPin, state ? HIGH : LOW);
    
    #if DEBUG_MODE
    Serial.print("Buzzer set to: ");
    Serial.println(state ? "ON" : "OFF");
    #endif
}

void ControlManager::beepBuzzer(int durationMs) {
    setBuzzer(true);
    delay(durationMs);
    setBuzzer(false);
}

void ControlManager::setServoAngle(int angle) {
    if (isValidServoAngle(angle)) {
        servoAngle = angle;
        myServo.write(angle);
        
        #if DEBUG_MODE
        Serial.print("Servo set to angle: ");
        Serial.println(angle);
        #endif
    } else {
        #if DEBUG_MODE
        Serial.print("Invalid servo angle: ");
        Serial.println(angle);
        #endif
    }
}

void ControlManager::setServoRange(int minAngle, int maxAngle) {
    servoMinAngle = minAngle;
    servoMaxAngle = maxAngle;
    
    #if DEBUG_MODE
    Serial.print("Servo range set to: ");
    Serial.print(minAngle);
    Serial.print(" - ");
    Serial.println(maxAngle);
    #endif
}

void ControlManager::executeCommand(const String& command, const String& value) {
    #if DEBUG_MODE
    Serial.print("Executing command: ");
    Serial.print(command);
    Serial.print(" = ");
    Serial.println(value);
    #endif
    
    if (command == "LED") {
        if (value == "ON") {
            setLed(true);
        } else if (value == "OFF") {
            setLed(false);
        } else if (value == "TOGGLE") {
            toggleLed();
        }
    }
    else if (command == "BUZZER") {
        if (value == "ON") {
            setBuzzer(true);
        } else if (value == "OFF") {
            setBuzzer(false);
        } else if (value == "BEEP") {
            beepBuzzer(200);
        }
    }
    else if (command == "SERVO") {
        int angle = value.toInt();
        setServoAngle(angle);
    }
    else if (command == "LED_BLINK") {
        int times = value.toInt();
        if (times > 0) {
            blinkLed(times);
        } else {
            blinkLed();
        }
    }
    else if (command == "BUZZER_BEEP") {
        int duration = value.toInt();
        if (duration > 0) {
            beepBuzzer(duration);
        } else {
            beepBuzzer();
        }
    }
}

bool ControlManager::isValidServoAngle(int angle) const {
    return angle >= servoMinAngle && angle <= servoMaxAngle;
}

void ControlManager::printStatus() {
    Serial.println("=== Control Status ===");
    Serial.print("LED: "); Serial.println(ledState ? "ON" : "OFF");
    Serial.print("Buzzer: "); Serial.println(buzzerState ? "ON" : "OFF");
    Serial.print("Servo angle: "); Serial.println(servoAngle);
    Serial.println("=====================");
}