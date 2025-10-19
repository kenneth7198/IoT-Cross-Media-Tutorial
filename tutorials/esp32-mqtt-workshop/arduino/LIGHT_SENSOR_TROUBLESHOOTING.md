# 光敏電阻讀值為 0 的故障排除指南

## 🔍 問題診斷

如果光敏電阻一直讀到 0，請按照以下步驟檢查：

---

## 1️⃣ **檢查硬體連接**

### 正確的光敏電阻電路接法：

```
3.3V ----+---- 光敏電阻 ----+---- GPIO 34 (ADC)
                             |
                           10kΩ
                             |
                            GND
```

### 接線說明：
- **光敏電阻一端** → 連接到 **3.3V**
- **光敏電阻另一端** → 連接到 **GPIO 34** 和 **10kΩ 電阻**
- **10kΩ 電阻** → 連接 GPIO 34 與 **GND** 之間

### ⚠️ 常見錯誤：
❌ 光敏電阻沒有連接到 3.3V
❌ 缺少 10kΩ 下拉電阻
❌ GPIO 34 沒有連接到分壓點
❌ 接線鬆脫或接觸不良

---

## 2️⃣ **檢查 GPIO 腳位**

### ESP32 可用的 ADC 腳位：

**ADC1（推薦使用，不會與 WiFi 衝突）：**
- GPIO 32, 33, 34, 35, 36, 39

**ADC2（WiFi 開啟時不可用）：**
- GPIO 0, 2, 4, 12, 13, 14, 15, 25, 26, 27

### ✅ 建議使用：
- **GPIO 34** (預設)
- **GPIO 35**
- **GPIO 36**
- **GPIO 39**

### 修改腳位方法：
在程式碼中修改：
```cpp
const int LIGHT_SENSOR_PIN = 34;  // 改成你實際使用的腳位
```

---

## 3️⃣ **使用測試程式驗證**

### 簡單測試程式（不需要 MQTT）：

```cpp
// 光敏電阻測試程式
void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  
  Serial.println("光敏電阻測試程式");
}

void loop() {
  int raw = analogRead(34);
  float voltage = (raw / 4095.0) * 3.3;
  
  Serial.print("GPIO 34 | 原始值: ");
  Serial.print(raw);
  Serial.print(" | 電壓: ");
  Serial.print(voltage, 2);
  Serial.println("V");
  
  delay(500);
}
```

### 預期結果：
- **明亮環境**：數值應該在 **2000-4000** 之間
- **昏暗環境**：數值應該在 **500-2000** 之間
- **遮住光敏電阻**：數值應該在 **0-500** 之間

### ⚠️ 如果一直是 0：
- 表示 GPIO 34 沒有收到電壓訊號
- 檢查硬體連接

### ⚠️ 如果一直是 4095：
- 表示 GPIO 34 一直是 3.3V
- 可能缺少下拉電阻或光敏電阻沒有連接

---

## 4️⃣ **檢查麵包板和跳線**

### 常見問題：
1. **麵包板接觸不良**
   - 試著重新插拔元件
   - 換不同的孔位

2. **跳線損壞或鬆脫**
   - 用三用電表測試導通性
   - 更換跳線

3. **光敏電阻損壞**
   - 用三用電表測試電阻值
   - 明亮時應該是幾 kΩ
   - 遮住時應該是幾十 kΩ 到幾 MΩ

---

## 5️⃣ **軟體設定檢查**

### 確認 ADC 設定：

```cpp
// 在 setup() 中加入
analogReadResolution(12);      // 12-bit (0-4095)
analogSetAttenuation(ADC_11db); // 0-3.3V 全範圍
```

### 不同的衰減設定：

| 衰減設定 | 讀取電壓範圍 | 適用場景 |
|---------|------------|---------|
| `ADC_0db` | 0 - 1.1V | 低電壓感測 |
| `ADC_2_5db` | 0 - 1.5V | - |
| `ADC_6db` | 0 - 2.2V | - |
| `ADC_11db` | 0 - 3.3V | **推薦（光敏電阻）** |

---

## 6️⃣ **環境光線檢查**

