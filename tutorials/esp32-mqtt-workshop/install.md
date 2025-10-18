# 安裝與準備（Windows）

本章節帶你在 30~45 分鐘內完成所有安裝與驗證。

## 1. 安裝 Arduino 與 ESP32 核心
- 安裝 Arduino IDE 2.x：https://www.arduino.cc/en/software/
- 安裝 CP210x 驅動（若需）：https://www.silabs.com/software-and-tools/usb-to-uart-bridge-vcp-drivers
- 在 Arduino 偏好設定的「Additional Boards Manager URLs」加入：
  - https://espressif.github.io/arduino-esp32/package_esp32_index.json
- 開啟 Boards Manager，搜尋 ESP32 並安裝。

必裝函式庫（用於 MQTT 範例）：
- PubSubClient（在 Library Manager 搜尋並安裝）

驗證：連接 ESP32，於「工具 > 開發板」選擇對應型號，「工具 > 埠」選擇正確 COM 埠，燒錄「Blink」。

## 2. 安裝 MQTT Broker（二選一）

選項 A：Mosquitto（本機 TCP + WebSocket）
- 安裝 Windows 版 Mosquitto：https://mosquitto.org/download/
- 將 `server/mosquitto.conf` 複製到 Mosquitto 安裝資料夾或任意資料夾。
- 啟動服務：在 PowerShell 以系統管理員執行，使用說明見 `server/README.md`。

選項 B：Node 版 Aedes（快速內建）
- 進入 `server/` 目錄安裝依賴，啟動簡易 Broker（支援 1883 與 9001）。

注意：請確認防火牆放行 1883（TCP）與 9001（WebSocket）。

## 3. 安裝 VS Code（可選，但強烈建議）
- https://code.visualstudio.com/
- 安裝 Markdown Preview、Marp for VS Code（用於投影片）。

## 4. 手機端網頁需求
- 同一個 Wi-Fi
- 支援現代瀏覽器（Chrome/Safari）
 - 建議使用本專案的簡易靜態伺服器（`tutorials/esp32-mqtt-workshop/web` 內提供），讓手機以 http://<你的電腦IP>:3000 開啟網頁。

## 5. 測試 MQTT 連線
- 使用 `server/test.bat` 或 README 指令測試 `publish`/`subscribe`。
- 或用 MQTTX、MQTT Explorer 測試。

## 6. 範例程式的變數需調整
- `arduino/04_mqtt_basic.ino`：
  - WIFI_SSID、WIFI_PASS 改為你的 Wi-Fi
  - MQTT_HOST 改為 Broker 的 IP（範例：你的電腦 IP）
  - 若使用外接 LED，確認 LED_PIN 腳位；若用板載 LED 可改成 2
- `web/index.html`：
  - WebSocket URL 預設 ws://localhost:9001；在手機上請改成 ws://<你的電腦IP>:9001
