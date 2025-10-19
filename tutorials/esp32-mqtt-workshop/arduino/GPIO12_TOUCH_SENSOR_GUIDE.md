# GPIO 12 電容觸摸感測器接線說明

## ✅ **確認：GPIO 12 數位輸入不受 WiFi 影響**

| 功能 | 狀態 |
|------|------|
| `digitalRead(12)` | ✅ WiFi 開啟時正常工作 |
| `digitalWrite(12)` | ✅ WiFi 開啟時正常工作 |
| `analogRead(12)` | ❌ WiFi 開啟時無法使用 |

---

## 🔌 **硬體接線**

### **常見觸摸感測器模組（TTP223）**

```
電容觸摸模組          ESP32
┌─────────────┐    ┌──────────┐
│   TTP223    │    │          │
│             │    │          │
│ VCC ────────┼────┤ 3.3V     │
│ GND ────────┼────┤ GND      │
│ SIG/OUT ────┼────┤ GPIO 12  │
│             │    │          │
└─────────────┘    └──────────┘
```

### **接線步驟**

1. **VCC（電源）**
   - 連接到 ESP32 的 **3.3V** 或 **5V**
   - 大多數 TTP223 模組支援 3.3V 和 5V

2. **GND（接地）**
   - 連接到 ESP32 的 **GND**

3. **SIG/OUT（訊號輸出）**
   - 連接到 ESP32 的 **GPIO 12**

---

## 📊 **工作原理**

### **觸摸邏輯**

```
未觸摸：GPIO 12 = HIGH (3.3V)
  │
  │  (用手指觸摸感測器)
  ▼
觸摸中：GPIO 12 = LOW (0V)
  │
  │  (手指離開)
  ▼
未觸摸：GPIO 12 = HIGH (3.3V)
```

---

## 💻 **程式碼重點**

### **初始化設定**

```cpp
// 設定為數位輸入，使用內部上拉
pinMode(TOUCH_SENSOR_PIN, INPUT_PULLUP);

// 讀取狀態
int touchState = digitalRead(TOUCH_SENSOR_PIN);

if (touchState == LOW) {
  // 觸摸中
} else {
  // 未觸摸
}
```

### **MQTT 訊息格式**

```
主題：sensor/touch
訊息：
  "1" ← 觸摸
  "0" ← 釋放
```

---

## 🧪 **測試步驟**

### **1. 上傳程式**
- 將修改後的程式上傳到 ESP32
- 開啟序列監控器（115200 baud）

### **2. 確認初始化**
```
=== ESP32 電容觸摸感測器 MQTT 發送器 ===
觸摸感測器腳位: GPIO 12
✅ GPIO 12 數位輸入不受 WiFi 影響

初始狀態: 未觸摸 (HIGH)

正在連接 WiFi: NetArt
...
```

### **3. 測試觸摸**
- 用手指觸摸感測器
- 觀察序列監控器輸出

預期輸出：
```
✓ 發送成功 [sensor/touch] 狀態: 觸摸 (1)
✓ 發送成功 [sensor/touch] 狀態: 釋放 (0)
```

### **4. 訂閱 MQTT 訊息**

在電腦終端機執行：
```bash
mosquitto_sub -h 192.168.100.200 -t "sensor/touch" -v
```

看到的訊息：
```
sensor/touch 1  ← 觸摸時
sensor/touch 0  ← 釋放時
```

---

## 🛠️ **常見問題排除**

### **問題 1：沒有反應**

**可能原因：**
1. 接線錯誤
2. 觸摸模組未供電
3. GPIO 腳位設定錯誤

**解決方法：**
```cpp
// 測試程式
void loop() {
  int state = digitalRead(12);
  Serial.print("GPIO 12: ");
  Serial.println(state ? "HIGH" : "LOW");
  delay(500);
}
```

---

### **問題 2：邏輯相反**

有些觸摸模組：
- 觸摸 = HIGH
- 未觸摸 = LOW

**修改程式碼：**
```cpp
if (touchState == HIGH) {  // 改成 HIGH
  message = "1";  // 觸摸
  status = "觸摸";
} else {
  message = "0";  // 未觸摸
  status = "釋放";
}
```

