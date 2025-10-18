# ESP32 × MQTT 工作坊 - 快速啟動指南

此指南讓你 15 分鐘內完成環境設定與第一個 Demo。

## 📦 準備清單

- [ ] Windows 10/11 電腦
- [ ] ESP32 開發板 × 1
- [ ] USB 傳輸線
- [ ] LED、按鈕、麵包板（可選）
- [ ] 手機（與電腦同一個 Wi-Fi）

---

## ⚡ 5 步驟快速啟動

### 1️⃣ 安裝軟體（首次 10 分鐘）

```powershell
# Arduino IDE 2.x
# 下載：https://www.arduino.cc/en/software/

# CP210x 驅動（若需要）
# 下載：https://www.silabs.com/software-and-tools/usb-to-uart-bridge-vcp-drivers

# 在 Arduino IDE 中：
# 1. 檔案 > 偏好設定 > 額外的開發板管理員網址
#    貼上：https://espressif.github.io/arduino-esp32/package_esp32_index.json
# 2. 工具 > 開發板 > 開發板管理員 > 搜尋 ESP32 > 安裝
# 3. 工具 > 管理程式庫 > 搜尋 PubSubClient > 安裝
```

### 2️⃣ 啟動 MQTT Broker（1 分鐘）

選項 A：Node 版（推薦新手）
```powershell
cd tutorials\esp32-mqtt-workshop\server
npm install
npm start
# 看到 "TCP broker listening on 1883" 與 "WS broker listening on 9001" 即成功
```

選項 B：Mosquitto（推薦正式環境）
```powershell
# 安裝 Mosquitto 後
"C:\Program Files\mosquitto\mosquitto.exe" -c tutorials\esp32-mqtt-workshop\server\mosquitto.conf -v
```

### 3️⃣ 設定 ESP32 程式（2 分鐘）

1. 複製設定檔：
   ```powershell
   cd tutorials\esp32-mqtt-workshop\arduino
   copy config.example.h config.h
   ```

2. 編輯 `config.h`：
   ```cpp
   #define WIFI_SSID      "你的Wi-Fi名稱"
   #define WIFI_PASS      "你的Wi-Fi密碼"
   #define MQTT_HOST      "192.168.1.100"  // 改成你電腦的IP
   #define MQTT_PORT      1883
   #define DEVICE_ID      "esp32-01"
   ```

3. 燒錄程式（選一個）：
   - 基礎 LED 控制：`04_mqtt_basic.ino`
   - 藝術互動：`custom/art_color_led_mqtt.ino`

### 4️⃣ 啟動手機網頁（1 分鐘）

```powershell
cd tutorials\esp32-mqtt-workshop\web
./serve.ps1 -Port 3000
```

手機瀏覽器開啟：`http://<你的電腦IP>:3000`

選擇頁面：
- `index.html`：基本控制台（LED ON/OFF）
- `art.html`：藝術互動（色塊撈取）

設定 WebSocket URL：`ws://<你的電腦IP>:9001`

### 5️⃣ 測試互動（1 分鐘）

- 點選網頁上的 LED 按鈕或色塊
- 觀察 ESP32 的 LED 變化
- 序列監控視窗查看 MQTT 訊息

---

## 🎨 進階：Processing 視覺藝術（可選）

1. 安裝 Processing 4.x
2. 在 Processing 中安裝 MQTT 函式庫
3. 開啟 `processing/IoT_Art_Visualizer.pde`
4. 修改 `MQTT_HOST` 為你的 Broker IP
5. 執行後，用手機點選色塊，Processing 畫面即時變色

---

## 🔧 常見問題排除

**Q: ESP32 連不上 Wi-Fi**
- 確認 SSID/密碼正確
- 確認 ESP32 支援你的 Wi-Fi 頻段（2.4GHz）

**Q: 手機連不上 MQTT**
- 確認手機與電腦在同一個 Wi-Fi
- 確認防火牆放行 9001 埠
- WebSocket URL 要用電腦 IP，不是 localhost

**Q: ESP32 連不上 MQTT Broker**
- 確認 MQTT_HOST 是電腦 IP，不是 localhost
- 確認防火牆放行 1883 埠
- 序列監控查看錯誤訊息

**Q: Processing 連不上 MQTT**
- 確認已安裝 MQTT 函式庫
- 確認 Broker 正在運行
- 檢查 MQTT_HOST 設定

---

## 📚 完整教材

- 安裝步驟：`install.md`
- 課程大綱：`materials.md`
- Topic 設計：`topics.md`
- 接線指南：`assets/wiring.md`
- 投影片：`slides/iot-mqtt-workshop.marp.md`

---

## 🚀 現在就開始

1. 複製設定檔：`copy config.example.h config.h`
2. 啟動 Broker：`cd server && npm start`
3. 燒錄程式到 ESP32
4. 啟動網頁：`cd web && ./serve.ps1`
5. 手機連線開始互動！

🎉 祝你玩得開心！
