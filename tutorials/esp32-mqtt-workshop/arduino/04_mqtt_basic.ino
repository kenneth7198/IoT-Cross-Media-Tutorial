// 04_mqtt_basic.ino - ESP32 連線 MQTT，發布光敏值、接收 LED 指令
#include <WiFi.h>
#include <PubSubClient.h>

// ---- WiFi 與 MQTT 設定 ----
const char* WIFI_SSID = "NetArt";
const char* WIFI_PASS = "1qaz2wsx";

// 固定 IP 設定
IPAddress local_IP(192, 168, 100, 201);
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway(192, 168, 100, 1);
IPAddress dns(8, 8, 8, 8);

const char* MQTT_HOST = "192.168.100.1"; // MQTT Broker IP
const uint16_t MQTT_PORT = 1883;
const char* DEVICE_ID = "esp32-01";  // 每個裝置要不同！

// 腳位
const int LED_PIN = 23;
const int ADC_PIN = 34;

WiFiClient espClient;
PubSubClient mqtt(espClient);
unsigned long lastMs = 0;

String topicSensor(){
  return String("iot/lab/") + DEVICE_ID + "/sensor/light";
}
String topicStatus(){
  return String("iot/lab/") + DEVICE_ID + "/status";
}
String topicCmdLed(){
  return String("iot/lab/") + DEVICE_ID + "/cmd/led";
}

void onMqtt(char* topic, byte* payload, unsigned int len){
  String t = topic; String p;
  for (unsigned int i=0;i<len;i++) p += (char)payload[i];
  Serial.printf("[MQTT] %s : %s\n", t.c_str(), p.c_str());
  if (t == topicCmdLed()){
    int val = 0; // 預設 OFF
    if (p.indexOf("1") >= 0 || p.indexOf("true") >= 0) val = 1;
    digitalWrite(LED_PIN, val ? HIGH : LOW);
  }
}

void ensureWiFi(){
  if (WiFi.status() == WL_CONNECTED) return;
  WiFi.mode(WIFI_STA);
  
  // 設定固定 IP
  if (!WiFi.config(local_IP, gateway, subnet, dns)) {
    Serial.println("Static IP Failed!");
  }
  
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("WiFi connecting");
  while (WiFi.status() != WL_CONNECTED){ 
    delay(300); 
    Serial.print("."); 
  }
  Serial.println(" OK");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void ensureMqtt(){
  if (mqtt.connected()) return;
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(onMqtt);
  while (!mqtt.connected()){
    Serial.print("MQTT...");
    if (mqtt.connect(DEVICE_ID)){
      Serial.println("OK");
      mqtt.subscribe(topicCmdLed().c_str(), 1);
      // 上線狀態
      String st = String("{\"deviceId\":\"") + DEVICE_ID + "\",\"status\":\"online\",\"ts\":" + String((uint32_t)millis()) + "}";
      mqtt.publish(topicStatus().c_str(), st.c_str(), true);
    } else { Serial.print("fail "); Serial.println(mqtt.state()); delay(1000); }
  }
}

void setup(){
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  ensureWiFi();
  ensureMqtt();
}

void loop(){
  ensureWiFi();
  ensureMqtt();
  mqtt.loop();

  unsigned long now = millis();
  if (now - lastMs > 2000){
    lastMs = now;
    int v = analogRead(ADC_PIN);
    String payload = String("{\"deviceId\":\"") + DEVICE_ID + "\",\"type\":\"light\",\"value\":" + String(v) + ",\"unit\":\"adc\",\"ts\":" + String((uint32_t)millis()) + "}";
    mqtt.publish(topicSensor().c_str(), payload.c_str());
  }
}
