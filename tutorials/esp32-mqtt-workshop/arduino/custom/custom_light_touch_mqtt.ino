// custom_light_touch_mqtt.ino
// 功能：光敏電阻/電容感測值週期性發布到 MQTT，並可遠端控制 LED
// 腳位：光敏 = GPIO4，電容 = GPIO12，LED = GPIO23
// MQTT topic：iot/lab/esp32-01/sensor/light, iot/lab/esp32-01/sensor/touch, iot/lab/esp32-01/cmd/led
// 控制格式：{"cmd":"led","value":1}

#include <WiFi.h>
#include <PubSubClient.h>
#include "../config.h"

// 固定 IP 設定
IPAddress local_IP(192, 168, 100, 201);
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway(192, 168, 100, 1);
IPAddress dns(8, 8, 8, 8);

WiFiClient espClient;
PubSubClient mqtt(espClient);

const int touchPin = 12;
const int adcPin = 4;
const int ledPin = 23;
unsigned long lastMs = 0;

void onMqtt(char* topic, byte* payload, unsigned int len) {
  String t = topic;
  String p;
  for (unsigned int i=0;i<len;i++) p += (char)payload[i];
  if (t.endsWith("/cmd/led")) {
    int val = (p.indexOf("1") >= 0 || p.indexOf("true") >= 0) ? 1 : 0;
    digitalWrite(ledPin, val ? HIGH : LOW);
    Serial.printf("[MQTT] LED set to %d\n", val);
  }
}

void ensureWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;
  WiFi.mode(WIFI_STA);
  
  // 設定固定 IP
  if (!WiFi.config(local_IP, gateway, subnet, dns)) {
    Serial.println("Static IP Failed!");
  }
  
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("WiFi connecting");
  while (WiFi.status() != WL_CONNECTED) { 
    delay(300); 
    Serial.print("."); 
  }
  Serial.println(" OK");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void ensureMqtt() {
  if (mqtt.connected()) return;
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(onMqtt);
  while (!mqtt.connected()) {
    if (mqtt.connect(DEVICE_ID)) {
      mqtt.subscribe((String("iot/lab/") + DEVICE_ID + "/cmd/led").c_str(), 1);
    } else { delay(1000); }
  }
}

void setup() {
  pinMode(touchPin, INPUT);
  pinMode(adcPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(115200);
  ensureWiFi();
  ensureMqtt();
}

void loop() {
  ensureWiFi();
  ensureMqtt();
  mqtt.loop();
  unsigned long now = millis();
  if (now - lastMs > 2000) {
    lastMs = now;
    int touchVal = digitalRead(touchPin);
    int adcVal = analogRead(adcPin);
    String payload1 = String("{\"deviceId\":\"") + DEVICE_ID + "\",\"type\":\"light\",\"value\":" + String(adcVal) + ",\"unit\":\"adc\",\"ts\":" + String((uint32_t)millis()) + "}";
    String payload2 = String("{\"deviceId\":\"") + DEVICE_ID + "\",\"type\":\"touch\",\"value\":" + String(touchVal) + ",\"unit\":\"digital\",\"ts\":" + String((uint32_t)millis()) + "}";
    mqtt.publish((String("iot/lab/") + DEVICE_ID + "/sensor/light").c_str(), payload1.c_str());
    mqtt.publish((String("iot/lab/") + DEVICE_ID + "/sensor/touch").c_str(), payload2.c_str());
    Serial.println(payload1);
    Serial.println(payload2);
  }
  delay(50);
}
