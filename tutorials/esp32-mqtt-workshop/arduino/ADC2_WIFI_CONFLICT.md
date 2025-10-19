# ESP32 ADC2 與 WiFi 衝突問題說明

## 🚨 **問題原因**

### GPIO 4 讀值為 0 的根本原因：

**GPIO 4 屬於 ADC2，當 WiFi 啟動後會被 WiFi 模組佔用，導致 ADC 無法正常讀取！**

---

## 📊 **ESP32 ADC 腳位分類**

### **ADC1（推薦使用，不受 WiFi 影響）**
| GPIO | ADC 通道 | 特性 |
|------|---------|------|
| 32 | ADC1_CH4 | ✅ 隨時可用 |
| 33 | ADC1_CH5 | ✅ 隨時可用 |
| **34** | ADC1_CH6 | ✅ **推薦**（僅輸入） |
| **35** | ADC1_CH7 | ✅ **推薦**（僅輸入） |
| **36** | ADC1_CH0 | ✅ **推薦**（僅輸入） |
| **39** | ADC1_CH3 | ✅ **推薦**（僅輸入） |

### **ADC2（WiFi 使用時不可用）⚠️**
| GPIO | ADC 通道 | 特性 |
|------|---------|------|
| 0 | ADC2_CH1 | ❌ WiFi 衝突 + 啟動腳位 |
| 2 | ADC2_CH2 | ❌ WiFi 衝突 + 板載 LED |
| **4** | ADC2_CH0 | ❌ **WiFi 衝突**（你目前使用） |
| 12 | ADC2_CH5 | ❌ WiFi 衝突 |
| 13 | ADC2_CH4 | ❌ WiFi 衝突 |
| 14 | ADC2_CH6 | ❌ WiFi 衝突 |
| 15 | ADC2_CH3 | ❌ WiFi 衝突 |
| 25 | ADC2_CH8 | ❌ WiFi 衝突 |
| 26 | ADC2_CH9 | ❌ WiFi 衝突 |
| 27 | ADC2_CH7 | ❌ WiFi 衝突 |

---

## 🔧 **解決方案**

### **方案 1：改用 ADC1 腳位（強烈推薦）✅**

#### 步驟：

1. **重新接線**：將光敏電阻從 GPIO 4 改接到 GPIO 34

```
舊接法（有問題）：
3.3V ─┬─ 光敏電阻 ─┬─ GPIO 4 (ADC2) ❌
                    │
                  10kΩ
                    │
                   GND

新接法（正確）：
3.3V ─┬─ 光敏電阻 ─┬─ GPIO 34 (ADC1) ✅
                    │
                  10kΩ
                    │
                   GND
```

2. **修改程式碼**：

```cpp
// 改成 GPIO 34
const int LIGHT_SENSOR_PIN = 34;  // ADC1，不受 WiFi 影響
```

3. **上傳程式並測試**

---

### **方案 2：禁用 WiFi（不實用）❌**

僅在不需要 WiFi 時才能使用 ADC2：

```cpp
void setup() {
  analogReadResolution(12);
  
  // 讀取感測器（WiFi 關閉時）
  int value = analogRead(4);
  
  // 之後再啟動 WiFi
  WiFi.begin(SSID, PASSWORD);
}
```

**缺點**：無法同時使用 WiFi 和 ADC2 讀取

---

### **方案 3：在 WiFi 連接前讀取（臨時方案）⚠️**

```cpp
void setup() {
  // 在啟動 WiFi 前讀取
  int value = analogRead(4);  // 可以讀取
  Serial.println(value);
  
  // 啟動 WiFi
  WiFi.begin(SSID, PASSWORD);
  
  // WiFi 連接後，analogRead(4) 會失效
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  
  // 這裡再讀取會是 0
  value = analogRead(4);  // ❌ 讀不到
  Serial.println(value);  // 輸出 0
}
```

**缺點**：無法持續監測

---

## 📝 **推薦的腳位選擇**

### **最佳選擇（僅輸入腳位）：**
1. **GPIO 34** ⭐⭐⭐⭐⭐
2. **GPIO 35** ⭐⭐⭐⭐⭐
3. **GPIO 36** ⭐⭐⭐⭐⭐
4. **GPIO 39** ⭐⭐⭐⭐⭐

**優點**：
- ✅ 不受 WiFi 影響
- ✅ 僅輸入，不會誤觸發
- ✅ 沒有其他特殊功能

### **次佳選擇（可輸入輸出）：**
1. **GPIO 32** ⭐⭐⭐⭐
2. **GPIO 33** ⭐⭐⭐⭐

