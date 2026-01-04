# ESP32 LED 控制器 - 手機 WebApp 使用說明

## 📱 概述

這是一個專門為 `ESP32_MQTT_LED_Control.ino` 設計的手機網頁應用程式，可透過 MQTT over WebSocket 控制 ESP32 的 LED。

## 🎯 功能特色

### ✅ 核心功能
- **MQTT WebSocket 連線** - 透過瀏覽器連接 MQTT Broker
- **LED 開關控制** - 點擊按鈕控制 ESP32 上的 LED
- **即時狀態回饋** - 接收並顯示 LED 的即時狀態
- **搖晃偵測** - 搖動手機自動開啟 LED
- **訊息日誌** - 即時顯示所有 MQTT 通訊記錄

### 🎨 UI/UX 特點
- 響應式設計，完美適配手機與平板
- 漂亮的漸層配色與動畫效果
- LED 狀態視覺化指示燈
- 觸控友善的大按鈕設計

---

## 🚀 快速開始

### 前置準備

1. **ESP32 硬體設定**
   - 已上傳 `ESP32_MQTT_LED_Control.ino`
   - LED 接在 GPIO 23（正極 → GPIO 23 → 220Ω → GND）
   - ESP32 已連接 WiFi

2. **MQTT Broker 設定**
   - 已啟動 MQTT Broker（Mosquitto 或 Aedes）
   - 啟用 WebSocket 支援（通常是 port 9001）
   - 確保防火牆允許連線

3. **網路環境**
   - 手機與電腦連接到相同 WiFi
   - 知道電腦的 IP 位址

---

## 📝 使用步驟

### Step 1: 啟動 MQTT Broker

#### 方法 A - 使用 Mosquitto（推薦）

**Windows:**
```powershell
# 前景執行（可看到日誌）
"C:\Program Files\mosquitto\mosquitto.exe" -c .\mosquitto.conf -v
```

**macOS/Linux:**
```bash
mosquitto -c ./mosquitto.conf -v
```

#### 方法 B - 使用 Node.js Aedes
```bash
cd tutorials/esp32-mqtt-workshop/server
npm install
node broker.js
```

成功後應該會看到：
```
MQTT Broker started
TCP: localhost:1883
WebSocket: ws://localhost:9001
```

---

### Step 2: 啟動靜態伺服器

在 `web/` 目錄下執行：

**Windows (PowerShell):**
```powershell
./serve.ps1 -Port 3000
```

**macOS/Linux:**
```bash
# 使用 Python
python3 -m http.server 3000

# 或使用 Node.js (需先安裝 http-server)
npx http-server -p 3000
```

---

### Step 3: 在手機上開啟網頁

1. **查詢電腦 IP 位址**
   
   **Windows:**
   ```powershell
   ipconfig
   # 找到 IPv4 位址，例如: 192.168.100.200
   ```
   
   **macOS/Linux:**
   ```bash
   ifconfig | grep inet
   # 或
   ip addr show
   ```

2. **在手機瀏覽器開啟**
   ```
   http://[你的電腦IP]:3000/esp32-led-control.html
   ```
   
   例如：
   ```
   http://192.168.100.200:3000/esp32-led-control.html
   ```

---

### Step 4: 設定並連線

1. **設定 MQTT Broker URL**
   - 預設: `ws://192.168.100.200:9001`
   - 修改為你的電腦 IP（保持 port 9001）

2. **Client ID（選填）**
   - 留空會自動生成
   - 或自訂名稱，例如: `my-phone`

3. **點擊「連線」按鈕**
   - 看到「✅ 已連線」表示成功
   - 日誌顯示「📡 已訂閱主題: status/led」

---

### Step 5: 控制 LED

#### 方法 1: 按鈕控制
- 點擊「🌟 開啟」→ LED 亮起
- 點擊「🌙 關閉」→ LED 熄滅
- 觀察 LED 狀態指示燈變化

#### 方法 2: 搖晃控制
1. 點擊「啟用搖晃偵測」
2. iOS 用戶會看到權限請求，請允許
3. 搖動手機
4. LED 自動開啟！

---

## 🔧 MQTT 主題與訊息格式

### 發布主題（WebApp → ESP32）
```
主題: sensor/LED
格式: {"GPIO23": "on"}  或  {"GPIO23": "off"}
```

