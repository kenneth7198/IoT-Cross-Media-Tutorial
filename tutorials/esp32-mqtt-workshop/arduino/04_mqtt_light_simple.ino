// 04_mqtt_light_simple.ino - 超簡單範例：只發送光敏電阻數據到 MQTT
// 功能：每隔 1 秒讀取光敏電阻數值，並發布到 MQTT broker

#include <WiFi.h>
#include <PubSubClient.h>

// ==================== 設定區 ====================
// WiFi 設定
const char* WIFI_SSID = "NetArt";
const char* WIFI_PASS = "1qaz2wsx";

// 固定 IP 設定（必須設定）
IPAddress local_IP(192, 168, 100, 201);       // ESP32 的固定 IP
IPAddress gateway(192, 168, 100, 1);          // 路由器閘道
IPAddress subnet(255, 255, 255, 0);           // 子網路遮罩
IPAddress primaryDNS(8, 8, 8, 8);             // DNS 伺服器

// MQTT Broker 設定
const char* MQTT_HOST = "192.168.100.1";      // MQTT Broker IP
const int   MQTT_PORT = 1883;                 // MQTT Port
const char* DEVICE_ID = "esp32-light-01";     // 設備 ID（每台設備要不同）

// 硬體設定
const int LIGHT_SENSOR_PIN = 4;               // 光敏電阻接在 GPIO 4 (ADC2_CH0)
// ⚠️ 注意：GPIO 4 是 ADC2，WiFi 開啟時無法使用！
// 建議改用 ADC1 腳位：32, 33, 34, 35, 36, 39

// 發送間隔
const unsigned long SEND_INTERVAL = 1000;     // 每 1 秒發送一次 (毫秒)

// MQTT 主題
const char* TOPIC_LIGHT = "sensor/light";     // 發布光敏電阻數據的主題
// ===============================================

WiFiClient espClient;
PubSubClient mqtt(espClient);
unsigned long lastSendTime = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n=== ESP32 光敏電阻 MQTT 發送器 ===");
  
  // 設定 ADC 解析度和衰減
  analogReadResolution(12);  // 12-bit 解析度 (0-4095)
  analogSetAttenuation(ADC_11db);  // 設定衰減，讀取 0-3.3V 全範圍
  
  // ⚠️ 重要警告：GPIO 4 是 ADC2，WiFi 使用時會衝突
  Serial.println("⚠️⚠️⚠️ 警告：GPIO 4 是 ADC2 腳位 ⚠️⚠️⚠️");
  Serial.println("   WiFi 開啟時，ADC2 無法正常讀取！");
  Serial.println("   這會導致讀值一直為 0");
  Serial.println("   強烈建議改用 ADC1 腳位：32, 33, 34, 35, 36, 39");
  Serial.println();
  
  // 在 WiFi 連接前測試讀取（此時 ADC2 還可用）
  Serial.print("WiFi 連接前測試 (GPIO ");
  Serial.print(LIGHT_SENSOR_PIN);
  Serial.print("): ");
  int testValue = analogRead(LIGHT_SENSOR_PIN);
  Serial.println(testValue);
  
  if (testValue == 0) {
    Serial.println("⚠️ 警告：光敏電阻讀值為 0！");
    Serial.println("   請檢查：");
    Serial.println("   1. 光敏電阻是否正確連接到 GPIO 4");
    Serial.println("   2. 分壓電路是否正確 (光敏電阻 + 10kΩ)");
    Serial.println("   3. 環境是否太暗");
  }
  
  // 連接 WiFi
  connectWiFi();
  
  // 設定 MQTT
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  
  // 連接 MQTT Broker
  connectMQTT();
  
  Serial.println("初始化完成，開始發送光敏電阻數據...\n");
}

void loop() {
  // 確保 WiFi 和 MQTT 保持連線
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }
  
  if (!mqtt.connected()) {
    connectMQTT();
  }
  
  mqtt.loop();  // 處理 MQTT 訊息
  
  // 每隔指定時間發送光敏電阻數據
  unsigned long now = millis();
  if (now - lastSendTime >= SEND_INTERVAL) {
    lastSendTime = now;
    sendLightData();
  }
}

