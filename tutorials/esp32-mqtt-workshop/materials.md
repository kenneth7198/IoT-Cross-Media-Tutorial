# 講義與實作大綱

本講義搭配投影片 `slides/iot-mqtt-workshop.marp.md` 使用，內容包含：

## A. IoT 與 MQTT 基礎（20 分鐘）
- IoT 四層架構：裝置層/通訊層/平台層/應用層
- MQTT 角色：Broker、Publisher、Subscriber
- QoS、Retain、Last Will 選讀

## B. Topic 與 Payload 設計（15 分鐘）
- 命名建議：`iot/{site}/{deviceId}/{capability}`
- 範例：`iot/lab/esp32-01/sensor/light`、`iot/lab/esp32-01/cmd/led`
- JSON 格式約定：timestamp、unit、value

## C. ESP32 端四個範例（60~90 分鐘）
- 1. LED + 按鈕（數位 IO）
- 2. 光敏電阻（ADC）
- 3. TTP223/觸控（數位或觸控）
- 4. MQTT 基礎：連線、發布、訂閱

## D. 手機網頁（30 分鐘）
- 用 MQTT.js 透過 WebSocket 訂閱 ESP32 訊息
- 在手機上控制 ESP32 LED（發佈指令）

## E. 架構與資料流（15 分鐘）
- 端到端流程與風險點（網路、Broker、Topic 類型）
- 除錯建議與常見問題

## F. 藝術互動挑戰（可選 30~60 分鐘）
- 手機撈取畫作色塊，MQTT 同步到 ESP32 與 Processing
- RGB LED 變色
- Processing 視覺藝術即時反應
- 多人協作、創意專題

## G. 作業與延伸
- 加入多裝置命名、QoS 1
- 設計基本的設備註冊與心跳
- 創意專題：IoT × 藝術跨界作品
