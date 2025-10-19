# ESP32 + MQTT + Processing 工作坊完整教學計畫

**總時長：4 小時（240 分鐘）**  
**目標學員：具備基礎程式設計經驗的學生/開發者**  
**教學目標：從零開始建立 IoT 視覺互動系統**

---

## 📋 課前準備檢查清單（講師用）

### 硬體準備
- [ ] ESP32 開發板 x N（每位學員 1 片）
- [ ] USB 傳輸線 x N
- [ ] LED、電阻、光敏電阻、按鈕、跳線若干
- [ ] 麵包板 x N

### 軟體準備
- [ ] Arduino IDE 2.x 已安裝 ESP32 支援
- [ ] Processing 4.x 已安裝 MQTT Library
- [ ] Node.js 已安裝（for MQTT Broker）
- [ ] 測試用 MQTT Broker 正常運作
- [ ] 網頁伺服器測試正常

### 教材準備
- [ ] 投影片準備完成
- [ ] 範例程式碼已測試
- [ ] 網路環境確認（WiFi SSID/密碼）
- [ ] 備用教材（USB 隨身碟）

### 網路設定
- [ ] WiFi 可用（記錄 SSID 和密碼）
- [ ] 講師電腦 IP 位址（寫在白板上）
- [ ] 防火牆允許 1883, 9001, 3000 port

---

## 🎯 課程大綱

| 時段 | 時長 | 主題 | 內容 |
|------|------|------|------|
| **Part 1** | 60min | IoT 基礎 + 環境設定 | IoT 概念、MQTT 協定、軟體安裝 |
| **Part 2** | 60min | ESP32 基礎互動 | LED、光敏、按鈕、感測器實作 |
| **休息** | 15min | 中場休息 | 咖啡時間 ☕ |
| **Part 3** | 60min | MQTT 整合 + 手機控制 | MQTT 通訊、網頁介面、手機搖晃 |
| **Part 4** | 45min | Processing 視覺藝術 | 視覺效果、色彩互動、創意挑戰 |

---

## 📖 詳細教學流程

---

## Part 1: IoT 基礎 + 環境設定（60 分鐘）

### ⏰ 00:00 - 00:15 | 開場與課程介紹（15 分鐘）

**教學內容**：
1. **自我介紹與課程目標**
   - 講師介紹
   - 今日目標：建立一個手機控制 LED，Processing 視覺化的 IoT 系統
   - 展示最終成果（Demo 影片）

2. **IoT 基本概念**
   - 什麼是 IoT？（Internet of Things）
   - IoT 三層架構：感知層、網路層、應用層
   - 常見應用：智慧家居、工業 4.0、智慧城市

3. **MQTT 協定介紹**
   - 為什麼選擇 MQTT？（輕量、雙向、即時）
   - Pub/Sub 模式說明
   - Topic 設計概念
   - QoS 等級簡介

**投影片重點**：
- IoT 架構圖
- MQTT Pub/Sub 示意圖
- 今日專案架構圖（ESP32 ↔ Broker ↔ Web/Processing）

**互動環節**：
- 提問：「有人用過智慧音箱/智慧燈泡嗎？」
- 討論：IoT 在生活中的應用案例

---

### ⏰ 00:15 - 00:45 | 軟體環境設定（30 分鐘）

**教學目標**：確保所有學員環境設定完成

#### 步驟 1：Arduino IDE 設定（10 分鐘）

**操作步驟**：
```
1. 開啟 Arduino IDE 2.x
2. File > Preferences > Additional Board Manager URLs
3. 加入：https://espressif.github.io/arduino-esp32/package_esp32_index.json
4. Tools > Board > Boards Manager > 搜尋 "ESP32" > 安裝
5. Tools > Board > ESP32 Arduino > ESP32 Dev Module
6. 插入 ESP32，選擇正確的 COM Port
```

**講師示範**：
- 投影螢幕操作
- 強調常見錯誤（驅動程式、Port 選擇）

**學員實作**：
- 跟著步驟操作
- 助教巡視協助

#### 步驟 2：安裝 PubSubClient Library（5 分鐘）

**操作步驟**：
```
1. Tools > Manage Libraries
2. 搜尋 "PubSubClient"
3. 安裝 by Nick O'Leary
```

#### 步驟 3：測試 ESP32（10 分鐘）

