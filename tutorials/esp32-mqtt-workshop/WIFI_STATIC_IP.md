# ESP32 WiFi 固定 IP 設定說明

本專案所有使用 WiFi 的 ESP32 範例程式已更新為**固定 IP 設定**。

## 🌐 網路設定

```cpp
// WiFi 設定
SSID:     NetArt
密碼:     1qaz2wsx

// 固定 IP 設定
IP:       192.168.100.201
子網路:   255.255.255.0
閘道:     192.168.100.1
DNS:      8.8.8.8
```

---

## 📝 已更新的檔案清單

### 1. config.example.h
- ✅ 新增固定 IP 參數定義
- ✅ 更新 WiFi SSID/密碼為 NetArt
- ✅ 更新 MQTT_HOST 為 192.168.100.1

### 2. 04_mqtt_basic.ino
- ✅ 加入固定 IP 變數宣告
- ✅ 更新 ensureWiFi() 函式（加入 WiFi.config()）
- ✅ 更新 WiFi SSID/密碼

### 3. custom/TEMPLATE_mqtt_device.ino
- ✅ 加入固定 IP 變數宣告
- ✅ 更新 ensureWiFi() 函式

### 4. custom/custom_button_led_mqtt.ino
- ✅ 加入固定 IP 變數宣告
- ✅ 更新 ensureWiFi() 函式

### 5. custom/custom_light_touch_mqtt.ino
- ✅ 加入固定 IP 變數宣告
- ✅ 更新 ensureWiFi() 函式

### 6. custom/art_color_led_mqtt.ino
- ✅ 加入固定 IP 變數宣告
- ✅ 更新 ensureWiFi() 函式

---

## 🔧 程式碼變更說明

### 原本的程式碼（DHCP 自動取得 IP）
```cpp
void ensureWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) { 
    delay(300); 
  }
}
```

### 更新後的程式碼（固定 IP）
```cpp
// 固定 IP 設定（在程式開頭加入）
IPAddress local_IP(192, 168, 100, 201);
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway(192, 168, 100, 1);
IPAddress dns(8, 8, 8, 8);

void ensureWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;
  WiFi.mode(WIFI_STA);
  
  // 設定固定 IP
  if (!WiFi.config(local_IP, gateway, subnet, dns)) {
    Serial.println("Static IP Failed!");
  }
  
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("WiFi connecting");
  while (WiFi.status() != WL_CONNECTED) { 
    delay(300); 
    Serial.print("."); 
  }
  Serial.println(" OK");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
```

---

## 🎯 關鍵變更點

1. **加入固定 IP 變數宣告**
   ```cpp
   IPAddress local_IP(192, 168, 100, 201);
   IPAddress subnet(255, 255, 255, 0);
   IPAddress gateway(192, 168, 100, 1);
   IPAddress dns(8, 8, 8, 8);
   ```

2. **在 WiFi.begin() 之前呼叫 WiFi.config()**
   ```cpp
   WiFi.config(local_IP, gateway, subnet, dns);
   ```

3. **改善除錯訊息**
   - 顯示連線過程（點點點）
   - 顯示最終取得的 IP（確認是否為固定 IP）

---

## ⚙️ 如何修改 IP 設定

如果需要使用不同的 IP 位址，請修改以下變數：

```cpp
// 範例：改成 192.168.100.202
IPAddress local_IP(192, 168, 100, 202);  // ← 改這裡
IPAddress subnet(255, 255, 255, 0);      // 子網路遮罩通常不變
IPAddress gateway(192, 168, 100, 1);     // 閘道通常不變
IPAddress dns(8, 8, 8, 8);               // DNS 通常不變
```

**注意事項**：
- 確保 IP 不與其他裝置衝突
- 每個 ESP32 要設定不同的 IP（例如 .201, .202, .203...）
- IP 需在同一網段（例如都是 192.168.100.xxx）

---

## 🧪 測試驗證

### 1. 上傳程式後檢查 Serial Monitor

**預期輸出**：
```
WiFi connecting........... OK
IP Address: 192.168.100.201
MQTT...OK
```

**如果 IP 不是 192.168.100.201**：
- 檢查程式碼中的 `local_IP` 設定
- 確認 `WiFi.config()` 有被呼叫

### 2. 測試網路連線

**在電腦上 ping 測試**：
```powershell
ping 192.168.100.201
```

