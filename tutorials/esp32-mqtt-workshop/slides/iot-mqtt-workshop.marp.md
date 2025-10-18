---
marp: true
theme: default
paginate: true
author: IoT Cross-Media
size: 16:9
---

# ESP32 × MQTT 工作坊（3~5 小時）

- 架構與概念
- 安裝與實作
- 手機網頁整合

---

## IoT 架構速覽

- 裝置層（ESP32）
- 通訊層（MQTT）
- 平台層（Broker/Server）
- 應用層（Web/行動端）

---

## 為什麼用 MQTT？

- 輕量、即時、廣泛支援
- Pub/Sub 解耦
- QoS、Retain、Last Will

---

## 今日目標

1. 在 Windows 啟動本地 Broker（1883 / 9001）
2. 燒錄 4 個 ESP32 範例並驗證
3. 手機網頁透過 WebSocket 控制/觀察
4. 理解 Topic 與 Payload 設計

---

## 安裝清單

- Arduino IDE 2.x + ESP32 Boards
- Mosquitto 或 Node 版 Aedes
- VS Code + Marp（選）

詳見 `install.md`

---

## Topic 設計建議

- 感測：`iot/{site}/{device}/sensor/{type}`
- 控制：`iot/{site}/{device}/cmd/{act}`
- 狀態：`iot/{site}/{device}/status`

詳見 `topics.md`

---

## 範例一：按鈕 + LED

- 觀念：數位 IO
- 練習：防彈跳（延伸）

---

## 範例二：光敏電阻（ADC）

- 觀念：類比量測 0..4095
- 練習：門檻判斷/平均濾波

---

## 範例三：觸控/TTP223

- 觀念：數位輸入或觸控讀值
- 練習：狀態機切換 LED

---

## 範例四：MQTT 基礎

- 連線、訂閱指令、發布感測
- Web 端用 MQTT.js 測試

---

## Demo：手機網頁

- 連 `ws://<broker-ip>:9001`
- 訂閱 `iot/+/+/sensor/+`
- 發布 `iot/lab/<id>/cmd/led`

---

## 常見問題

- 防火牆阻擋 1883/9001
- 不同網段/不可達
- COM 埠與驅動

---

## 🎨 藝術互動挑戰

- 手機撈取色塊 → MQTT → ESP32 RGB LED
- 同步到 Processing 視覺藝術
- 多人協作、即時互動

---

## Demo：IoT Art

1. 手機開啟 `web/art.html`
2. ESP32 燒錄 RGB 控制程式
3. Processing 執行視覺藝術程式
4. 點選色塊，三端同步變色

---

## Processing 視覺藝術

- 安裝 MQTT 函式庫
- 訂閱 `iot/lab/+/cmd/color`
- 即時變色、粒子效果
- 學生可自訂視覺元素

---

## 結語與延伸

- 增加 QoS1、Retain 狀態
- 加入設備註冊、心跳
- 後端儲存與可視化
- 🎨 創意專題：IoT × 藝術

感謝參與！