**測試程式**：
```cpp
void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
}

void loop() {
  digitalWrite(2, HIGH);
  Serial.println("LED ON");
  delay(1000);
  digitalWrite(2, LOW);
  Serial.println("LED OFF");
  delay(1000);
}
```

**檢查點**：
- [ ] 程式上傳成功
- [ ] Serial Monitor 有輸出
- [ ] 板載 LED 閃爍

#### 步驟 4：Processing 安裝（5 分鐘）

**操作步驟**：
```
1. 下載 Processing 4.x（https://processing.org/download）
2. 解壓縮並執行
3. Tools > Add Tool > Libraries > 搜尋 "MQTT"
4. 安裝 "mqtt" by Joel Gaehwiler
```

**講師提示**：
- Processing 可能需要 Java 環境
- 確認 MQTT Library 安裝成功

---

### ⏰ 00:45 - 01:00 | MQTT Broker 設定（15 分鐘）

**教學目標**：啟動 MQTT Broker 並測試連線

#### 步驟 1：啟動 Node.js Broker（5 分鐘）

**操作步驟**（講師示範）：
```powershell
cd d:\git\IoT-Cross-Media-Tutorial\tutorials\esp32-mqtt-workshop\server
npm install
node broker.js
```

**預期輸出**：
```
Aedes MQTT broker listening on:
- TCP: localhost:1883
- WebSocket: localhost:9001
```

**記錄資訊**（寫在白板）：
```
MQTT Broker IP: 192.168.1.XXX（講師電腦 IP）
TCP Port: 1883
WebSocket Port: 9001
Web Server: http://192.168.1.XXX:3000
```

#### 步驟 2：測試 MQTT 連線（10 分鐘）

**使用 PowerShell 測試**（講師示範）：
```powershell
# 訂閱測試
.\pubsub.ps1 -BrokerHost localhost -Topic "test/hello" -Subscribe

# 發布測試（開另一個終端）
.\pubsub.ps1 -BrokerHost localhost -Topic "test/hello" -Message "Hello MQTT!"
```

**學員觀察**：
- 第一個終端收到訊息 "Hello MQTT!"
- 理解 Pub/Sub 概念

**Q&A 時間**（5 分鐘）：
- 回答學員問題
- 確認所有人理解 MQTT 運作方式

---

## Part 2: ESP32 基礎互動（60 分鐘）

### ⏰ 01:00 - 01:20 | 範例 1: LED 控制（20 分鐘）

**教學目標**：學會 GPIO 控制與 Serial 通訊

#### 講師示範（5 分鐘）

**接線說明**：
```
ESP32 GPIO 25 → LED 正極（長腳）
LED 負極 → 220Ω 電阻 → GND
```

**電路圖展示**（投影片）：
```
ESP32          LED        
GPIO25 ------>|---- 220Ω ---- GND
```

#### 程式碼講解（10 分鐘）

**載入範例**：
```
tutorials/esp32-mqtt-workshop/arduino/01_button_led.ino
```

**重點說明**：
```cpp
// GPIO 定義
const int LED_PIN = 25;
const int BUTTON_PIN = 26;

// 初始化
pinMode(LED_PIN, OUTPUT);
pinMode(BUTTON_PIN, INPUT_PULLUP);

// 讀取按鈕
bool buttonState = digitalRead(BUTTON_PIN);
if (buttonState == LOW) {  // 按下時為 LOW
  digitalWrite(LED_PIN, HIGH);
}
```

#### 學員實作（5 分鐘）

**任務**：
1. 接線（LED + 按鈕）
2. 上傳程式
3. 測試按鈕控制 LED

**檢查點**：
- [ ] LED 可以點亮
- [ ] 按鈕可以控制 LED
- [ ] Serial Monitor 有輸出

---

### ⏰ 01:20 - 01:40 | 範例 2: 光敏電阻（20 分鐘）

**教學目標**：學會 ADC 類比訊號讀取

#### 講師示範（5 分鐘）

**接線說明**：
```
光敏電阻 1 腳 → 3.3V
光敏電阻 2 腳 → ESP32 GPIO 34 (ADC)
光敏電阻 2 腳 → 10KΩ 電阻 → GND
```

**電路圖說明**（投影片）：
- 分壓電路原理
- ADC 解析度：0-4095（12-bit）

#### 程式碼講解（10 分鐘）

**載入範例**：
```
tutorials/esp32-mqtt-workshop/arduino/02_light_sensor.ino
```

