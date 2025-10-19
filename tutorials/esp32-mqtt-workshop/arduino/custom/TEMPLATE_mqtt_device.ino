// TEMPLATE_mqtt_device.ino - 將你的現成程式貼入此檔案後改名
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h" // 請由 config.example.h 複製為 config.h 並填入

// 固定 IP 設定
IPAddress local_IP(192, 168, 100, 201);
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway(192, 168, 100, 1);
IPAddress dns(8, 8, 8, 8);

WiFiClient espClient;
PubSubClient mqtt(espClient);

String topicBase(){ return String("iot/lab/") + DEVICE_ID; }
String tpSensor(const char* type){ return topicBase() + "/sensor/" + type; }
String tpCmd(const char* act){ return topicBase() + "/cmd/" + act; }
String tpStatus(){ return topicBase() + "/status"; }

void ensureWiFi(){
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
