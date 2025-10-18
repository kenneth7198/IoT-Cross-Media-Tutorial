# 教材完整性檢查清單

## ✅ 文件與講義

- [x] `README.md` - 工作坊總覽
- [x] `QUICKSTART.md` - 15分鐘快速啟動 ⭐
- [x] `install.md` - 安裝與環境設定
- [x] `materials.md` - 講義大綱與課程結構
- [x] `topics.md` - Topic 與 JSON 格式設計

## ✅ ESP32 程式（arduino/）

### 基礎範例
- [x] `01_button_led.ino` - 按鈕控制 LED
- [x] `02_lightsensor_adc.ino` - 光敏電阻 ADC
- [x] `03_touch_or_ttp223.ino` - 電容觸控
- [x] `04_mqtt_basic.ino` - MQTT 基礎（發布/訂閱）

### 學生原始程式（custom/）
- [x] `custom_button_led.ino` - 微動開關控制 LED
- [x] `custom_light_touch.ino` - 光敏+電容感測
- [x] `custom_button_led_mqtt.ino` - MQTT 版按鈕 LED
- [x] `custom_light_touch_mqtt.ino` - MQTT 版光敏電容
- [x] `art_color_led_mqtt.ino` - RGB LED 藝術互動
- [x] `TEMPLATE_mqtt_device.ino` - 學生自訂範本

### 設定檔
- [x] `config.example.h` - Wi-Fi/MQTT 設定範例
- [x] `README.md` - 程式使用說明

## ✅ 手機網頁（web/）

- [x] `index.html` - 基本 MQTT 控制台（LED ON/OFF、手機搖晃）
- [x] `art.html` - 藝術互動（色塊撈取）
- [x] `serve.ps1` - PowerShell 靜態伺服器
- [x] `README.md` - 網頁使用說明

## ✅ Processing 視覺藝術（processing/）

- [x] `IoT_Art_Visualizer.pde` - MQTT 視覺藝術程式
- [x] `README.md` - Processing 安裝與使用

## ✅ MQTT 伺服器（server/）

- [x] `broker.js` - Node Aedes MQTT Broker
- [x] `mosquitto.conf` - Mosquitto 設定檔
- [x] `package.json` - Node 依賴定義
- [x] `pubsub.ps1` - PowerShell 發布/訂閱工具
- [x] `test.bat` - 快速測試腳本
- [x] `README.md` - 伺服器啟動說明

## ✅ 圖表與架構（assets/）

- [x] `wiring.md` - 接線指南（Mermaid 圖）
- [x] `architecture.md` - 端到端時序圖

## ✅ 投影片（slides/）

- [x] `iot-mqtt-workshop.marp.md` - Marp 投影片（IoT/MQTT/藝術互動）

## ✅ 功能測試

### MQTT Broker
- [ ] Node 版 Aedes 可啟動（port 1883, 9001）
- [ ] Mosquitto 可用設定檔啟動
- [ ] `test.bat` 可正常收送訊息

### ESP32 程式
- [ ] 基礎範例可編譯
- [ ] MQTT 版本可連線 Broker
- [ ] `config.h` 設定正確後可燒錄

### 手機網頁
- [ ] `serve.ps1` 可啟動靜態伺服器
- [ ] 手機可開啟 `index.html` 與 `art.html`
- [ ] WebSocket 連線成功
- [ ] 手機搖晃可觸發 LED 控制
- [ ] 色塊點選可同步到 ESP32

### Processing
- [ ] MQTT 函式庫已安裝
- [ ] 程式可連線 Broker
- [ ] 收到 MQTT 顏色指令可變色
- [ ] 粒子效果正常顯示

## ✅ 課程規劃

- [x] 3~5 小時課程節奏建議
- [x] 安裝步驟（30~45 分鐘）
- [x] 基礎 IO 實作（45~60 分鐘）
- [x] MQTT 整合（60 分鐘）
- [x] 手機網頁（30 分鐘）
- [x] 藝術互動挑戰（30~60 分鐘，可選）
- [x] 架構回顧與 Q&A（15 分鐘）

## ✅ 教學素材

- [x] 安裝步驟清楚（含截圖建議位置）
- [x] 程式碼註解完整
- [x] Topic 命名規範
- [x] JSON 格式範例
- [x] 常見問題排除
- [x] 進階挑戰題

## 📝 建議改進（可選）

- [ ] 加入電路圖（Fritzing/真實照片）
- [ ] 錄製 Demo 影片
- [ ] 加入學生作品展示區
- [ ] 建立線上版 MQTT Broker 教學
- [ ] 加入更多 Processing 視覺效果範例

## 🎉 教材狀態：完整可用

所有必要檔案已建立，可直接用於 3~5 小時工作坊教學。

---

最後更新：2025-10-19
