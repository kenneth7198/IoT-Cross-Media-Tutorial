# ESP32 + MQTT 3~5 小時工作坊

本工作坊將用 4 個 ESP32 範例逐步完成 IoT 到 Web 的端到端串接：
- MQTT Broker 架設（Windows）
- Topic 與 Payload 設計
- ESP32 端（Arduino）發佈/訂閱
- 手機端網頁（MQTT over WebSocket）
- 前後端架構與資料流講解
- 投影片（Marp）與講義

## 課程節奏建議（約 4 小時，可調至 3~5 小時）
1) 30~45 分鐘：環境安裝 + Broker 架設
2) 45~60 分鐘：ESP32 基礎 IO 與接線（LED/按鈕/光敏/電容）
3) 60 分鐘：MQTT 入門與四個範例跑通
4) 30 分鐘：手機網頁 + WebSocket 測試
5) 15 分鐘：整體架構與實作 Q&A

## 內容目錄
- **快速啟動**：`QUICKSTART.md` ⭐ 新手從這開始
- 安裝指南：`install.md`
- 講義與大綱：`materials.md`
- Topic 與訊息格式：`topics.md`
- ESP32 程式碼：`arduino/`
- 手機網頁前端：`web/`
- Processing 視覺藝術：`processing/` 🎨
- 本地測試工具（Node + MQTT）：`server/`
- 投影片：`slides/iot-mqtt-workshop.marp.md`

---

## 🎨 藝術互動挑戰（IoT Art）

### 完整互動鏈

手機 → MQTT → ESP32 → Processing 視覺藝術

### 操作流程

1. **手機端**：開啟 `web/art.html`，點選畫作色塊
2. **ESP32 端**：燒錄 `arduino/custom/art_color_led_mqtt.ino`，RGB LED 變色
3. **Processing 端**：執行 `processing/IoT_Art_Visualizer.pde`，畫面即時變色並產生粒子效果

### Topic 與格式

- Topic: `iot/lab/esp32-01/cmd/color`
- Payload: `{"cmd":"color","value":"#e74c3c"}`

### 進階玩法

- 多人協作：同時控制不同視覺元素
- 顏色接力：每人依序撈取顏色完成拼圖
- 自訂畫布：學生設計自己的互動藝術作品

## 先決條件
- Windows 10/11
- 安裝 Arduino IDE 2.x 與 ESP32 Board
- 有網路（同一個 Wi-Fi）
- 1~4 台 ESP32 開發板 + LED、按鈕、光敏電阻、TTP223（或可用板載觸控/ADC 替代）

更多背景說明可參考倉庫根目錄的 `MQTT整合說明.md`。
