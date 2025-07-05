#include "FirebaseManager.h"
#include <config.h>

FirebaseManager::FirebaseManager(const char* firebaseHost) 
    : host(firebaseHost), lastUpdateTime(0), updateInterval(UPDATE_INTERVAL), isConnected(false) {
}

void FirebaseManager::begin() {
    // Kiểm tra kết nối ban đầu
    isConnected = client.connect(host, 80);
    
    #if DEBUG_MODE
    if (isConnected) {
        Serial.println("Firebase connection established");
    } else {
        Serial.println("Failed to connect to Firebase");
    }
    #endif
    
    if (isConnected) {
        client.stop();
    }
}

bool FirebaseManager::sendSensorData(float temperature, int lightLevel, bool motionDetected) {
    unsigned long currentTime = millis();
    
    // Kiểm tra thời gian gửi
    if (currentTime - lastUpdateTime < updateInterval) {
        return false;
    }
    
    // Tạo JSON object
    StaticJsonDocument<JSON_DOCUMENT_SIZE> doc;
    doc["temperature"] = temperature;
    doc["lightLevel"] = lightLevel;
    doc["motionDetected"] = motionDetected;
    doc["timestamp"] = currentTime;
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    #if DEBUG_MODE
    Serial.println("Sending data to Firebase...");
    Serial.println(jsonString);
    #endif
    
    // Gửi dữ liệu
    bool success = sendHttpRequest("POST", "/arduino_data.json", jsonString);
    
    if (success) {
        lastUpdateTime = currentTime;
        #if DEBUG_MODE
        Serial.println("Data sent successfully!");
        #endif
    }
    
    return success;
}

bool FirebaseManager::sendAlert(const String& type, const String& message) {
    StaticJsonDocument<JSON_DOCUMENT_SIZE> doc;
    doc["type"] = type;
    doc["message"] = message;
    doc["timestamp"] = millis();
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    #if DEBUG_MODE
    Serial.println("Sending alert: " + message);
    #endif
    
    return sendHttpRequest("POST", "/alerts.json", jsonString);
}

bool FirebaseManager::checkControlCommands() {
    return sendHttpRequest("GET", "/arduino_control.json");
}

bool FirebaseManager::sendHttpRequest(const String& method, const String& path, const String& data) {
    if (!client.connect(host, 80)) {
        isConnected = false;
        return false;
    }
    
    isConnected = true;
    
    // Tạo HTTP request
    client.print(method + " " + path + " HTTP/1.1\r\n");
    client.print("Host: " + String(host) + "\r\n");
    
    if (data.length() > 0) {
        client.print("Content-Type: application/json\r\n");
        client.print("Content-Length: " + String(data.length()) + "\r\n");
    }
    
    client.print("Connection: close\r\n");
    client.print("\r\n");
    
    if (data.length() > 0) {
        client.print(data);
    }
    
    // Đọc response
    String response = "";
    while (client.available()) {
        char c = client.read();
        response += c;
    }
    
    client.stop();
    
    // Parse commands nếu là GET request
    if (method == "GET") {
        parseControlCommands(response);
    }
    
    return true;
}

void FirebaseManager::parseControlCommands(const String& response) {
    // Tìm JSON data trong response
    int jsonStart = response.indexOf('{');
    if (jsonStart == -1) return;
    
    String jsonData = response.substring(jsonStart);
    
    // Parse JSON
    DynamicJsonDocument doc(JSON_DOCUMENT_SIZE);
    DeserializationError error = deserializeJson(doc, jsonData);
    
    if (error) {
        #if DEBUG_MODE
        Serial.print("JSON parsing failed: ");
        Serial.println(error.c_str());
        #endif
        return;
    }
    
    // Xử lý từng lệnh điều khiển
    for (JsonPair kv : doc.as<JsonObject>()) {
        JsonObject command = kv.value();
        
        if (command.containsKey("command") && command.containsKey("value")) {
            String cmd = command["command"].as<String>();
            String value = command["value"].as<String>();
            
            #if DEBUG_MODE
            Serial.print("Executing command: ");
            Serial.print(cmd);
            Serial.print(" = ");
            Serial.println(value);
            #endif
            
            // Xóa lệnh đã thực thi
            deleteCommand(kv.key().c_str());
        }
    }
}

bool FirebaseManager::deleteCommand(const char* commandKey) {
    String path = "/arduino_control/" + String(commandKey) + ".json";
    return sendHttpRequest("DELETE", path);
}

void FirebaseManager::printConnectionStatus() {
    Serial.print("Firebase connection: ");
    Serial.println(isConnected ? "Connected" : "Disconnected");
}