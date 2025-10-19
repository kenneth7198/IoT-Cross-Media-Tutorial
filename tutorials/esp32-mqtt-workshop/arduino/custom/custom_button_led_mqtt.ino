// custom_button_led_mqtt.ino
// 功能：微動開關控制 LED，並可透過 MQTT/WebSocket（手機）遠端控制 LED
// 腳位：LED = GPIO23，按鈕 = GPIO22
// MQTT topic：iot/lab/esp32-01/cmd/led
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

const int ledPin = 23;
const int inputButton = 22;

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
  pinMode(ledPin, OUTPUT);
  pinMode(inputButton, INPUT);
  Serial.begin(115200);
  ensureWiFi();
  ensureMqtt();
}

void loop() {
  ensureWiFi();
  ensureMqtt();
  mqtt.loop();
  int val = digitalRead(inputButton);
  // 本地按鈕優先
  digitalWrite(ledPin, val ? HIGH : LOW);
  Serial.println(val);
  delay(100);
}
