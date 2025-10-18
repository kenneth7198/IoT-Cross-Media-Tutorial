# 本地 MQTT 伺服器與工具

此資料夾提供兩種方式啟動本地 MQTT：

## 方式 A：Mosquitto（推薦）
- 準備 `mosquitto.conf`，內容同目錄提供的範例，啟用：
  - TCP: 1883
  - WebSocket: 9001
- 啟動（系統管理員 PowerShell）：
  - 安裝服務：`"C:\Program Files\mosquitto\mosquitto.exe" -install`
  - 啟動服務：`Start-Service mosquitto`
  - 或以前景執行：`"C:\Program Files\mosquitto\mosquitto.exe" -c .\mosquitto.conf -v`
- 測試訂閱與發布（需另裝 mosquitto 客戶端或用 MQTTX）：
  - 訂閱：`mosquitto_sub -h localhost -t "test/#" -v`
  - 發布：`mosquitto_pub -h localhost -t "test/hello" -m "hi"`

## 方式 B：Node（Aedes）
- `npm init -y` 後安裝 `aedes`、`ws`、`uWebSockets.js`（可選）
- 直接執行 `broker.js` 開啟 1883 與 9001 兩個通道。

## 小工具
- `pubsub.ps1`：PowerShell 版快速發布/訂閱
- `test.bat`：範例測試腳本