---

### **問題 3：靈敏度問題**

調整觸摸模組上的可變電阻（如果有）：
- 順時針轉 → 提高靈敏度
- 逆時針轉 → 降低靈敏度

---

## 🎯 **常見觸摸模組規格**

### **TTP223 電容觸摸模組**
- 工作電壓：2.0V - 5.5V
- 工作電流：1.5μA
- 輸出電流：< 8mA
- 響應時間：60ms - 220ms
- 觸摸板大小：可調

### **接線標示對照**

| 模組標示 | 說明 | 連接 |
|---------|------|------|
| VCC/+ | 電源 | 3.3V 或 5V |
| GND/- | 接地 | GND |
| SIG/OUT/IO | 訊號 | GPIO 12 |

---

## ✅ **驗證清單**

- [ ] VCC 連接到 3.3V 或 5V
- [ ] GND 連接到 GND
- [ ] OUT 連接到 GPIO 12
- [ ] 序列監控器顯示初始化成功
- [ ] WiFi 連接成功
- [ ] MQTT 連接成功
- [ ] 觸摸時序列監控器有反應
- [ ] MQTT 訂閱能收到訊息

---

## 📝 **完整測試程式（簡化版）**

```cpp
// GPIO 12 觸摸感測器快速測試
void setup() {
  Serial.begin(115200);
  pinMode(12, INPUT_PULLUP);
  Serial.println("GPIO 12 觸摸測試");
}

void loop() {
  static int lastState = HIGH;
  int currentState = digitalRead(12);
  
  if (currentState != lastState) {
    lastState = currentState;
    
    if (currentState == LOW) {
      Serial.println("✓ 觸摸！");
    } else {
      Serial.println("✓ 釋放！");
    }
  }
  
  delay(50);
}
```

---

## 🎓 **學習重點**

### **為什麼 GPIO 12 可以用？**

1. **數位讀取不受影響**
   - `digitalRead()` 只讀取 HIGH/LOW
   - 不使用 ADC（類比數位轉換器）
   - WiFi 只佔用 ADC2，不影響數位功能

2. **GPIO 12 功能分類**
   - ✅ 數位輸入（GPIO）
   - ✅ 數位輸出（GPIO）
   - ❌ 類比輸入（ADC2，WiFi 時不可用）

### **對比：光敏電阻為什麼不行？**

```
光敏電阻：
- 輸出：類比電壓（0-3.3V）
- 讀取：analogRead(4) 或 analogRead(12)
- GPIO 4/12 = ADC2
- WiFi 開啟 → ADC2 被佔用 → 讀到 0 ❌

觸摸感測器：
- 輸出：數位訊號（HIGH/LOW）
- 讀取：digitalRead(12)
- 不使用 ADC
- WiFi 開啟 → 完全不影響 ✅
```

---

## 📚 **參考資料**

- ESP32 腳位功能表
- TTP223 觸摸 IC 規格書
- GPIO 數位輸入原理

---

## ✨ **成功範例輸出**

```
=== ESP32 電容觸摸感測器 MQTT 發送器 ===
觸摸感測器腳位: GPIO 12
✅ GPIO 12 數位輸入不受 WiFi 影響

初始狀態: 未觸摸 (HIGH)

正在連接 WiFi: NetArt
固定 IP 設定成功
......
WiFi 連接成功！
IP 位址: 192.168.100.201
閘道: 192.168.100.1
訊號強度 (RSSI): -45 dBm
正在連接 MQTT Broker...
MQTT 連接成功！
設備 ID: esp32-touch-01
發布主題: sensor/touch

初始化完成，開始監測觸摸感測器...

✓ 發送成功 [sensor/touch] 狀態: 觸摸 (1)
✓ 發送成功 [sensor/touch] 狀態: 釋放 (0)
✓ 發送成功 [sensor/touch] 狀態: 觸摸 (1)
✓ 發送成功 [sensor/touch] 狀態: 釋放 (0)
```

**完美！** 🎉
