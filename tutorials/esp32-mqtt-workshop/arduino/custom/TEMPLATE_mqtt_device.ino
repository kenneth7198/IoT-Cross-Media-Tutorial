// TEMPLATE_mqtt_device.ino - 將你的現成程式貼入此檔案後改名
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h" // 請由 config.example.h 複製為 config.h 並填入

WiFiClient espClient;
PubSubClient mqtt(espClient);

String topicBase(){ return String("iot/lab/") + DEVICE_ID; }
String tpSensor(const char* type){ return topicBase() + "/sensor/" + type; }
String tpCmd(const char* act){ return topicBase() + "/cmd/" + act; }
String tpStatus(){ return topicBase() + "/status"; }

void ensureWiFi(){
  if (WiFi.status() == WL_CONNECTED) return;
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) { delay(300); }
}

void onMqtt(char* topic, byte* payload, unsigned int len){
  // TODO: 依你的程式解析控制訊息，如 LED 或馬達
}

void ensureMqtt(){
  if (mqtt.connected()) return;
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(onMqtt);
  while (!mqtt.connected()){ if (mqtt.connect(DEVICE_ID)) mqtt.subscribe((tpCmd("#")).c_str(), 1); else delay(1000); }
}

void setup(){ Serial.begin(115200); ensureWiFi(); ensureMqtt(); }

void loop(){ ensureWiFi(); ensureMqtt(); mqtt.loop(); /* 在此發布你的感測資料或處理控制 */ }