**優點**：
- ✅ 不受 WiFi 影響
- ✅ 可以輸入輸出

---

## 🧪 **驗證方法**

### 測試程式（驗證 ADC2 與 WiFi 衝突）：

```cpp
#include <WiFi.h>

const char* SSID = "YourWiFi";
const char* PASSWORD = "YourPassword";

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  delay(1000);
  
  Serial.println("\n=== ADC2 與 WiFi 衝突測試 ===\n");
  
  // WiFi 連接前測試
  Serial.println("【測試 1】WiFi 關閉時讀取 GPIO 4:");
  for (int i = 0; i < 5; i++) {
    int value = analogRead(4);
    Serial.print("  讀取 ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(value);
    delay(100);
  }
  
  // 連接 WiFi
  Serial.println("\n正在連接 WiFi...");
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi 已連接！");
  
  // WiFi 連接後測試
  Serial.println("\n【測試 2】WiFi 開啟後讀取 GPIO 4:");
  for (int i = 0; i < 5; i++) {
    int value = analogRead(4);
    Serial.print("  讀取 ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(value);
    if (value == 0) {
      Serial.print(" ❌ 無法讀取（被 WiFi 佔用）");
    }
    Serial.println();
    delay(100);
  }
  
  // 測試 ADC1（對照組）
  Serial.println("\n【測試 3】WiFi 開啟後讀取 GPIO 34 (ADC1):");
  for (int i = 0; i < 5; i++) {
    int value = analogRead(34);
    Serial.print("  讀取 ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(value);
    Serial.println(" ✅ 正常讀取");
    delay(100);
  }
  
  Serial.println("\n=== 結論 ===");
  Serial.println("ADC2 (GPIO 4) 在 WiFi 啟動後無法使用");
  Serial.println("ADC1 (GPIO 34) 隨時都可以正常使用");
}

void loop() {
  // 持續測試
  Serial.println("\n持續監測:");
  Serial.print("  GPIO 4 (ADC2): ");
  Serial.println(analogRead(4));
  Serial.print("  GPIO 34 (ADC1): ");
  Serial.println(analogRead(34));
  delay(2000);
}
```

### 預期結果：

```
=== ADC2 與 WiFi 衝突測試 ===

【測試 1】WiFi 關閉時讀取 GPIO 4:
  讀取 1: 2458
  讀取 2: 2461
  讀取 3: 2455
  讀取 4: 2459
  讀取 5: 2462

正在連接 WiFi...
........
WiFi 已連接！

【測試 2】WiFi 開啟後讀取 GPIO 4:
  讀取 1: 0 ❌ 無法讀取（被 WiFi 佔用）
  讀取 2: 0 ❌ 無法讀取（被 WiFi 佔用）
  讀取 3: 0 ❌ 無法讀取（被 WiFi 佔用）
  讀取 4: 0 ❌ 無法讀取（被 WiFi 佔用）
  讀取 5: 0 ❌ 無法讀取（被 WiFi 佔用）

【測試 3】WiFi 開啟後讀取 GPIO 34 (ADC1):
  讀取 1: 2458 ✅ 正常讀取
  讀取 2: 2461 ✅ 正常讀取
  讀取 3: 2455 ✅ 正常讀取
  讀取 4: 2459 ✅ 正常讀取
  讀取 5: 2462 ✅ 正常讀取

=== 結論 ===
ADC2 (GPIO 4) 在 WiFi 啟動後無法使用
ADC1 (GPIO 34) 隨時都可以正常使用
```

---

## 🎯 **快速修正指南**

### 如果你目前使用 GPIO 4：

1. ⚠️ **確認問題**：上傳測試程式，確認是 ADC2 衝突
2. 🔧 **重新接線**：改接到 GPIO 34
3. 💻 **修改程式**：`const int LIGHT_SENSOR_PIN = 34;`
4. 📤 **上傳測試**：確認讀值正常

---

## 📚 **相關文件**

- [ESP32 ADC 官方文件](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html)
- ESP32 技術參考手冊 4.11 章節

---

## ✅ **總結**

| 項目 | GPIO 4 (ADC2) | GPIO 34 (ADC1) |
|-----|--------------|----------------|
| WiFi 開啟時可用 | ❌ 不可用 | ✅ 可用 |
| 讀值穩定 | ❌ WiFi 啟動後為 0 | ✅ 穩定 |
| 推薦用於 IoT | ❌ 不推薦 | ✅ **推薦** |

**建議：立即改用 GPIO 34！**
