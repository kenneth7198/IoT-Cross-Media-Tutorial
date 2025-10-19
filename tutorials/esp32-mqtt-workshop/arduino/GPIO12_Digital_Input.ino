// GPIO 12 數位輸入範例（按鈕）配合 MQTT
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
const char* DEVICE_ID = "esp32-button-01";

// 硬體設定
const int BUTTON_PIN = 12;  // ✅ GPIO 12 作為數位輸入（按鈕）
const int LED_PIN = 2;      // 板載 LED

// MQTT 主題
const char* TOPIC_BUTTON = "sensor/button";

WiFiClient espClient;
PubSubClient mqtt(espClient);

// 按鈕狀態
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n=== ESP32 GPIO 12 數位輸入測試 ===");
  
  // 設定 GPIO 12 為輸入，使用內部上拉
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // 測試 GPIO 12 數位讀取
  Serial.print("WiFi 連接前，GPIO 12 數位讀取: ");
  int testValue = digitalRead(BUTTON_PIN);
  Serial.println(testValue ? "HIGH" : "LOW");
  
  // 連接 WiFi
  connectWiFi();
  
  // WiFi 連接後再次測試
  Serial.print("WiFi 連接後，GPIO 12 數位讀取: ");
  testValue = digitalRead(BUTTON_PIN);
  Serial.println(testValue ? "HIGH (✅ 正常)" : "LOW (✅ 正常)");
  Serial.println("✅ GPIO 12 的數位輸入功能不受 WiFi 影響！");
  
  // 設定 MQTT
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  connectMQTT();
  
  Serial.println("\n初始化完成！");
  Serial.println("按下連接到 GPIO 12 的按鈕進行測試...\n");
}

void loop() {
  // 確保連線
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }
  if (!mqtt.connected()) {
    connectMQTT();
  }
  mqtt.loop();
  
  // 讀取按鈕狀態（帶防彈跳）
  int reading = digitalRead(BUTTON_PIN);
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW) {  // 按鈕被按下（上拉，按下為 LOW）
      digitalWrite(LED_PIN, HIGH);
      
      // 發送按鈕事件到 MQTT
      String message = "pressed";
      mqtt.publish(TOPIC_BUTTON, message.c_str());
      
      Serial.println("✓ 按鈕按下！發送: pressed");
      
      // 等待按鈕釋放
      while (digitalRead(BUTTON_PIN) == LOW) {
        delay(10);
      }
      
      digitalWrite(LED_PIN, LOW);
      message = "released";
      mqtt.publish(TOPIC_BUTTON, message.c_str());
      Serial.println("✓ 按鈕釋放！發送: released");
    }
  }
  
  lastButtonState = reading;
  delay(10);
}

void connectWiFi() {
  Serial.print("正在連接 WiFi: ");
  Serial.println(WIFI_SSID);
  
  WiFi.mode(WIFI_STA);
  
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
}

void connectMQTT() {
  Serial.print("正在連接 MQTT Broker...");
  
  while (!mqtt.connected()) {
    Serial.print(".");
    
    if (mqtt.connect(DEVICE_ID)) {
      Serial.println("\nMQTT 連接成功！");
      Serial.print("設備 ID: ");
      Serial.println(DEVICE_ID);
      Serial.print("發布主題: ");
      Serial.println(TOPIC_BUTTON);
    } else {
      Serial.print("\nMQTT 連接失敗，錯誤代碼: ");
      Serial.println(mqtt.state());
      Serial.println("5 秒後重試...");
      delay(5000);
    }
  }
}

/*
 * ========== 使用說明 ==========
 * 
 * ✅ GPIO 12 作為數位輸入是可以的！
 * 
 * ADC2 與 WiFi 衝突只影響 analogRead()，不影響 digitalRead()
 * 
 * 1. 硬體連接：
 *    - 按鈕一端接 GPIO 12
 *    - 按鈕另一端接 GND
 *    - 使用內部上拉電阻（INPUT_PULLUP）
 *    - 不需要外部電阻
 * 
 * 2. 工作原理：
 *    - 未按下：GPIO 12 = HIGH (3.3V，內部上拉)
 *    - 按下：GPIO 12 = LOW (0V，接地)
 * 
 * 3. 注意事項：
 *    ⚠️ 啟動時不要外接上拉到 3.3V
 *    ✅ 使用內部上拉（INPUT_PULLUP）
 *    ✅ 或使用下拉到 GND
 * 
 * 4. 測試方法：
 *    - 開啟序列監控器（115200）
 *    - 按下按鈕，觀察 LED 和訊息
 *    - 訂閱 MQTT：mosquitto_sub -h 192.168.100.1 -t "sensor/button" -v
 * 
 * 5. GPIO 12 功能對照：
 *    digitalRead(12)  ✅ WiFi 開啟時正常
 *    digitalWrite(12) ✅ WiFi 開啟時正常
 *    analogRead(12)   ❌ WiFi 開啟時無法使用
 */
