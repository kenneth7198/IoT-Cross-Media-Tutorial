# ESP32 MQTT LED 控制指南

## 📋 概述

這個程式讓 ESP32 成為 MQTT **訂閱者 (Subscriber)**,接收 JSON 格式的控制指令來控制 LED 燈。

## 🔌 硬體連接

### LED 連接方式
```
ESP32 GPIO 23 → 220Ω 電阻 → LED 正極(長腳)
LED 負極(短腳) → GND
```

### 可選：本地測試按鈕
```
ESP32 GPIO 22 → 按鈕一端
按鈕另一端 → GND
```

## 📦 所需函式庫

### 1. ArduinoJson (必須安裝)

**Arduino IDE 安裝步驟：**
1. 開啟 Arduino IDE
2. 工具 → 管理程式庫
3. 搜尋 "ArduinoJson"
4. 選擇 by Benoit Blanchon
5. 點擊安裝（建議安裝 6.x 版本）

**或使用指令安裝：**
```bash
arduino-cli lib install "ArduinoJson@6.21.3"
```

### 2. 其他函式庫（通常已內建）
- WiFi.h (ESP32 內建)
- PubSubClient.h (需要安裝，2.8.0 版本)

## 🚀 使用方式

### 1. 上傳程式到 ESP32
1. 開啟 `ESP32_MQTT_LED_Control.ino`
2. 檢查 WiFi 和 MQTT 設定是否正確
3. 上傳到 ESP32
4. 開啟序列埠監控視窗 (115200 baud)

### 2. 測試 LED 控制

#### 方法 A：使用 mosquitto_pub 指令

**開啟 LED：**
```bash
mosquitto_pub -h 192.168.100.200 -t 'control/led' -m '{"led":1}'
```

**關閉 LED：**
```bash
mosquitto_pub -h 192.168.100.200 -t 'control/led' -m '{"led":0}'
```

**使用 macOS Mosquitto（完整路徑）：**
```bash
/opt/homebrew/sbin/mosquitto_pub -h 192.168.100.200 -t 'control/led' -m '{"led":1}'
```

#### 方法 B：使用 MQTT.fx 或 MQTT Explorer

1. 連接到 Broker: `192.168.100.200:1883`
2. 發布到主題: `control/led`
3. 訊息內容: `{"led":1}` 或 `{"led":0}`

#### 方法 C：使用 Node.js 腳本

建立檔案 `led_control.js`:
```javascript
const mqtt = require('mqtt');
const client = mqtt.connect('mqtt://192.168.100.200:1883');

client.on('connect', () => {
  console.log('已連接到 MQTT Broker');
  
  // 開啟 LED
  client.publish('control/led', JSON.stringify({led: 1}));
  console.log('已發送: LED ON');
  
  // 3秒後關閉 LED
  setTimeout(() => {
    client.publish('control/led', JSON.stringify({led: 0}));
    console.log('已發送: LED OFF');
    client.end();
  }, 3000);
});
```

執行：
```bash
node led_control.js
```

### 3. 監控 LED 狀態

ESP32 會回報 LED 狀態到 `status/led` 主題，可以訂閱此主題監控：

```bash
mosquitto_sub -h 192.168.100.200 -t 'status/led' -v
```

或使用完整路徑：
```bash
/opt/homebrew/sbin/mosquitto_sub -h 192.168.100.200 -t 'status/led' -v
```

## 📊 MQTT 主題架構

### 訂閱主題 (ESP32 接收)
- `control/led` - 接收 LED 控制指令

### 發布主題 (ESP32 發送)
- `status/led` - 回報 LED 當前狀態

## 📝 JSON 格式說明

### 控制指令格式
```json
{
  "led": 1    // 1 = 開啟, 0 = 關閉
}
```

### 狀態回報格式
```json
{
  "led": 1,
  "pin": 23,
  "timestamp": 12345
}
```

## 🔧 進階功能擴充

### 1. PWM 亮度控制

如果想控制 LED 亮度，可以修改 JSON 格式：
```json
{
  "led": 1,
  "brightness": 128    // 0-255
}
```

並在程式中加入 PWM 控制：
```cpp
const int PWM_CHANNEL = 0;
const int PWM_FREQ = 5000;
const int PWM_RESOLUTION = 8;

void setup() {
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(LED_PIN, PWM_CHANNEL);
}

void controlLED(int state, int brightness = 255) {
  if (state) {
    ledcWrite(PWM_CHANNEL, brightness);
  } else {
    ledcWrite(PWM_CHANNEL, 0);
  }
}
```

### 2. 多個 LED 控制

JSON 格式：
```json
{
  "leds": {
    "red": 1,
    "green": 0,
    "blue": 1
  }
}
```

### 3. 定時控制

JSON 格式：
```json
{
  "led": 1,
  "duration": 5000    // 5秒後自動關閉
}
```

## 🐛 除錯檢查清單

### LED 沒有反應
- [ ] 檢查 LED 接線方向（長腳接電阻，短腳接 GND）
- [ ] 確認電阻值（建議 220Ω-1kΩ）
- [ ] 檢查 GPIO 23 是否正確連接
- [ ] 用序列埠監控檢查是否收到 MQTT 訊息

### 無法連接 MQTT
- [ ] 確認 Mosquitto Broker 是否正在運行
- [ ] 檢查 IP 位址是否正確 (192.168.100.200)
- [ ] 測試用 mosquitto_sub 是否能連接

### JSON 解析失敗
- [ ] 確認 JSON 格式正確（使用雙引號，不是單引號）
- [ ] 檢查是否安裝 ArduinoJson 函式庫
- [ ] 確認 JSON 大小不超過 200 bytes

### 序列埠監控沒有輸出
- [ ] 檢查 Baud Rate 是否設定為 115200
- [ ] 重新上傳程式並按下 ESP32 的 RESET 按鈕

## 🧪 完整測試流程

### 1. 啟動 Mosquitto Broker
```bash
/opt/homebrew/sbin/mosquitto -c mosquitto-mac.conf -v
```

### 2. 開啟狀態監控終端
```bash
/opt/homebrew/sbin/mosquitto_sub -h 192.168.100.200 -t 'status/led' -v
```

### 3. 開啟控制終端
```bash
# 開啟 LED
/opt/homebrew/sbin/mosquitto_pub -h 192.168.100.200 -t 'control/led' -m '{"led":1}'

# 等待 2 秒

# 關閉 LED
/opt/homebrew/sbin/mosquitto_pub -h 192.168.100.200 -t 'control/led' -m '{"led":0}'
```

### 4. 檢查結果
- LED 應該在第一個指令後亮起
- LED 應該在第二個指令後熄滅
- 狀態監控終端應該顯示對應的狀態更新
- ESP32 序列埠監控應該顯示接收到的 JSON 訊息

## 📚 相關資源

- [ArduinoJson 文檔](https://arduinojson.org/)
- [PubSubClient 文檔](https://pubsubclient.knolleary.net/)
- [MQTT 協議說明](https://mqtt.org/)
- [ESP32 GPIO 參考](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html)

## 💡 應用場景

1. **遠端燈光控制** - 透過手機 App 或網頁控制家中燈光
2. **狀態指示燈** - 根據系統狀態自動控制警示燈
3. **通知提醒** - 收到訊息或事件時閃爍 LED
4. **藝術裝置** - 互動藝術作品的燈光控制
5. **自動化系統** - 與感測器配合的自動照明系統
