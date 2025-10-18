# 手機前端（MQTT.js）

## 兩種開啟方式

1) 直接用檔案方式開啟 `index.html`（僅限桌面瀏覽器測試）
2) 使用 PowerShell 啟動同網段靜態伺服器，供手機連線：

PowerShell
./serve.ps1 -Port 3000

然後在手機以瀏覽器開啟：http://<你的電腦IP>:3000

## 連線設定
- 預設 WebSocket URL：ws://localhost:9001
- 若用手機，請改成 ws://<你的電腦IP>:9001

## 功能
- 訂閱多個 topic（以逗號分隔）
- 發布控制命令到 `iot/lab/<deviceId>/cmd/led`
- 發布自訂 topic/payload