**重點說明**：
```cpp
// 讀取類比值
int lightValue = analogRead(LIGHT_PIN);

// 映射到 0-100
int brightness = map(lightValue, 0, 4095, 0, 100);

// 輸出到 Serial
Serial.print("Light: ");
Serial.println(lightValue);
```

#### 學員實作（5 分鐘）

**任務**：
1. 接線（光敏電阻）
2. 上傳程式
3. 用手遮光，觀察數值變化

**實驗引導**：
- 「試著用手遮住光敏電阻，看數值如何變化？」
- 「在亮處和暗處的數值差異多大？」

---

### ⏰ 01:40 - 02:00 | 範例 3: 觸控感測器（20 分鐘）

**教學目標**：學會 ESP32 內建觸控功能

#### 講師示範（5 分鐘）

**接線說明**：
```
ESP32 GPIO 4 (Touch Pin) → 跳線（當作觸控極板）
```

**原理說明**：
- ESP32 內建電容式觸控
- 觸碰時電容值改變
- 不需要外接電路！

#### 程式碼講解（10 分鐘）

**載入範例**：
```
tutorials/esp32-mqtt-workshop/arduino/03_touch_sensor.ino
```

**重點說明**：
```cpp
// 讀取觸控值
int touchValue = touchRead(TOUCH_PIN);

// 判斷觸碰（閾值需要實測）
if (touchValue < TOUCH_THRESHOLD) {
  Serial.println("Touched!");
}
```

#### 學員實作（5 分鐘）

**任務**：
1. 接線（一條跳線即可）
2. 上傳程式
3. 用手指觸碰跳線，觀察數值

**除錯提示**：
- 觸控閾值可能因環境不同需要調整
- 建議先觀察未觸碰時的數值，再設定閾值

---

## 中場休息（15 分鐘）⏸️

**講師工作**：
- 檢查學員進度
- 協助落後學員
- 準備下半場 Demo

**學員活動**：
- 休息、喝水
- 交流討論
- 可以繼續實驗感測器

---

## Part 3: MQTT 整合 + 手機控制（60 分鐘）

### ⏰ 02:15 - 02:35 | MQTT 基礎整合（20 分鐘）

**教學目標**：ESP32 連上 MQTT Broker

#### 步驟 1：WiFi 設定（5 分鐘）

**建立 config.h**（講師引導）：
```cpp
// config.h
#ifndef CONFIG_H
#define CONFIG_H

const char* WIFI_SSID = "工作坊WiFi";  // 改成現場 WiFi
const char* WIFI_PASSWORD = "12345678";

const char* MQTT_SERVER = "192.168.1.XXX";  // 講師電腦 IP
const int MQTT_PORT = 1883;
const char* DEVICE_ID = "esp32-01";  // 每人不同

#endif
```

**講師提示**：
- 每位學員的 DEVICE_ID 要不同（esp32-01, esp32-02...）
- 記錄自己的 Device ID（後續會用到）

#### 步驟 2：MQTT 程式講解（10 分鐘）

**載入範例**：
```
tutorials/esp32-mqtt-workshop/arduino/04_mqtt_basic.ino
```

**重點說明**：
```cpp
// 1. WiFi 連線
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

// 2. MQTT 連線
client.setServer(MQTT_SERVER, MQTT_PORT);
client.setCallback(callback);

// 3. 訂閱 Topic
String cmdTopic = "iot/lab/" + String(DEVICE_ID) + "/cmd/led";
client.subscribe(cmdTopic.c_str());

// 4. 處理訊息
void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  // 解析 JSON 並控制 LED
}

// 5. 發布感測器資料
String sensorTopic = "iot/lab/" + String(DEVICE_ID) + "/sensor/light";
client.publish(sensorTopic.c_str(), jsonData);
```

#### 步驟 3：上傳測試（5 分鐘）

**學員操作**：
1. 修改 config.h（WiFi、MQTT Server、Device ID）
2. 上傳程式到 ESP32
3. 觀察 Serial Monitor

**預期輸出**：
```
Connecting to WiFi...
WiFi connected
IP: 192.168.1.XXX
Connecting to MQTT...
MQTT Connected!
Subscribed to: iot/lab/esp32-01/cmd/led
```

---

### ⏰ 02:35 - 02:55 | 手機網頁控制（20 分鐘）

**教學目標**：用手機控制 ESP32 LED

#### 步驟 1：啟動 Web Server（5 分鐘）

**講師操作**：
```powershell
cd web
.\serve.ps1
```

