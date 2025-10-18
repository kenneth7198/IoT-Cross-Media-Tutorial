# Topic 與訊息格式設計

建議遵循自描述、可擴充且容易分流的命名：

- 感測資料：`iot/{site}/{deviceId}/sensor/{type}`
- 事件/狀態：`iot/{site}/{deviceId}/status`
- 控制命令：`iot/{site}/{deviceId}/cmd/{act}`
- 廣播：`iot/{site}/+/cmd/{act}`（+ 表示單層萬用字元）

## JSON 格式

感測資料
{
  "deviceId": "esp32-01",
  "type": "light",
  "value": 523,
  "unit": "adc",
  "ts": 1739900000000
}

控制命令
{
  "cmd": "led",
  "value": 1,
  "ts": 1739900000500
}

狀態心跳
{
  "deviceId": "esp32-01",
  "ip": "192.168.1.23",
  "rssi": -55,
  "uptime": 120000,
  "ts": 1739900001000
}

## QoS 與 Retain
- 感測資料：QoS 0/1，通常不 Retain
- 目標狀態：可用 Retain 發佈最後狀態
- 控制命令：QoS 1 建議；必要時可增加 ack topic
