# 4 小時工作坊快速參考卡

**👨‍🏫 講師專用 - 隨時查閱**

---

## ⏱️ 時間軸（一張表看懂）

| 時間 | 時長 | 階段 | 關鍵動作 | 檢查點 |
|------|------|------|----------|--------|
| 00:00 | 15min | 開場 | 投影片、Demo 展示 | 學員到齊 |
| 00:15 | 30min | 環境設定 | Arduino IDE + ESP32 | LED 閃爍 ✅ |
| 00:45 | 15min | MQTT Broker | 啟動 broker.js | PowerShell 測試通過 |
| 01:00 | 20min | LED 控制 | 接線 + 上傳程式 | 按鈕控制 LED ✅ |
| 01:20 | 20min | 光敏感測器 | ADC 讀取 | 手遮光數值變化 ✅ |
| 01:40 | 20min | 觸控感測器 | 觸控測試 | 手指觸碰反應 ✅ |
| 02:00 | 15min | 休息 ☕ | - | - |
| 02:15 | 20min | MQTT 整合 | WiFi + MQTT 連線 | Serial 顯示 "Connected" ✅ |
| 02:35 | 20min | 手機控制 | 開啟 web 頁面 | 手機控制 LED ✅ |
| 02:55 | 20min | RGB 色彩 | art.html + RGB LED | 顏色同步顯示 ✅ |
| 03:15 | 15min | Processing 基礎 | Art_Visualizer | 視覺效果出現 ✅ |
| 03:30 | 20min | 進階視覺 | 粒子、3D、生成藝術 | 多種效果展示 |
| 03:50 | 10min | 挑戰 + 總結 | 創意分享、回顧 | 合影 📸 |

---

## 🎯 四大階段目標

### Part 1: 環境設定（60min）
**目標**：所有人軟硬體就緒
- Arduino IDE + ESP32 ✅
- Processing + MQTT Library ✅
- MQTT Broker 啟動 ✅
- LED 閃爍測試通過 ✅

### Part 2: 基礎互動（60min）
**目標**：掌握 ESP32 基本功能
- LED 控制 ✅
- 光敏感測器 ✅
- 觸控感測器 ✅

### Part 3: MQTT 整合（60min）
**目標**：完成雙向通訊
- ESP32 連上 MQTT ✅
- 手機控制 LED ✅
- RGB 色彩同步 ✅

### Part 4: 視覺藝術（45min）
**目標**：Processing 視覺化
- 色彩視覺化 ✅
- 粒子/3D 效果 ✅
- 創意挑戰 ✅

---

## 📋 關鍵指令速查

### MQTT Broker 啟動
```powershell
cd server
node broker.js
```

### Web Server 啟動
```powershell
cd web
.\serve.ps1
```

### MQTT 測試（PowerShell）
```powershell
# 訂閱
.\pubsub.ps1 -BrokerHost localhost -Topic "test/hello" -Subscribe

# 發布
.\pubsub.ps1 -BrokerHost localhost -Topic "test/hello" -Message "Hello"
```

### 發送 LED 控制指令
```powershell
.\pubsub.ps1 -BrokerHost localhost -Topic "iot/lab/esp32-01/cmd/led" -Message '{"cmd":"on"}'
```

### 發送顏色指令
```powershell
.\pubsub.ps1 -BrokerHost localhost -Topic "iot/lab/esp32-01/cmd/color" -Message '{"cmd":"color","value":"#ff0000"}'
```

---

## 🔧 常見問題快速修復

### ❌ ESP32 上傳失敗
1. 檢查 COM Port
2. 按住 BOOT 按鈕再上傳
3. 換 USB 線
4. 降低 Upload Speed (Tools > Upload Speed > 115200)

### ❌ WiFi 連不上
1. 確認 2.4GHz WiFi（不支援 5GHz）
2. 檢查 SSID/密碼大小寫
3. 重啟 ESP32
4. 確認訊號強度

### ❌ MQTT 連線失敗
1. `ping 192.168.1.XXX` 測試網路
2. 防火牆允許 1883, 9001
3. 重啟 broker.js
4. 檢查 MQTT_SERVER IP

### ❌ 手機控制無效
1. 確認 Device ID 正確
2. 手機與 ESP32 同一 WiFi
3. 檢查 Serial Monitor 是否收到訊息
4. 用 PowerShell 測試

### ❌ Processing 無反應
1. 確認 MQTT Library 已安裝
2. 修改 MQTT_HOST 為正確 IP
3. 查看 Console 錯誤訊息
4. 重啟 Processing

---

## 🎨 接線速查表