### 訂閱主題（ESP32 → WebApp）
```
主題: status/led
格式: {"GPIO23": "on", "pin": 23, "timestamp": 12345}
```

---

## 🐛 故障排除

### ❌ 無法連線到 MQTT Broker

**檢查清單:**
- [ ] MQTT Broker 是否正在執行？
- [ ] WebSocket 端口 9001 是否開啟？
- [ ] 防火牆是否允許 port 9001？
- [ ] IP 位址是否正確？
- [ ] 手機與電腦在同一個 WiFi？

**測試方法:**
```bash
# 使用 mosquitto_sub 測試
mosquitto_sub -h localhost -t "#" -v

# 使用 MQTTX 測試
# 安裝 MQTTX GUI 工具並嘗試連線
```

---

### ❌ 按鈕無法控制 LED

**檢查清單:**
- [ ] ESP32 是否已連線到 WiFi？
- [ ] ESP32 序列埠監控是否顯示「收到 MQTT 訊息」？
- [ ] MQTT 主題是否正確？（應為 `sensor/LED`）
- [ ] JSON 格式是否正確？

**除錯步驟:**
1. 打開 ESP32 序列埠監控
2. 在網頁點擊按鈕
3. 觀察序列埠輸出
4. 應該看到:
   ```
   --- 收到 MQTT 訊息 ---
   主題: sensor/LED
   內容: {"GPIO23":"on"}
   GPIO23 控制指令: on
   LED 狀態: ON
   ```

---

### ❌ 搖晃偵測無效

**iOS 裝置:**
- iOS 13+ 需要用戶授權
- 必須使用 HTTPS 或 localhost
- 點擊「啟用搖晃偵測」後允許權限

**Android 裝置:**
- 確保瀏覽器支援 DeviceMotion API
- 建議使用 Chrome 瀏覽器
- 檢查瀏覽器設定中的感測器權限

**測試方法:**
```javascript
// 在瀏覽器控制台執行
window.addEventListener('devicemotion', (e) => {
  console.log(e.accelerationIncludingGravity);
});
// 搖動手機，應該看到數值變化
```

---

### ❌ 手機無法開啟網頁

**可能原因:**
1. IP 位址錯誤
2. 靜態伺服器未啟動
3. 防火牆阻擋 port 3000
4. 手機與電腦不在同一網段

**解決方法:**
```powershell
# Windows 防火牆允許規則
New-NetFirewallRule -DisplayName "HTTP Server" -Direction Inbound -LocalPort 3000 -Protocol TCP -Action Allow

# 確認伺服器正在執行
netstat -an | findstr 3000
```

---

## 📊 通訊流程圖

```
┌─────────┐         ┌──────────────┐         ┌─────────┐
│  手機   │         │ MQTT Broker  │         │  ESP32  │
│ WebApp  │         │(WebSocket)   │         │         │
└────┬────┘         └──────┬───────┘         └────┬────┘
     │                     │                      │
     │  1. WebSocket連線   │                      │
     ├────────────────────>│                      │
     │  <CONNECT ACK>      │                      │
     │<────────────────────┤                      │
     │                     │                      │
     │  2. 訂閱status/led  │                      │
     ├────────────────────>│                      │
     │                     │                      │
     │  3. 發布{"GPIO23":"on"}                   │
     ├────────────────────>│                      │
     │                     │  轉發到sensor/LED    │
     │                     ├─────────────────────>│
     │                     │                      │
     │                     │  4. LED開啟          │
     │                     │                      │
     │                     │  5. 發布狀態         │
     │                     │<─────────────────────┤
     │  6. 收到狀態更新    │                      │
     │<────────────────────┤                      │
     │                     │                      │
```

---

## 🎨 自訂與擴充

### 修改 MQTT 主題

如果你的 ESP32 程式使用不同的主題，請修改：

```javascript
// 在 esp32-led-control.html 中找到並修改
const TOPIC_LED_CONTROL = "sensor/LED";    // 改成你的控制主題
const TOPIC_LED_STATUS = "status/led";      // 改成你的狀態主題
```

### 修改 JSON 格式

如果你的 ESP32 使用不同的 JSON 欄位：