// 連接 WiFi（使用固定 IP）
void connectWiFi() {
  Serial.print("正在連接 WiFi: ");
  Serial.println(WIFI_SSID);
  
  WiFi.mode(WIFI_STA);
  
  // 設定固定 IP（必須在 WiFi.begin 之前）
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS)) {
    Serial.println("固定 IP 設定失敗！");
  } else {
    Serial.println("固定 IP 設定成功");
  }
  
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi 連接成功！");
  Serial.print("IP 位址: ");
  Serial.println(WiFi.localIP());
  Serial.print("閘道: ");
  Serial.println(gateway);
  Serial.print("訊號強度 (RSSI): ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}

// 連接 MQTT Broker
void connectMQTT() {
  Serial.print("正在連接 MQTT Broker...");
  
  while (!mqtt.connected()) {
    Serial.print(".");
    
    if (mqtt.connect(DEVICE_ID)) {
      Serial.println("\nMQTT 連接成功！");
      Serial.print("設備 ID: ");
      Serial.println(DEVICE_ID);
      Serial.print("發布主題: ");
      Serial.println(TOPIC_LIGHT);
    } else {
      Serial.print("\nMQTT 連接失敗，錯誤代碼: ");
      Serial.println(mqtt.state());
      Serial.println("5 秒後重試...");
      delay(5000);
    }
  }
}

// 讀取並發送光敏電阻數據
void sendLightData() {
  // 讀取光敏電阻 ADC 值 (0-4095)
  // 多次讀取取平均值，提高穩定性
  int sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += analogRead(LIGHT_SENSOR_PIN);
    delay(10);
  }
  int lightValue = sum / 5;
  
  // 簡單方法：直接發送數值（轉成字串）
  String message = String(lightValue);
  
  // 發布到 MQTT Broker
  bool success = mqtt.publish(TOPIC_LIGHT, message.c_str());
  
  // 在序列監控器顯示（包含原始讀值和電壓）
  float voltage = (lightValue / 4095.0) * 3.3;
  
  if (success) {
    Serial.print("✓ 發送成功 [");
    Serial.print(TOPIC_LIGHT);
    Serial.print("] 光敏值: ");
    Serial.print(lightValue);
    Serial.print(" (");
    Serial.print(voltage, 2);
    Serial.println("V)");
  } else {
    Serial.println("✗ 發送失敗");
  }
  
  // 診斷資訊：如果持續為 0
  if (lightValue == 0) {
    Serial.println("⚠️ 警告：光敏電阻讀值為 0！請檢查硬體連接。");
  }
}

/*
 * ========== 使用說明 ==========
 * 
 * ⚠️⚠️⚠️ 重要：GPIO 4 與 WiFi 衝突問題 ⚠️⚠️⚠️
 * 
 * GPIO 4 屬於 ADC2，當 WiFi 啟動後，ADC2 會被 WiFi 佔用，
 * 導致 analogRead() 無法正常工作，讀值會一直是 0。
 * 
 * 解決方案：
 * 方案 1（推薦）：改用 ADC1 腳位
 *    - GPIO 32, 33, 34, 35, 36, 39 都可以
 *    - 只需修改線路和程式碼中的 LIGHT_SENSOR_PIN
 * 
 * 方案 2（不推薦）：在 WiFi 連接前讀取
 *    - 只能在 WiFi 連接前使用 ADC2
 *    - 無法用於持續監測
 * 
 * 1. 硬體連接（目前使用 GPIO 4）：
 *    - 光敏電阻一端接 3.3V
 *    - 光敏電阻另一端接 GPIO 4 (ADC2_CH0) ⚠️ 與 WiFi 衝突
 *    - GPIO 4 與 GND 之間接 10kΩ 電阻（分壓電路）
 * 
 * 2. 設定步驟：
 *    ⚠️ 必須修改固定 IP 設定（避免 IP 衝突）
 *    - 修改 local_IP：每台 ESP32 要不同 (例如 .201, .202, .203)
 *    - 修改 gateway：你的路由器 IP (通常是 192.168.100.1)
 *    - 修改 WiFi SSID 和密碼
 *    - 修改 MQTT_HOST：MQTT Broker 的 IP
 *    - 修改 DEVICE_ID：每台設備要不同
 * 
 * 3. 上傳後測試：
 *    - 開啟序列監控器（115200 baud）
 *    - 確認固定 IP 設定成功
 *    - 確認 WiFi 和 MQTT 連接成功
 *    - 用手遮住光敏電阻，觀察數值變化
 * 
 * 4. 在電腦上訂閱 MQTT 訊息（測試接收）：
 *    mosquitto_sub -h 192.168.100.1 -t "sensor/light" -v
 * 
 *    你會看到：
 *    sensor/light 2048
 *    sensor/light 2156
 *    sensor/light 1823
 * 
 * 5. 數值範圍：
 *    - 0-4095 (ESP32 的 12-bit ADC)
 *    - 數值越大 → 光線越強（明亮環境）
 *    - 數值越小 → 環境越暗（用手遮住）
 * 
 * 6. 發送方式：
 *    - 使用 mqtt.publish() 發布訊息
 *    - 主題：sensor/light
 *    - 內容：純數值（例如 "2048"）
 *    - 每 1 秒發送一次
 */