**顯示 QR Code**（投影片）：
```
http://192.168.1.XXX:3000/index.html
```

#### 步驟 2：學員連線測試（10 分鐘）

**學員操作**：
1. 手機連上相同 WiFi
2. 開啟瀏覽器
3. 輸入網址：`http://192.168.1.XXX:3000/index.html`
4. 輸入自己的 Device ID（esp32-01...）
5. 點擊「LED ON」按鈕

**預期結果**：
- ESP32 板載 LED 亮起！
- Serial Monitor 顯示收到指令

**互動環節**：
- 「大家可以互相控制對方的 LED 嗎？」（改 Device ID）
- 「試試看訂閱 `iot/lab/+/cmd/led`，可以看到所有指令！」

#### 步驟 3：手機搖晃功能（5 分鐘）

**講師示範**：
1. 在網頁上啟用「搖晃偵測」
2. 搖動手機
3. ESP32 LED 閃爍

**程式碼講解**（簡短）：
```javascript
// DeviceMotion API
window.addEventListener('devicemotion', (event) => {
  const acc = event.accelerationIncludingGravity;
  const magnitude = Math.sqrt(acc.x**2 + acc.y**2 + acc.z**2);
  
  if (magnitude > 18) {  // 搖晃閾值
    // 發送 MQTT 指令
    client.publish(topic, '{"cmd":"shake"}');
  }
});
```

**學員測試**：
- 啟用搖晃偵測
- 搖動手機控制 LED

---

### ⏰ 02:55 - 03:15 | 藝術色彩互動（20 分鐘）

**教學目標**：RGB LED 色彩控制

#### 步驟 1：RGB LED 接線（5 分鐘）

**接線說明**：
```
ESP32 GPIO 25 → 紅色 LED → 220Ω → GND
ESP32 GPIO 26 → 綠色 LED → 220Ω → GND
ESP32 GPIO 27 → 藍色 LED → 220Ω → GND
```

**講師提示**：
- 如果沒有 RGB LED，可以用三個單色 LED
- 使用共陰極 RGB LED

#### 步驟 2：上傳藝術控制程式（5 分鐘）

**載入範例**：
```
tutorials/esp32-mqtt-workshop/arduino/custom/art_color_led_mqtt.ino
```

**重點說明**：
```cpp
// 解析 #hex 顏色
void parseColor(String hex) {
  int r = strtol(hex.substring(1, 3).c_str(), NULL, 16);
  int g = strtol(hex.substring(3, 5).c_str(), NULL, 16);
  int b = strtol(hex.substring(5, 7).c_str(), NULL, 16);
  
  setColor(r, g, b);
}

// PWM 輸出
void setColor(int r, int g, int b) {
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}
```

#### 步驟 3：藝術網頁測試（10 分鐘）

**學員操作**：
1. 手機開啟 `http://192.168.1.XXX:3000/art.html`
2. 輸入自己的 Device ID
3. 點擊不同顏色色塊
4. 觀察 RGB LED 顏色變化

**創意實驗**：
- 「試試看快速點擊不同顏色，產生動態效果！」
- 「兩人協作，一人控制顏色，一人觀察視覺效果」

---

## Part 4: Processing 視覺藝術（45 分鐘）

### ⏰ 03:15 - 03:30 | Processing 基礎視覺化（15 分鐘）

**教學目標**：Processing 接收 MQTT 產生視覺效果

#### 步驟 1：開啟範例（5 分鐘）

**講師操作**：
```
1. 開啟 Processing IDE
2. File > Open > IoT_Art_Visualizer.pde
3. 修改 MQTT_HOST 為講師電腦 IP
4. 點擊執行 ▶️
```

**預期結果**：
- Processing 視窗開啟（800x600）
- 顯示「Connecting to MQTT...」
- 連線成功後顯示「Connected」

#### 步驟 2：整合測試（5 分鐘）

**講師示範**：
1. 手機開啟 `art.html`
2. 點擊色塊
3. **同時觀察三個地方**：
   - 📱 手機：顯示選擇的顏色
   - 💡 ESP32：RGB LED 顯示顏色
   - 🎨 Processing：視覺效果（背景漸變、粒子爆炸）

**效果說明**：
- 背景色漸變到選擇的顏色
- 中央圓形發光
- 粒子從中心爆炸擴散
- 脈衝光環動畫

#### 步驟 3：學員體驗（5 分鐘）