### 測試步驟：
1. **正常室內光線**：讀值應該 1500-3000
2. **用手電筒照射**：讀值應該接近 4000
3. **完全遮住**：讀值應該降到 500 以下

### ⚠️ 如果沒有變化：
- 表示硬體連接有問題

---

## 7️⃣ **完整診斷程式**

```cpp
// 完整的光敏電阻診斷程式
#include <WiFi.h>

const int LIGHT_PIN = 34;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n=== ESP32 光敏電阻診斷程式 ===\n");
  
  // ADC 設定
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  
  // 測試多個 ADC 腳位
  Serial.println("測試所有 ADC1 腳位：");
  testADC(32);
  testADC(33);
  testADC(34);
  testADC(35);
  testADC(36);
  testADC(39);
  
  Serial.println("\n開始連續監測 GPIO 34...\n");
}

void loop() {
  int value = analogRead(LIGHT_PIN);
  float voltage = (value / 4095.0) * 3.3;
  
  Serial.print("時間: ");
  Serial.print(millis() / 1000);
  Serial.print("s | 數值: ");
  Serial.print(value);
  Serial.print(" | 電壓: ");
  Serial.print(voltage, 2);
  Serial.print("V | 狀態: ");
  
  if (value == 0) {
    Serial.println("❌ 無訊號（檢查連接）");
  } else if (value < 500) {
    Serial.println("🌑 很暗");
  } else if (value < 1500) {
    Serial.println("🌘 昏暗");
  } else if (value < 2500) {
    Serial.println("🌤 明亮");
  } else {
    Serial.println("☀️ 很亮");
  }
  
  delay(1000);
}

void testADC(int pin) {
  int value = analogRead(pin);
  Serial.print("  GPIO ");
  Serial.print(pin);
  Serial.print(": ");
  Serial.print(value);
  Serial.print(" (");
  Serial.print((value / 4095.0) * 3.3, 2);
  Serial.println("V)");
}
```

---

## 8️⃣ **快速檢查清單**

- [ ] 光敏電阻正確連接到 3.3V
- [ ] 10kΩ 電阻連接 GPIO 34 到 GND
- [ ] GPIO 34 連接到光敏電阻和電阻的交點
- [ ] 所有接線牢固，沒有鬆脫
- [ ] 使用 ADC1 的腳位（32-39）
- [ ] 程式中設定了 `analogSetAttenuation(ADC_11db)`
- [ ] 光敏電阻沒有損壞
- [ ] 環境有足夠光線
- [ ] 序列監控器波特率設定為 115200

---

## 9️⃣ **替代方案：使用其他腳位**

如果 GPIO 34 有問題，可以試試其他腳位：

```cpp
// 方案 A：使用 GPIO 35
const int LIGHT_SENSOR_PIN = 35;

// 方案 B：使用 GPIO 36
const int LIGHT_SENSOR_PIN = 36;

// 方案 C：使用 GPIO 39
const int LIGHT_SENSOR_PIN = 39;
```

---

## 🛠️ **需要的工具**

1. **三用電表**：測試電阻值和導通性
2. **手電筒**：測試光敏電阻反應
3. **備用跳線**：替換可能損壞的線材
4. **備用光敏電阻**：測試元件是否損壞

---

## 📞 **仍然無法解決？**

提供以下資訊協助診斷：

1. 序列監控器的完整輸出
2. 硬體連接的照片
3. 使用的 ESP32 開發板型號
4. 光敏電阻型號和規格
5. 是否使用麵包板或直接焊接

---

## ✅ **成功案例**

正常工作時，序列監控器應該顯示：

```
=== ESP32 光敏電阻 MQTT 發送器 ===
測試光敏電阻 (GPIO 34): 2458
正在連接 WiFi: NetArt
固定 IP 設定成功
...
WiFi 連接成功！
IP 位址: 192.168.100.201
...
MQTT 連接成功！
初始化完成，開始發送光敏電阻數據...

✓ 發送成功 [sensor/light] 光敏值: 2458 (1.98V)
✓ 發送成功 [sensor/light] 光敏值: 2512 (2.02V)
✓ 發送成功 [sensor/light] 光敏值: 2389 (1.92V)
```

數值會隨著環境光線變化！
