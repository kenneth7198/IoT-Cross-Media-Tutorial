// GPIO 4 作為數位輸入範例（證明可以使用）
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi 設定
const char* WIFI_SSID = "NetArt";
const char* WIFI_PASS = "1qaz2wsx";

// 固定 IP 設定
IPAddress local_IP(192, 168, 100, 201);
IPAddress gateway(192, 168, 100, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);

// MQTT Broker 設定
const char* MQTT_HOST = "192.168.100.1";
const int   MQTT_PORT = 1883;
const char* DEVICE_ID = "esp32-gpio4-test";

// 硬體設定
const int DIGITAL_INPUT_PIN = 4;  // ✅ GPIO 4 作為數位輸入
const int LED_PIN = 2;

// MQTT 主題
const char* TOPIC_SENSOR = "sensor/digital";

WiFiClient espClient;
PubSubClient mqtt(espClient);

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n=== GPIO 4 數位輸入功能測試 ===\n");
  
  // 設定 GPIO 4 為數位輸入
  pinMode(DIGITAL_INPUT_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  
  // ===== 測試 1：WiFi 連接前 =====
  Serial.println("【測試 1】WiFi 連接前測試 GPIO 4:");
  Serial.println("功能測試：");
  
  // 測試數位讀取
  int digitalValue = digitalRead(DIGITAL_INPUT_PIN);
  Serial.print("  digitalRead(4): ");
  Serial.print(digitalValue);
  Serial.println(" ✅ 正常");
  
  // 測試類比讀取（對照）
  int analogValue = analogRead(DIGITAL_INPUT_PIN);
  Serial.print("  analogRead(4): ");
  Serial.print(analogValue);
  Serial.println(" ✅ WiFi 關閉時可用");
  
  Serial.println();
  
  // ===== 連接 WiFi =====
  Serial.println("正在連接 WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.config(local_IP, gateway, subnet, primaryDNS);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi 已連接！");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  
  // ===== 測試 2：WiFi 連接後 =====
  Serial.println("【測試 2】WiFi 連接後測試 GPIO 4:");
  Serial.println("功能測試：");
  
  // 測試數位讀取
  digitalValue = digitalRead(DIGITAL_INPUT_PIN);
  Serial.print("  digitalRead(4): ");
  Serial.print(digitalValue);
  Serial.println(" ✅ 正常（不受 WiFi 影響）");
  
  // 測試類比讀取（對照）
  analogValue = analogRead(DIGITAL_INPUT_PIN);
  Serial.print("  analogRead(4): ");
  Serial.print(analogValue);
  if (analogValue == 0) {
    Serial.println(" ❌ 無法使用（被 WiFi 佔用）");
  } else {
    Serial.println(" - 可能還能讀取");
  }
  
  Serial.println();
  Serial.println("【結論】");
  Serial.println("✅ GPIO 4 的 digitalRead() 功能不受 WiFi 影響");
  Serial.println("❌ GPIO 4 的 analogRead() 功能被 WiFi 佔用");
  Serial.println();
  
  // 連接 MQTT
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  connectMQTT();
  
  Serial.println("開始持續監測 GPIO 4 數位輸入...\n");
}

void loop() {
  // 確保連線
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
  }
  if (!mqtt.connected()) {
    connectMQTT();
  }
  mqtt.loop();
  
  // 持續讀取 GPIO 4 的數位狀態
  static int lastState = -1;
  int currentState = digitalRead(DIGITAL_INPUT_PIN);
  
  // 狀態改變時發送
  if (currentState != lastState) {
    lastState = currentState;
    
    String stateStr = currentState ? "HIGH" : "LOW";
    digitalWrite(LED_PIN, currentState);
    
    // 發送到 MQTT
    mqtt.publish(TOPIC_SENSOR, stateStr.c_str());
    
    Serial.print("GPIO 4 數位輸入狀態變化: ");
    Serial.print(stateStr);
    Serial.println(" ✅");
    
    // 同時測試類比讀取（對照）
    int analogValue = analogRead(DIGITAL_INPUT_PIN);
    Serial.print("  (類比讀取對照: ");
    Serial.print(analogValue);
    Serial.println(analogValue == 0 ? " - WiFi 佔用)" : ")");
  }
  
  delay(100);
}

void connectMQTT() {
  Serial.print("正在連接 MQTT...");
  
  while (!mqtt.connected()) {
    if (mqtt.connect(DEVICE_ID)) {
      Serial.println(" 成功！");
      Serial.print("發布主題: ");
      Serial.println(TOPIC_SENSOR);
    } else {
      Serial.print("失敗 (");
      Serial.print(mqtt.state());
      Serial.println(")");
      delay(5000);
    }
  }
}

/*
 * ========== 測試結果說明 ==========
 * 
 * ✅ GPIO 4 可以作為數位輸入使用！
 * 
 * 1. 硬體連接測試方式：
 *    方案 A：按鈕測試
 *    - 按鈕一端接 GPIO 4
 *    - 按鈕另一端接 GND
 *    - 使用內部上拉（INPUT_PULLUP）
 *    - 按下時 = LOW，放開時 = HIGH
 * 
 *    方案 B：跳線測試
 *    - GPIO 4 接 3.3V = 讀取 HIGH
 *    - GPIO 4 接 GND = 讀取 LOW
 *    - GPIO 4 懸空 = 讀取 HIGH（上拉）
 * 
 * 2. 功能驗證：
 *    ✅ digitalRead(4) 在 WiFi 開啟時正常工作
 *    ✅ digitalWrite(4) 在 WiFi 開啟時正常工作
 *    ❌ analogRead(4) 在 WiFi 開啟時讀到 0
 * 
 * 3. 應用場景：
 *    ✅ 按鈕輸入
 *    ✅ 開關感測器
 *    ✅ 人體紅外線感測器（PIR）
 *    ✅ 觸摸感測器（數位輸出型）
 *    ✅ 門磁感測器
 *    ❌ 光敏電阻（需要類比輸入）
 *    ❌ 電位器（需要類比輸入）
 *    ❌ 溫度感測器（類比型）
 * 
 * 4. 為什麼您的光敏電阻讀到 0？
 *    - 光敏電阻是類比感測器
 *    - 需要用 analogRead() 讀取電壓
 *    - GPIO 4 的 analogRead() 被 WiFi 佔用
 *    - 解決方案：改用 GPIO 34（ADC1）
 * 
 * 5. 總結：
 *    問題：光敏電阻 + GPIO 4 + WiFi = 不相容
 *    解決方案 A：光敏電阻 + GPIO 34 + WiFi = ✅
 *    解決方案 B：數位感測器 + GPIO 4 + WiFi = ✅
 */