**學員操作**：
- 用手機控制
- 觀察 Processing 視覺效果
- 嘗試不同顏色組合

---

### ⏰ 03:30 - 03:50 | 進階視覺效果（20 分鐘）

**教學目標**：體驗多種 Processing 視覺效果

#### 範例 1：粒子系統（5 分鐘）

**講師示範**：
```
開啟 IoT_Particle_System.pde
```

**操作**：
1. 手機開啟 `processing-control.html`
2. 點擊「💥 爆炸」按鈕
3. 觀察粒子效果

**效果展示**：
- 爆炸：放射狀粒子
- 雨滴：從上落下
- 波浪：橫向波動
- 螺旋：螺旋擴散

#### 範例 2：3D 互動（5 分鐘）

**講師示範**：
```
開啟 IoT_3D_Cube.pde
```

**操作**：
1. 調整 X/Y/Z 滑桿
2. 觀察 3D 方塊旋轉
3. 點擊「隨機旋轉」

**互動說明**：
- 滑鼠拖曳可旋轉
- ESP32 光敏感測器可控制旋轉

#### 範例 3：生成藝術（可選，5 分鐘）

**講師示範**：
```
開啟 IoT_Generative_Art.pde
```

**操作**：
1. ESP32 觸控感測器新增繪圖點
2. 光敏感測器改變顏色
3. 按空白鍵切換繪圖模式

**效果展示**：
- 線條藝術
- 圓形藝術
- 三角形藝術
- 貝茲曲線

#### 討論與 Q&A（5 分鐘）

**討論主題**：
- 「你們覺得哪個視覺效果最酷？」
- 「可以想到什麼應用場景？」
- 「如果要改造成自己的專案，會怎麼做？」

---

### ⏰ 03:50 - 04:00 | 創意挑戰 + 總結（10 分鐘）

**創意挑戰（5 分鐘）**

**挑戰任務**（選一）：
1. **藝術挑戰**：組合 3 種以上感測器，創作互動藝術
2. **遊戲挑戰**：設計一個雙人對戰遊戲（用 LED 和按鈕）
3. **實用挑戰**：設計智慧家居場景（光控燈、觸控開關）

**展示時間**：
- 自願分享作品
- 全班投票選出最佳創意

---

**課程總結（5 分鐘）**

**回顧今日學習**：
1. ✅ IoT 基礎概念與 MQTT 協定
2. ✅ ESP32 GPIO、ADC、觸控感測器
3. ✅ MQTT 雙向通訊
4. ✅ 手機網頁控制（含搖晃偵測）
5. ✅ Processing 視覺藝術
6. ✅ RGB LED 色彩控制

**技能樹**：
```
📚 理論知識
  ├─ IoT 架構
  ├─ MQTT Pub/Sub
  └─ 感測器原理

🛠️ 實作能力
  ├─ Arduino 程式設計
  ├─ ESP32 硬體控制
  ├─ Web 前端開發
  └─ Processing 視覺設計

🎨 創意應用
  ├─ 互動藝術
  ├─ 智慧家居
  └─ 數據視覺化
```

**延伸學習資源**：
- ESP32 官方文件：https://docs.espressif.com/
- MQTT.org：https://mqtt.org/
- Processing 教學：https://processing.org/tutorials
- OpenProcessing 範例：https://openprocessing.org/

**作業（可選）**：
1. 完成一個創意專案
2. 拍攝 Demo 影片
3. 上傳到 GitHub

**問卷調查**：
- 填寫課後問卷（QR Code）

**感謝與合影** 📸

---

## 📚 講師備忘錄

### 常見問題處理

#### ❌ ESP32 無法上傳程式
**原因**：
- COM Port 選錯
- 驅動程式未安裝
- USB 線僅供電不傳資料

**解決方式**：
1. 檢查 Device Manager 是否有 COM Port
2. 安裝 CH340/CP2102 驅動程式
3. 換一條 USB 線
4. 上傳時按住 BOOT 按鈕

---

#### ❌ WiFi 連線失敗
**原因**：
- SSID/密碼錯誤
- WiFi 訊號弱
- ESP32 不支援 5GHz WiFi

**解決方式**：
1. 確認 WiFi 是 2.4GHz
2. 檢查 SSID/密碼（注意大小寫）
3. 靠近 AP
4. 重開 ESP32

---

#### ❌ MQTT 連線失敗
**原因**：
- Broker IP 錯誤
- 防火牆阻擋
- Broker 未啟動