### LED + 按鈕
```
GPIO 25 → LED+ → 220Ω → GND
GPIO 26 → Button → GND (INPUT_PULLUP)
```

### 光敏電阻
```
光敏 1 腳 → 3.3V
光敏 2 腳 → GPIO 34 (ADC)
光敏 2 腳 → 10KΩ → GND
```

### 觸控感測器
```
GPIO 4 → 跳線（無需其他電路）
```

### RGB LED
```
GPIO 25 → R → 220Ω → GND
GPIO 26 → G → 220Ω → GND
GPIO 27 → B → 220Ω → GND
```

---

## 💡 白板資訊（記得寫）

```
📶 WiFi 資訊
SSID: ______________
密碼: ______________

🖥️ 講師電腦 IP
192.168.1.___

🌐 網頁位址
http://192.168.1.___:3000/

📡 MQTT Broker
mqtt://192.168.1.___:1883

🆔 Device ID 分配
esp32-01: 王同學
esp32-02: 李同學
esp32-03: 張同學
...
```

---

## 📁 範例程式位置

```
arduino/
├─ 01_button_led.ino          ← Part 2 開始
├─ 02_light_sensor.ino         ← 光敏感測器
├─ 03_touch_sensor.ino         ← 觸控感測器
├─ 04_mqtt_basic.ino           ← Part 3 開始
└─ custom/
   └─ art_color_led_mqtt.ino   ← RGB LED 控制

processing/
├─ IoT_Art_Visualizer.pde      ← Part 4 必教
├─ IoT_Particle_System.pde     ← 粒子效果
├─ IoT_3D_Cube.pde             ← 3D 方塊
└─ IoT_Generative_Art.pde      ← 生成藝術

web/
├─ index.html                  ← 基本控制
├─ art.html                    ← 色彩控制
└─ processing-control.html     ← Processing 控制台
```

---

## 🎤 重要話術

### 開場（激發興趣）
> 「各位同學好！今天我們要從零開始，用 4 小時建立一個完整的 IoT 系統。你們可以用手機搖一搖，控制 LED 燈光，甚至產生藝術視覺效果！」

### 每個階段開始前
> 「好，接下來這 20 分鐘，我們要完成 XXX。完成後你們就能看到 YYY 效果！」

### 檢查進度
> 「完成的同學請舉手！」  
> 「還沒完成的同學不用擔心，助教會來協助」

### 除錯鼓勵
> 「遇到錯誤很正常，這就是工程師的日常！我們一起來看看怎麼解決」

### 展示成果
> 「哇！你們看，XX 同學已經成功了！LED 亮起來了！」

### 挑戰引導
> 「現在給大家 5 分鐘自由發揮，看誰能創造最酷的效果！」

### 結尾
> 「恭喜大家！你們現在已經掌握了 IoT 的核心技術。回家後可以繼續玩，創作出自己的專案！」

---

## ✅ 課前準備清單（講師）

### 前一天
- [ ] 測試所有範例程式
- [ ] 確認 WiFi 可用
- [ ] 準備備用 USB 隨身碟
- [ ] 充電投影筆/麥克風
- [ ] 列印講義（可選）

### 上課前 1 小時
- [ ] 擺放硬體材料
- [ ] 開啟講師電腦
- [ ] 啟動 MQTT Broker
- [ ] 啟動 Web Server
- [ ] 測試投影設備
- [ ] 白板寫上 WiFi/IP 資訊
- [ ] 開啟範例程式（備用）

### 上課前 15 分鐘
- [ ] 播放背景音樂
- [ ] 展示最終 Demo
- [ ] 準備開場投影片
- [ ] 確認麥克風正常

---

## 🎁 學員離開前確認

- [ ] 填寫問卷
- [ ] 加入課後群組
- [ ] 領取 USB（程式碼）
- [ ] 拍照合影

---

## 📞 緊急聯絡

**技術支援**：
- IT 部門：分機 XXX
- WiFi 問題：分機 XXX

**場地問題**：
- 設備故障：分機 XXX

---

## 💾 備份方案

### 如果 WiFi 掛掉
- 改用 ESP32 AP 模式
- 手機直接連 ESP32 熱點

### 如果 MQTT Broker 掛掉
- 使用雲端 MQTT Broker（test.mosquitto.org）

### 如果 Processing 安裝失敗
- 跳過 Processing，專注在 ESP32 + Web

### 如果進度嚴重落後
- 快速 Demo 剩餘內容
- 提供完整程式碼讓學員回家玩

---

**祝教學成功！🎉**

_隨時回來查閱這張卡片，保持節奏流暢！_
