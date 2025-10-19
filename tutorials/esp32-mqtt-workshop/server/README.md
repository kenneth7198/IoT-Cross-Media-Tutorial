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

---

## 測試 MQTT Broker 的完整步驟

### 步驟 1：啟動 MQTT Broker

#### 使用 Mosquitto（Windows）
```powershell
# 方法 1：前景執行（推薦用於測試，可看到詳細日誌）
"C:\Program Files\mosquitto\mosquitto.exe" -c .\mosquitto.conf -v

# 方法 2：背景服務
Start-Service mosquitto
```

#### 使用 Mosquitto（macOS/Linux）
```bash
# 使用配置檔啟動
mosquitto -c ./mosquitto.conf -v

# 或使用預設配置
mosquitto
```

#### 使用 Node.js Aedes
```bash
node broker.js
```

### 步驟 2：測試 Broker 連線

#### 方法 A：使用 Mosquitto 客戶端工具

**終端機 1 - 訂閱主題（Subscriber）**
```bash
# 訂閱單一主題
mosquitto_sub -h localhost -t "test/hello" -v

# 訂閱所有主題（萬用字元）
mosquitto_sub -h localhost -t "#" -v

# 訂閱特定前綴的所有主題
mosquitto_sub -h localhost -t "sensor/#" -v

# 帶使用者認證（如果 broker 有設定）
mosquitto_sub -h localhost -u username -P password -t "test/#" -v
```

**終端機 2 - 發布訊息（Publisher）**
```bash
# 發布簡單文字訊息
mosquitto_pub -h localhost -t "test/hello" -m "Hello MQTT!"

# 發布 JSON 格式訊息
mosquitto_pub -h localhost -t "sensor/temperature" -m '{"value":25.5,"unit":"C"}'

# 發布保留訊息（retained message）
mosquitto_pub -h localhost -t "status/device1" -m "online" -r

# 連續發布多則訊息
for i in {1..10}; do mosquitto_pub -h localhost -t "test/count" -m "$i"; sleep 1; done
```

#### 方法 B：使用 MQTTX GUI 工具

1. 下載並安裝 [MQTTX](https://mqttx.app/)
2. 建立新連線：
   - Name: `Local Broker`
   - Host: `mqtt://localhost`
   - Port: `1883`
3. 點擊 "Connect"
4. 在訂閱區新增訂閱主題：`test/#`
5. 在發布區輸入：
   - Topic: `test/hello`
   - Payload: `Hello from MQTTX!`
6. 點擊發送

#### 方法 C：使用 Node.js MQTT 客戶端

**安裝客戶端**
```bash
npm install mqtt --save
```

**快速測試腳本（test-mqtt.js）**
```javascript
const mqtt = require('mqtt');
const client = mqtt.connect('mqtt://localhost:1883');

client.on('connect', () => {
  console.log('✅ Connected to MQTT Broker');
  
  // 訂閱主題
  client.subscribe('test/#', (err) => {
    if (!err) {
      console.log('📥 Subscribed to test/#');
      
      // 發布測試訊息
      client.publish('test/hello', 'Hello from Node.js!');
      console.log('📤 Published message');
    }
  });
});

client.on('message', (topic, message) => {
  console.log(`📨 Received: [${topic}] ${message.toString()}`);
});

client.on('error', (err) => {
  console.error('❌ Connection error:', err);
});
```

**執行測試**
```bash
node test-mqtt.js
```

### 步驟 3：測試 WebSocket 連線（Port 9001）

**使用瀏覽器 Console 測試**
```javascript
// 在瀏覽器開發者工具 Console 中執行
const client = new Paho.MQTT.Client('localhost', 9001, 'clientId_' + Date.now());

client.onConnectionLost = (response) => {
  console.log('Connection Lost:', response.errorMessage);
};

client.onMessageArrived = (message) => {
  console.log('Message:', message.destinationName, message.payloadString);
};

client.connect({
  onSuccess: () => {
    console.log('Connected via WebSocket!');
    client.subscribe('test/#');
    
    const message = new Paho.MQTT.Message('Hello from WebSocket');
    message.destinationName = 'test/websocket';
    client.send(message);
  }
});
```

### 步驟 4：驗證 Broker 狀態

**檢查 Broker 系統主題**
```bash
# 查看 broker 版本
mosquitto_sub -h localhost -t "\$SYS/broker/version" -v -C 1

# 查看連線的客戶端數量
mosquitto_sub -h localhost -t "\$SYS/broker/clients/connected" -v

# 查看所有系統資訊
mosquitto_sub -h localhost -t "\$SYS/#" -v
```

### 步驟 5：常見測試場景

#### 測試 QoS 等級
```bash
# QoS 0 (最多一次)
mosquitto_pub -h localhost -t "test/qos0" -m "QoS 0 message" -q 0

# QoS 1 (至少一次)
mosquitto_pub -h localhost -t "test/qos1" -m "QoS 1 message" -q 1

# QoS 2 (確保一次)
mosquitto_pub -h localhost -t "test/qos2" -m "QoS 2 message" -q 2
```

#### 測試保留訊息
```bash
# 發布保留訊息
mosquitto_pub -h localhost -t "status/light" -m "ON" -r

# 新訂閱者會立即收到最後的保留訊息
mosquitto_sub -h localhost -t "status/light" -v -C 1
```

#### 壓力測試
```bash
# 快速發送 1000 則訊息
for i in {1..1000}; do 
  mosquitto_pub -h localhost -t "stress/test" -m "Message $i"
done
```

### 故障排除

#### Broker 無法啟動
```bash
# 檢查端口是否被佔用
netstat -an | grep 1883
lsof -i :1883  # macOS/Linux

# Windows
netstat -ano | findstr 1883
```

#### 連線被拒絕
```bash
# 測試 TCP 連線
telnet localhost 1883
nc -zv localhost 1883  # macOS/Linux

# 檢查防火牆設定
```

#### 訊息收不到
```bash
# 使用詳細模式查看完整日誌
mosquitto -c ./mosquitto.conf -v

# 檢查主題名稱是否正確（區分大小寫）
# 檢查 QoS 設定
# 確認訂閱時機（是否在發布之前訂閱）
```

### 成功指標

測試成功時，您應該看到：
- ✅ Broker 成功啟動並監聽 1883 和 9001 端口
- ✅ 訂閱者能成功連線並訂閱主題
- ✅ 發布者發送的訊息能被訂閱者即時接收
- ✅ WebSocket 連線正常運作
- ✅ 多個客戶端可同時連線