**解決方式**：
1. ping 測試 Broker IP
2. 防火牆允許 1883, 9001 port
3. 確認 Broker 正在運行
4. 檢查 Serial Monitor 錯誤訊息

---

#### ❌ 手機無法控制 ESP32
**原因**：
- Device ID 不符
- 手機與 ESP32 不在同一網段
- MQTT 訂閱 Topic 錯誤

**解決方式**：
1. 確認 Device ID 正確
2. 手機連上相同 WiFi
3. 檢查 Topic 格式
4. 用 PowerShell 測試發送

---

#### ❌ Processing 沒有視覺反應
**原因**：
- MQTT Library 未安裝
- MQTT_HOST 設定錯誤
- Topic 不符

**解決方式**：
1. 確認 MQTT Library 已安裝
2. 修改 MQTT_HOST 為正確 IP
3. 檢查 Processing Console 訊息
4. 用 pubsub.ps1 測試發送

---

### 時間控管技巧

**如果進度超前**：
- 增加創意挑戰時間
- 示範更多 Processing 範例
- 深入講解 MQTT QoS
- 介紹進階 Topic 設計

**如果進度落後**：
- 跳過部分 Processing 範例（保留 Art_Visualizer）
- 簡化感測器實作（只做 LED + 按鈕）
- 減少 Q&A 時間
- 助教協助個別輔導

---

### 教學小技巧

1. **Demo First**
   - 先展示最終成果，激發學習動機
   - 「這就是我們今天要完成的！」

2. **小步快跑**
   - 每個階段都要有明確的檢查點
   - 確認所有人完成才繼續

3. **互動提問**
   - 定期詢問「有沒有問題？」
   - 鼓勵學員分享想法

4. **視覺化教學**
   - 多用圖表、動畫
   - 投影實際操作畫面

5. **錯誤示範**
   - 故意犯錯，示範除錯過程
   - 「看，我也會遇到這個錯誤，這樣解決...」

6. **進度追蹤**
   - 用舉手確認完成進度
   - 助教巡視協助落後學員

7. **成就感設計**
   - 每個階段都有「哇！」的時刻
   - LED 亮起、手機控制成功、視覺效果出現

8. **拍照記錄**
   - 記錄學員作品
   - 最後大合照

---

## 🎁 課後資源

**提供給學員**：
- [ ] 完整程式碼 USB 隨身碟
- [ ] 教學投影片 PDF
- [ ] 參考資料連結清單
- [ ] Discord/Line 群組（課後討論）
- [ ] 證書（可選）

**GitHub 資源**：
```
https://github.com/kenneth7198/IoT-Cross-Media-Tutorial
└─ tutorials/esp32-mqtt-workshop/
   ├─ QUICKSTART.md
   ├─ install.md
   ├─ materials.md
   └─ 所有範例程式碼
```

---

## 🎯 教學成功指標

**知識層面**：
- [ ] 80% 學員能解釋 MQTT Pub/Sub
- [ ] 90% 學員能說出 IoT 三層架構

**技能層面**：
- [ ] 100% 學員完成 LED 控制
- [ ] 90% 學員完成 MQTT 整合
- [ ] 80% 學員完成手機控制
- [ ] 70% 學員完成 Processing 視覺化

**態度層面**：
- [ ] 學員積極提問與討論
- [ ] 學員自發延伸實驗
- [ ] 學員表達想繼續學習的意願

---

## 📝 課後檢討

**講師自我檢核**：
- [ ] 教學節奏是否合適？
- [ ] 哪些環節學員反應最好？
- [ ] 哪些地方需要改進？
- [ ] 時間分配是否合理？
- [ ] 教材是否需要更新？

**學員回饋收集**：
- 課後問卷
- 口頭訪談
- 社群媒體評價

**改進計畫**：
- 記錄常見問題並更新教材
- 優化範例程式碼
- 製作教學影片（供課後複習）

---

## 🎊 結語

這是一個從零到一的完整 IoT 工作坊，涵蓋硬體、軟體、網頁、視覺藝術四個面向。

**課程特色**：
✅ 理論與實作並重  
✅ 即時互動與視覺化  
✅ 創意空間大  
✅ 完整教材與範例  

**期待學員**：
🌟 對 IoT 產生興趣  
🌟 具備基礎開發能力  
🌟 能夠自主延伸學習  
🌟 創作出獨特專案  

**祝教學順利！** 🎉