```javascript
// 發送指令（第 368 行）
const payload = JSON.stringify({
  GPIO23: state ? "on" : "off"  // 改成你的欄位名稱
});

// 解析狀態（第 285 行）
if (data.GPIO23) {  // 改成你的欄位名稱
  const ledState = (data.GPIO23 === 'on');
  updateLedStatus(ledState);
}
```

### 新增更多控制

可以擴充為控制多個 LED 或其他裝置：

```javascript
// 範例：控制 RGB LED
function sendRgbCommand(r, g, b) {
  const payload = JSON.stringify({
    red: r,
    green: g,
    blue: b
  });
  mqttClient.publish("sensor/RGB", payload, { qos: 1 });
}
```

---

## 📱 進階功能

### 1. PWM 亮度控制

如果 ESP32 支援 PWM，可新增亮度滑桿：

```html
<input type="range" min="0" max="255" id="brightness">
<button onclick="setBrightness()">設定亮度</button>

<script>
function setBrightness() {
  const value = document.getElementById('brightness').value;
  const payload = JSON.stringify({
    GPIO23: "on",
    brightness: parseInt(value)
  });
  mqttClient.publish(TOPIC_LED_CONTROL, payload, { qos: 1 });
}
</script>
```

### 2. 排程控制

新增定時開關功能：

```javascript
function scheduleLED(delay, state) {
  setTimeout(() => {
    sendLedCommand(state);
    addLog(`⏰ 排程觸發: LED ${state ? 'ON' : 'OFF'}`, 'info');
  }, delay);
}

// 使用範例：5秒後開燈
scheduleLED(5000, true);
```

### 3. 語音控制

結合 Web Speech API：

```javascript
const recognition = new webkitSpeechRecognition();
recognition.lang = 'zh-TW';

recognition.onresult = (event) => {
  const command = event.results[0][0].transcript;
  if (command.includes('開燈')) {
    sendLedCommand(true);
  } else if (command.includes('關燈')) {
    sendLedCommand(false);
  }
};

recognition.start();
```

---

## 🔐 安全建議

### 生產環境注意事項

1. **使用 HTTPS/WSS**
   ```javascript
   const url = 'wss://your-server.com:9001';  // 使用加密連線
   ```

2. **新增帳號密碼驗證**
   ```javascript
   const options = {
     username: 'your_username',
     password: 'your_password'
   };
   mqttClient = mqtt.connect(url, options);
   ```

3. **限制主題訪問權限**
   在 Mosquitto 設定檔中：
   ```
   # mosquitto.conf
   allow_anonymous false
   password_file /etc/mosquitto/passwd
   acl_file /etc/mosquitto/acl
   ```

---

## 📚 相關資源

- **ESP32 程式碼**: `arduino/ESP32_MQTT_LED_Control.ino`
- **MQTT Broker 設定**: `server/README.md`
- **學生指南**: `STUDENT_GUIDE.md`
- **原始 WebApp**: `web/index.html`

---

## ❓ 常見問題

### Q: 可以同時多支手機連線嗎？
A: 可以！每個手機會有不同的 Client ID，可以同時控制同一個 ESP32。

### Q: 網頁關閉後 ESP32 會斷線嗎？
A: 不會。ESP32 直接連線到 MQTT Broker，網頁只是控制介面。

### Q: 可以在電腦瀏覽器使用嗎？
A: 可以！但搖晃偵測功能只適用於手機。

### Q: 支援哪些瀏覽器？
A: 支援所有現代瀏覽器（Chrome, Safari, Firefox, Edge）。

### Q: 需要安裝 App 嗎？
A: 不需要！這是純網頁應用，直接用瀏覽器開啟即可。

---

## 🎓 延伸學習

1. **MQTT QoS 等級**
   - QoS 0: 最多傳送一次
   - QoS 1: 至少傳送一次（本專案使用）
   - QoS 2: 只傳送一次

2. **WebSocket vs HTTP**
   - WebSocket 支援雙向即時通訊
   - 適合 IoT 即時控制場景

3. **JSON 序列化**
   - 輕量級資料交換格式
   - 易於人類閱讀和機器解析

---

## 📄 授權

MIT License

---

**製作日期**: 2025-10-20  
**版本**: 1.0.0  
**作者**: IoT Cross-Media Team
