// art_color_led_mqtt.ino
// 功能：接收 MQTT color 指令，控制 RGB LED 顯示對應顏色
// topic: iot/lab/esp32-01/cmd/color
// payload: {"cmd":"color","value":"#e74c3c"}
// 腳位：R=GPIO25, G=GPIO26, B=GPIO27（可依實際接線調整）

#include <WiFi.h>
#include <PubSubClient.h>
#include "../config.h"

const int PIN_R = 25;
const int PIN_G = 26;
const int PIN_B = 27;

WiFiClient espClient;
PubSubClient mqtt(espClient);

void setColor(uint32_t rgb) {
  int r = (rgb >> 16) & 0xFF;
  int g = (rgb >> 8) & 0xFF;
  int b = rgb & 0xFF;
  analogWrite(PIN_R, r);
  analogWrite(PIN_G, g);
  analogWrite(PIN_B, b);
}

uint32_t parseColor(const String& hex) {
  if (hex.length() != 7 || hex[0] != '#') return 0;
  return (uint32_t) strtol(hex.substring(1).c_str(), NULL, 16);
}

void onMqtt(char* topic, byte* payload, unsigned int len) {
  String t = topic;
  String p;
  for (unsigned int i=0;i<len;i++) p += (char)payload[i];
  if (t.endsWith("/cmd/color")) {
    int idx = p.indexOf("\"value\":");
    if (idx >= 0) {
      int q1 = p.indexOf('"', idx+8);
      int q2 = p.indexOf('"', q1+1);
      if (q1 >= 0 && q2 > q1) {
        String color = p.substring(q1+1, q2);
        uint32_t rgb = parseColor(color);
        setColor(rgb);
        Serial.printf("[MQTT] Set color: %s\n", color.c_str());
      }
    }
  }
}

void ensureWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) { delay(300); }
}

void ensureMqtt() {
  if (mqtt.connected()) return;
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(onMqtt);
  while (!mqtt.connected()) {
    if (mqtt.connect(DEVICE_ID)) {
      mqtt.subscribe((String("iot/lab/") + DEVICE_ID + "/cmd/color").c_str(), 1);
    } else { delay(1000); }
  }
}

void setup() {
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  analogWrite(PIN_R, 0);
  analogWrite(PIN_G, 0);
  analogWrite(PIN_B, 0);
  Serial.begin(115200);
  ensureWiFi();
  ensureMqtt();
}

void loop() {
  ensureWiFi();
  ensureMqtt();
  mqtt.loop();
  delay(50);
}