**預期結果**：
```
Reply from 192.168.100.201: bytes=32 time=10ms TTL=255
```

### 3. 測試 MQTT 連線

**確認 ESP32 能連上 MQTT Broker**：
- Serial Monitor 顯示 "MQTT...OK"
- 可以接收/發送 MQTT 訊息

---

## 🚨 常見問題

### ❌ 問題：WiFi 連線失敗

**可能原因**：
1. SSID/密碼錯誤
2. WiFi 是 5GHz（ESP32 僅支援 2.4GHz）
3. 固定 IP 設定錯誤（IP 與閘道不在同網段）

**解決方式**：
```cpp
// 檢查設定
const char* WIFI_SSID = "NetArt";      // ← 確認 SSID 正確
const char* WIFI_PASS = "1qaz2wsx";    // ← 確認密碼正確
IPAddress local_IP(192, 168, 100, 201); // ← 確認與閘道同網段
IPAddress gateway(192, 168, 100, 1);    // ← 確認閘道 IP
```

---

### ❌ 問題：IP 不是設定的固定 IP

**可能原因**：
- `WiFi.config()` 回傳 false（設定失敗）
- 路由器 DHCP 覆蓋固定 IP

**解決方式**：
```cpp
// 檢查 WiFi.config() 回傳值
if (!WiFi.config(local_IP, gateway, subnet, dns)) {
  Serial.println("Static IP Failed!");  // ← 會出現這個訊息
}
```

**如果出現 "Static IP Failed!"**：
- 檢查 IP 格式（192, 168, 100, 201）逗號分隔
- 確認參數順序：IP, Gateway, Subnet, DNS

---

### ❌ 問題：多個 ESP32 IP 衝突

**現象**：
- 某些 ESP32 連線不穩
- ping 時斷時續

**解決方式**：
每個 ESP32 使用不同的 IP：
```cpp
// ESP32 #1
IPAddress local_IP(192, 168, 100, 201);

// ESP32 #2
IPAddress local_IP(192, 168, 100, 202);

// ESP32 #3
IPAddress local_IP(192, 168, 100, 203);
```

---

## 📋 檢查清單（課前準備）

講師準備多個 ESP32 時：

- [ ] 每個 ESP32 設定不同 IP（.201, .202, .203...）
- [ ] 每個 ESP32 設定不同 DEVICE_ID（esp32-01, esp32-02...）
- [ ] 確認所有 IP 在同網段（192.168.100.xxx）
- [ ] 測試每個 ESP32 網路連線
- [ ] 記錄 IP 與 DEVICE_ID 對應表

**對應表範例**：
```
ESP32 #1 -> IP: 192.168.100.201 -> ID: esp32-01
ESP32 #2 -> IP: 192.168.100.202 -> ID: esp32-02
ESP32 #3 -> IP: 192.168.100.203 -> ID: esp32-03
```

---

## 🎓 教學注意事項

### 講師說明重點

1. **為什麼使用固定 IP？**
   - 避免 DHCP 衝突
   - 方便管理與除錯
   - 網路穩定性更高

2. **固定 IP 的限制**
   - 必須在正確的網段
   - 不能與其他裝置重複
   - 需要知道閘道 IP

3. **如何選擇 IP？**
   - 通常選擇 .200 以後的 IP（避開 DHCP 範圍）
   - 確認路由器設定（有些路由器 DHCP 範圍是 .100-.199）

---

## 🔄 切換回 DHCP（如果需要）

如果需要改回自動取得 IP，移除 `WiFi.config()` 即可：

```cpp
void ensureWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;
  WiFi.mode(WIFI_STA);
  
  // 註解或移除這段
  // if (!WiFi.config(local_IP, gateway, subnet, dns)) {
  //   Serial.println("Static IP Failed!");
  // }
  
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  // ... 其餘不變
}
```

---

## ✅ 更新完成確認

- ✅ config.example.h 已更新
- ✅ 6 個 Arduino 範例程式已更新
- ✅ 所有程式加入固定 IP 設定
- ✅ WiFi SSID/密碼已改為 NetArt/1qaz2wsx
- ✅ MQTT_HOST 已改為 192.168.100.1
- ✅ 固定 IP 設為 192.168.100.201

**準備就緒！可以開始上課了！** 🎉
