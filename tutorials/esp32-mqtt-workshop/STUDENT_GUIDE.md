# 學員操作手冊 - ESP32 + MQTT + Processing 工作坊

**🎯 你將學會：用手機控制 LED，用感測器產生視覺藝術！**

---

## 📚 目錄

1. [軟體安裝](#1-軟體安裝)
2. [硬體測試](#2-硬體測試)
3. [基礎實驗](#3-基礎實驗)
4. [MQTT 整合](#4-mqtt-整合)
5. [手機控制](#5-手機控制)
6. [視覺藝術](#6-視覺藝術)
7. [故障排除](#7-故障排除)

---

## 1. 軟體安裝

### Step 1: Arduino IDE

1. **開啟 Arduino IDE 2.x**
2. **設定 ESP32 板子**
   - `File` → `Preferences`
   - 在 `Additional Board Manager URLs` 貼上：
     ```
     https://espressif.github.io/arduino-esp32/package_esp32_index.json
     ```
   - 點擊 `OK`

3. **安裝 ESP32 Board**
   - `Tools` → `Board` → `Boards Manager`
   - 搜尋 `ESP32`
   - 安裝 `esp32 by Espressif Systems`

4. **選擇板子**
   - `Tools` → `Board` → `ESP32 Arduino` → `ESP32 Dev Module`

5. **選擇 COM Port**
   - 插入 ESP32
   - `Tools` → `Port` → 選擇 `COM3`（或其他編號）

### Step 2: 安裝 Library

1. `Tools` → `Manage Libraries`
2. 搜尋 `PubSubClient`
3. 安裝 `PubSubClient by Nick O'Leary`

### Step 3: 測試 ESP32

**複製這段程式**：
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

**上傳步驟**：
1. 點擊 `✓` (Verify) 編譯
2. 點擊 `→` (Upload) 上傳
3. 開啟 Serial Monitor（`Tools` → `Serial Monitor`）
4. 設定 Baud Rate 為 `115200`

**成功標誌**：
- ✅ 板載 LED 閃爍
- ✅ Serial Monitor 顯示 "LED ON" / "LED OFF"

---

## 2. 硬體測試

### 🔴 LED + 按鈕

**材料**：
- ESP32 x 1
- LED x 1
- 220Ω 電阻 x 1
- 按鈕 x 1
- 跳線若干

**接線圖**：
```
ESP32 GPIO 25 ──→ LED 正極（長腳）
LED 負極 ──→ 220Ω 電阻 ──→ GND

ESP32 GPIO 26 ──→ 按鈕一腳
按鈕另一腳 ──→ GND
```

**注意事項**：
- LED 有正負極（長腳為正）
- 電阻必須接上（保護 LED）
- 按鈕使用 `INPUT_PULLUP`（程式內設定）

**上傳程式**：
```
檔案位置：arduino/01_button_led.ino
```

**測試**：
- 按下按鈕 → LED 亮
- 放開按鈕 → LED 滅

---

### 💡 光敏感測器

**材料**：
- 光敏電阻 x 1
- 10KΩ 電阻 x 1
- 跳線若干

**接線圖**：
```
光敏電阻 1 腳 ──→ 3.3V
光敏電阻 2 腳 ──→ ESP32 GPIO 34 (ADC)
光敏電阻 2 腳 ──→ 10KΩ 電阻 ──→ GND
```

**原理**：
- 亮處：電阻小，電壓高，讀值大
- 暗處：電阻大，電壓低，讀值小

**上傳程式**：
```
檔案位置：arduino/02_light_sensor.ino
```

**測試**：
- 用手遮住光敏電阻
- 觀察 Serial Monitor 數值變化
- 亮處約 3000-4000
- 暗處約 100-500

---

### ✋ 觸控感測器

**材料**：
- 跳線 x 1（當作觸控極板）

**接線圖**：
```
ESP32 GPIO 4 ──→ 跳線（裸露金屬部分）
```

**原理**：
- ESP32 內建電容式觸控
- 手指觸碰時電容值改變
- **不需要外接電路！**

**上傳程式**：
```
檔案位置：arduino/03_touch_sensor.ino
```

**測試**：
1. 觀察未觸碰時的數值（例如 50）
2. 用手指觸碰跳線
3. 數值降低（例如降到 10）
4. 如果沒反應，調整程式中的 `TOUCH_THRESHOLD`

---

## 3. 基礎實驗

### 實驗 1：按鈕計數器

**目標**：按一次按鈕，LED 閃爍次數 +1

**提示**：
- 使用變數 `int count = 0;`
- 按鈕按下時 `count++`
- 用 `for` 迴圈閃爍 LED

---

### 實驗 2：光控 LED

**目標**：暗處自動開燈

**提示**：
- 讀取光敏電阻值
- 如果 `lightValue < 500` → LED ON
- 否則 → LED OFF

---

### 實驗 3：觸控調光

**目標**：觸控切換 LED 亮度（暗→中→亮→暗...）

**提示**：
- 使用 `analogWrite()` 調整亮度
- 亮度值：0, 128, 255
- 觸控時切換

---

## 4. MQTT 整合

### Step 1: 建立 config.h

在 Arduino 專案資料夾建立 `config.h`：

```cpp
#ifndef CONFIG_H
#define CONFIG_H

const char* WIFI_SSID = "工作坊WiFi";        // ← 改成現場 WiFi
const char* WIFI_PASSWORD = "12345678";      // ← 改成 WiFi 密碼

const char* MQTT_SERVER = "192.168.1.XXX";   // ← 改成講師電腦 IP
const int MQTT_PORT = 1883;
const char* DEVICE_ID = "esp32-01";          // ← 改成你的 ID

#endif
```

**重要**：
- 每個人的 `DEVICE_ID` 必須不同！
- 記下你的 Device ID（後續會用到）

---

### Step 2: 上傳 MQTT 程式

**檔案位置**：
```
arduino/04_mqtt_basic.ino
```

**程式功能**：
- 連接 WiFi
- 連接 MQTT Broker
- 訂閱指令：`iot/lab/{你的ID}/cmd/led`
- 發布感測器資料：`iot/lab/{你的ID}/sensor/light`

**上傳並觀察 Serial Monitor**：
```
Connecting to WiFi...
WiFi connected
IP: 192.168.1.105
Connecting to MQTT...
MQTT Connected!
Subscribed to: iot/lab/esp32-01/cmd/led
```

**如果連線失敗**：
- 檢查 WiFi SSID/密碼
- 確認 MQTT_SERVER IP 正確
- 問講師或助教

---

## 5. 手機控制

### Step 1: 開啟控制網頁

**手機操作**：
1. 連上相同 WiFi
2. 開啟瀏覽器
3. 輸入網址：
   ```
   http://192.168.1.XXX:3000/index.html
   ```
   （XXX 是講師電腦 IP，看白板）

### Step 2: 設定 Device ID

1. 在網頁輸入你的 Device ID（例如 `esp32-01`）
2. 點擊「連線」按鈕
3. 看到 "✅ 已連線" 訊息

### Step 3: 控制 LED

**按鈕功能**：
- **LED ON** → 點亮 LED
- **LED OFF** → 關閉 LED

**測試**：
1. 點擊 "LED ON"
2. ESP32 板載 LED 亮起！
3. Serial Monitor 顯示：`收到指令: on`

---

### 🎉 手機搖晃功能

**啟用步驟**：
1. 網頁上點擊「啟用搖晃偵測」
2. 搖動手機
3. LED 閃爍！

**原理**：
- 手機的加速度感測器
- 偵測晃動幅度
- 透過 MQTT 發送指令給 ESP32

---

## 6. 視覺藝術

### 🌈 RGB LED 色彩控制

#### Step 1: 接線

**材料**：
- 紅色 LED x 1
- 綠色 LED x 1
- 藍色 LED x 1
- 220Ω 電阻 x 3

**接線圖**：
```
ESP32 GPIO 25 → 紅色 LED → 220Ω → GND
ESP32 GPIO 26 → 綠色 LED → 220Ω → GND
ESP32 GPIO 27 → 藍色 LED → 220Ω → GND
```

或使用共陰極 RGB LED：
```
ESP32 GPIO 25 → RGB LED (R)
ESP32 GPIO 26 → RGB LED (G)
ESP32 GPIO 27 → RGB LED (B)
RGB LED 共陰極 → GND
每個腳位串接 220Ω 電阻
```

#### Step 2: 上傳程式

**檔案位置**：
```
arduino/custom/art_color_led_mqtt.ino
```

**記得建立 config.h**（同前面步驟）

#### Step 3: 手機色彩控制

**手機操作**：
1. 開啟 `http://192.168.1.XXX:3000/art.html`
2. 輸入你的 Device ID
3. 點擊不同顏色色塊
4. 觀察 RGB LED 顏色變化！

**可用顏色**：
- 🔴 紅色
- 🟢 綠色
- 🔵 藍色
- 🟠 橙色
- 🔷 青色
- 🟣 紫色

---

### 🎨 Processing 視覺效果

#### Step 1: 安裝 Processing

1. 下載：https://processing.org/download
2. 安裝並開啟
3. 安裝 MQTT Library：
   - `Tools` → `Add Tool` → `Libraries`
   - 搜尋 `MQTT`
   - 安裝 `mqtt by Joel Gaehwiler`

#### Step 2: 開啟範例

**檔案位置**：
```
processing/IoT_Art_Visualizer.pde
```

**修改設定**：
```java
String MQTT_HOST = "tcp://192.168.1.XXX:1883";  // ← 改成講師電腦 IP
```

#### Step 3: 執行

1. 點擊 `▶️` 執行按鈕
2. 視窗開啟（800x600）
3. 等待連線成功

#### Step 4: 整合測試

**操作流程**：
1. 📱 手機開啟 `art.html`
2. 💡 ESP32 RGB LED 準備好
3. 🎨 Processing 視窗開啟
4. 點擊色塊 →
   - 手機顯示選擇的顏色 ✅
   - ESP32 RGB LED 顯示顏色 ✅
   - Processing 產生視覺效果 ✅

**視覺效果**：
- 背景漸變成選擇的顏色
- 中央圓形發光
- 粒子從中心爆炸
- 脈衝光環動畫

---

### 🚀 進階視覺效果

#### 粒子系統

**開啟**：`processing/IoT_Particle_System.pde`

**效果**：
- 💥 爆炸
- 🌧️ 雨滴
- 🌊 波浪
- 🌀 螺旋

**控制**：
- 手機開啟 `processing-control.html`
- 點擊不同粒子按鈕

---

#### 3D 方塊

**開啟**：`processing/IoT_3D_Cube.pde`

**效果**：
- 3D 方塊旋轉
- 感測器控制
- 顏色變化

**互動**：
- 滑鼠拖曳旋轉
- ESP32 光敏感測器控制 Y 軸
- 觸控感測器觸發 X 軸旋轉

---

#### 生成藝術

**開啟**：`processing/IoT_Generative_Art.pde`

**效果**：
- 演算藝術生成
- 4 種繪圖模式
- 可儲存 PNG

**操作**：
- 空白鍵：切換模式
- `s` 鍵：儲存畫面
- `c` 鍵：清空畫布
- 滑鼠點擊：新增繪圖點

---

## 7. 故障排除

### ❌ ESP32 無法上傳程式

**解決方式**：
1. 檢查 COM Port 選擇
2. 上傳時按住 **BOOT** 按鈕
3. 換一條 USB 線
4. 降低 Upload Speed：
   - `Tools` → `Upload Speed` → `115200`

---

### ❌ WiFi 連線失敗

**檢查項目**：
- [ ] SSID/密碼正確（注意大小寫）
- [ ] WiFi 是 2.4GHz（ESP32 不支援 5GHz）
- [ ] 靠近 WiFi AP
- [ ] 重新啟動 ESP32

**Serial Monitor 錯誤訊息**：
```
Connecting to WiFi...........
Failed to connect!
```

→ 重新檢查 `config.h` 設定

---

### ❌ MQTT 連線失敗

**檢查項目**：
- [ ] MQTT_SERVER IP 正確
- [ ] Broker 正在運行（問講師）
- [ ] 防火牆沒有阻擋 1883 port

**Serial Monitor 錯誤訊息**：
```
Connecting to MQTT...........
MQTT connection failed!
```

→ 確認 IP 位址、重啟 ESP32

---

### ❌ 手機無法控制 ESP32

**檢查項目**：
- [ ] Device ID 輸入正確
- [ ] 手機與 ESP32 連上相同 WiFi
- [ ] ESP32 Serial Monitor 顯示 "MQTT Connected"
- [ ] 網頁顯示 "✅ 已連線"

**測試方法**：
1. 觀察 Serial Monitor
2. 點擊手機按鈕
3. 看是否顯示 "收到指令: xxx"

---

### ❌ Processing 無視覺反應

**檢查項目**：
- [ ] MQTT Library 已安裝
- [ ] MQTT_HOST IP 正確
- [ ] Processing Console 無錯誤訊息
- [ ] Broker 正在運行

**除錯方式**：
1. 查看 Processing Console（視窗下方）
2. 看是否有 "MQTT: ..." 訊息
3. 如果沒有 → 檢查 MQTT_HOST 設定

---

### ❌ RGB LED 顏色不正確

**可能原因**：
- LED 接錯腳位
- 共陽極/共陰極搞混
- PWM 範圍需要調整

**解決方式**：
1. 檢查接線：R=GPIO25, G=GPIO26, B=GPIO27
2. 如果是共陽極 RGB LED：
   - 反轉 PWM 值：`255 - r`
3. 測試單色：
   ```cpp
   analogWrite(RED_PIN, 255);  // 紅色
   analogWrite(GREEN_PIN, 0);
   analogWrite(BLUE_PIN, 0);
   ```

---

### ❌ 感測器讀值不正常

**光敏電阻**：
- 亮暗處數值沒變化 → 檢查接線
- 數值都是 0 → 可能接錯 ADC pin
- 數值都是 4095 → 檢查分壓電路

**觸控感測器**：
- 沒反應 → 調整 `TOUCH_THRESHOLD`
- 先觀察未觸碰時的值，再設定閾值
- 確認使用 Touch-capable GPIO（4, 12, 13, 14, 15, 27, 32, 33）

---

## 🎓 延伸學習

### 創意專案提案

1. **智慧夜燈**
   - 光敏感測器偵測亮度
   - 暗處自動開燈
   - 手機可強制開/關

2. **音樂視覺化**
   - ESP32 接麥克風
   - Processing 產生音樂波形
   - RGB LED 隨節奏閃爍

3. **雙人遊戲**
   - 兩個 ESP32
   - 按鈕搶答
   - Processing 顯示分數

4. **植物監測**
   - 土壤濕度感測器
   - 手機查看數據
   - 自動澆水提醒

5. **互動藝術裝置**
   - 多個感測器
   - Processing 生成藝術
   - 展覽作品

---

### 學習資源

**官方文件**：
- ESP32: https://docs.espressif.com/
- Arduino: https://www.arduino.cc/reference/
- Processing: https://processing.org/reference/
- MQTT: https://mqtt.org/

**範例與教學**：
- Arduino Examples: `File` → `Examples`
- Processing Examples: `File` → `Examples`
- OpenProcessing: https://openprocessing.org/

**社群**：
- Arduino Forum: https://forum.arduino.cc/
- ESP32 Forum: https://www.esp32.com/
- Processing Forum: https://discourse.processing.org/

---

## 📝 筆記區

**我的 WiFi 設定**：
```
SSID: _______________
密碼: _______________
```

**我的 Device ID**：
```
esp32-____
```

**講師電腦 IP**：
```
192.168.1.____
```

**網頁網址**：
```
http://192.168.1.____:3000/
```

**我的想法與筆記**：
```
__________________________
__________________________
__________________________
```

---

**🎉 恭喜完成工作坊！繼續探索 IoT 的無限可能！**

_有問題隨時問講師或助教！_
