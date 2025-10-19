# ESP32 GPIO 4 完整功能說明

## 📊 **GPIO 4 功能對照表**

| 功能 | 指令 | WiFi 關閉時 | WiFi 開啟時 | 說明 |
|------|------|-----------|-----------|------|
| **數位輸入** | `digitalRead(4)` | ✅ 可用 | ✅ **可用** | 不受影響 |
| **數位輸出** | `digitalWrite(4, HIGH/LOW)` | ✅ 可用 | ✅ **可用** | 不受影響 |
| **類比輸入** | `analogRead(4)` | ✅ 可用 | ❌ **不可用** | WiFi 佔用 ADC2 |

---

## ✅ **GPIO 4 可以當作 digitalRead - 完全沒問題！**

### **重點說明：**

1. ✅ **數位功能不受影響**
   - `digitalRead(4)` 在 WiFi 開啟時完全正常
   - `digitalWrite(4)` 在 WiFi 開啟時完全正常

2. ❌ **類比功能受影響**
   - `analogRead(4)` 在 WiFi 開啟時會讀到 0
   - 這就是您光敏電阻的問題

---

## 🎯 **您的情況分析**

### **問題診斷：**

```cpp
// 您目前的使用方式（光敏電阻）
const int LIGHT_SENSOR_PIN = 4;
int lightValue = analogRead(4);  // ❌ WiFi 開啟時 = 0

// 原因：光敏電阻需要類比讀取
```

### **解決方案選擇：**

#### **方案 1：改用 ADC1 腳位（推薦）✅**
```cpp
// 光敏電阻改接到 GPIO 34
const int LIGHT_SENSOR_PIN = 34;  // ADC1，不受 WiFi 影響
int lightValue = analogRead(34);  // ✅ 正常工作
```

#### **方案 2：GPIO 4 改用數位感測器** ✅
```cpp
// 改用數位感測器（如按鈕、PIR）
const int BUTTON_PIN = 4;
int buttonState = digitalRead(4);  // ✅ 正常工作

// 或觸摸感測器
const int TOUCH_PIN = 4;
int touched = digitalRead(4);  // ✅ 正常工作
```

---

## 📋 **適用於 GPIO 4 的感測器類型**

### ✅ **可以使用（數位感測器）：**

| 感測器類型 | 讀取方式 | WiFi 時可用 |
|----------|---------|-----------|
| 按鈕 | digitalRead | ✅ 可用 |
| 開關 | digitalRead | ✅ 可用 |
| 人體紅外線 (PIR) | digitalRead | ✅ 可用 |
| 觸摸感測器（數位型）| digitalRead | ✅ 可用 |
| 門磁感測器 | digitalRead | ✅ 可用 |
| 聲音感測器（數位型）| digitalRead | ✅ 可用 |
| 震動感測器（數位型）| digitalRead | ✅ 可用 |

### ❌ **不能使用（類比感測器）：**

| 感測器類型 | 讀取方式 | WiFi 時可用 |
|----------|---------|-----------|
| **光敏電阻** | analogRead | ❌ **不可用** |
| 電位器 | analogRead | ❌ 不可用 |
| 溫度感測器（類比型）| analogRead | ❌ 不可用 |
| 濕度感測器（類比型）| analogRead | ❌ 不可用 |
| 距離感測器（類比型）| analogRead | ❌ 不可用 |
| 麥克風（類比型）| analogRead | ❌ 不可用 |

---

## 🧪 **快速驗證方法**

### **測試 1：數位功能測試**

```cpp
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  pinMode(4, INPUT_PULLUP);
  
  // WiFi 連接前
  Serial.println("WiFi 連接前:");
  Serial.print("digitalRead(4) = ");
  Serial.println(digitalRead(4));  // 應該顯示 1 (HIGH)
  
  // 連接 WiFi
  WiFi.begin("SSID", "PASSWORD");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  // WiFi 連接後
  Serial.println("WiFi 連接後:");
  Serial.print("digitalRead(4) = ");
  Serial.println(digitalRead(4));  // ✅ 應該還是顯示 1 (HIGH)
}

void loop() {
  Serial.print("GPIO 4 狀態: ");
  Serial.println(digitalRead(4) ? "HIGH" : "LOW");
  delay(1000);
}
```

**預期結果：** ✅ 兩次都能正常讀取

---

### **測試 2：類比功能測試**

```cpp
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  
  // WiFi 連接前
  Serial.println("WiFi 連接前:");
  Serial.print("analogRead(4) = ");
  Serial.println(analogRead(4));  // 應該有數值
  
  // 連接 WiFi
  WiFi.begin("SSID", "PASSWORD");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  // WiFi 連接後
  Serial.println("WiFi 連接後:");
  Serial.print("analogRead(4) = ");
  Serial.println(analogRead(4));  // ❌ 會是 0
}

void loop() {
  Serial.print("GPIO 4 類比值: ");
  Serial.println(analogRead(4));  // ❌ 持續為 0
  delay(1000);
}
```

**預期結果：** ❌ WiFi 開啟後讀到 0

---

## 💡 **實際建議**

### **給您的具體建議：**

#### 如果您的硬體是光敏電阻：
```
目前接法：
3.3V ─ 光敏電阻 ─ GPIO 4 ─ 10kΩ ─ GND
                    ↓
              analogRead(4) ❌

改接到 GPIO 34：
3.3V ─ 光敏電阻 ─ GPIO 34 ─ 10kΩ ─ GND
                     ↓
               analogRead(34) ✅
```

**修改程式碼：**
```cpp
// 只需改一行
const int LIGHT_SENSOR_PIN = 34;  // 從 4 改成 34
```

---

#### 如果您想繼續用 GPIO 4：

改用數位感測器，例如：

**按鈕：**
```
按鈕 ─ GPIO 4
  │
 GND

pinMode(4, INPUT_PULLUP);
int pressed = !digitalRead(4);  // 按下 = true
```

**PIR 人體感測器：**
```
PIR VCC ─ 5V 或 3.3V
PIR GND ─ GND
PIR OUT ─ GPIO 4

pinMode(4, INPUT);
int detected = digitalRead(4);  // 偵測到 = HIGH
```

---

## 📚 **總結**

| 項目 | GPIO 4 數位功能 | GPIO 4 類比功能 |
|-----|---------------|---------------|
| WiFi 開啟時可用 | ✅ **可以** | ❌ **不能** |
| 適用感測器 | 按鈕、開關、PIR | 光敏電阻、電位器 |
| 您的情況 | - | **目前使用類比** |
| 建議 | 可保持 GPIO 4 | **改用 GPIO 34** |

---

## ✅ **結論**

**回答您的問題：GPIO 4 可以當作 digitalRead 嗎？**

**答案：✅ 完全可以！數位功能不受 WiFi 影響。**

**但是：** 您的光敏電阻需要 `analogRead()`，這才是問題所在。

**最佳解決方案：** 將光敏電阻從 GPIO 4 改接到 GPIO 34。
