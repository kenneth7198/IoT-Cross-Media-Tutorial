# Processing 視覺化範例

這個資料夾包含多種 Processing 視覺藝術範例，用於展示 MQTT + IoT 的視覺互動效果。

## 檔案說明

| 檔案 | 功能 | 難度 | 需求 |
|------|------|------|------|
| `IoT_Art_Visualizer.pde` | 即時色彩視覺化 | ⭐ | MQTT Library |
| `IoT_Video_Art.pde` | 影片藝術效果 | ⭐⭐ | MQTT + Video Library |
| `IoT_3D_Cube.pde` | 3D 方塊互動 | ⭐⭐ | MQTT Library |
| `IoT_Particle_System.pde` | 粒子系統特效 | ⭐⭐ | MQTT Library |
| `IoT_Sound_Visualizer.pde` | 音樂視覺化 | ⭐⭐⭐ | MQTT + Sound Library |
| `IoT_Generative_Art.pde` | 生成藝術 | ⭐⭐⭐ | MQTT Library |

## 安裝需求

### 1. Processing 4.x
- 下載：https://processing.org/download
- 安裝完成後開啟 Processing IDE

### 2. 必要 Libraries
```
Tools > Add Tool > Libraries > 搜尋並安裝：
- MQTT (by Joel Gaehwiler) ← 必要
- Video (內建) ← 影片範例需要
- Sound (內建) ← 音樂範例需要
```

### 3. 影片檔案（影片範例需要）
在 Processing sketch 資料夾建立 `data/` 目錄，放入影片檔案：
```
IoT_Video_Art/
  IoT_Video_Art.pde
  data/
    sample.mp4  ← 你的影片檔案
```

## 使用方式

1. 開啟 Processing，載入 `.pde` 檔案
2. 確認 MQTT broker 已啟動（localhost:1883）
3. 修改程式中的 `MQTT_HOST` 變數（如果需要）
4. 點擊執行按鈕 ▶️

## 範例詳解

### 1️⃣ IoT_Art_Visualizer.pde（色彩視覺化）

**功能**：接收 MQTT 顏色指令，產生動態視覺效果

**MQTT 設定**：
- Topic: `iot/lab/+/cmd/color`
- Payload: `{"cmd":"color","value":"#ff0000"}`

**視覺效果**：
- 漸變背景色
- 中央發光圓形
- 脈衝光環動畫
- 粒子爆炸效果

**控制方式**：
- 網頁 `art.html` 點擊色塊
- ESP32 RGB LED 同步顯示

---

### 2️⃣ IoT_Video_Art.pde（影片藝術）

**功能**：MQTT 控制影片播放與濾鏡效果

**MQTT 設定**：
- Topic: `iot/lab/+/cmd/video`
- Payload: `{"cmd":"video","action":"play","filter":"tint"}`

**支援指令**：
- `play` / `pause` / `stop` - 播放控制
- `reverse` - 倒轉播放
- `fast` / `slow` - 速度控制
- `filter`: `none` / `tint` / `blur` / `pixelate` - 濾鏡

**鍵盤操作**：
- 空白鍵：暫停/播放
- `f` 鍵：切換濾鏡

---

### 3️⃣ IoT_3D_Cube.pde（3D 互動）

**功能**：ESP32 感測器控制 3D 方塊旋轉

**MQTT 設定**：
- Topic: `iot/lab/+/cmd/3d`
- Payload: `{"cmd":"rotate","x":1.5,"y":2.0,"z":0.5}`
- 感測器 Topic: `iot/lab/+/sensor/light` / `touch`

**互動方式**：
- 光敏感測器 → 控制 Y 軸旋轉
- 觸控感測器 → 觸發 X 軸旋轉
- 顏色指令 → 改變方塊顏色
- 滑鼠拖曳 → 手動旋轉

---

### 4️⃣ IoT_Particle_System.pde（粒子系統）

**功能**：不同感測器觸發不同粒子效果

**MQTT 設定**：
- Topic: `iot/lab/+/cmd/particle`
- Payload: `{"cmd":"particle","type":"explosion","count":100}`

**粒子類型**：
- `explosion` - 爆炸效果
- `rain` - 雨滴效果
- `wave` - 波浪效果
- `spiral` - 螺旋效果

**觸發方式**：
- ESP32 光敏感測器遮光 → 爆炸
- 手機點擊螢幕 → 選擇粒子類型
- 滑鼠點擊 → 產生粒子

---

### 5️⃣ IoT_Sound_Visualizer.pde（音樂視覺化）

**功能**：聲音強度驅動視覺效果

**MQTT 設定**：
- Topic: `iot/lab/+/sensor/sound`
- Payload: `{"deviceId":"esp32-01","type":"sound","value":512}`

**視覺元素**：
- 頻譜條（64 條）
- 中央波動圓形
- 聲音粒子擴散

**硬體需求**：
- ESP32 + 麥克風/聲音感測器（如 MAX4466）

**鍵盤操作**：
- 空白鍵：暫停/播放音樂（如果載入了音樂檔案）

---

### 6️⃣ IoT_Generative_Art.pde（生成藝術）

**功能**：感測器資料驅動演算藝術生成

**MQTT 設定**：
- Topic: `iot/lab/+/sensor/+` (所有感測器)

**繪圖模式**：
- Lines - 連線藝術
- Circles - 圓形藝術
- Triangles - 三角形藝術
- Bezier Curves - 貝茲曲線藝術

**互動方式**：
- 光敏感測器 → 改變顏色主題
- 觸控感測器 → 新增繪圖點
- 按鈕 → 切換繪圖模式

**鍵盤操作**：
- `s` 鍵：儲存畫面為 PNG
- `c` 鍵：清空畫布
- 空白鍵：切換繪圖模式
- 滑鼠點擊：新增繪圖點

---

## 整合測試

### 快速啟動（3 步驟）

1️⃣ **啟動 MQTT Broker**
```powershell
cd server
node broker.js
```

2️⃣ **執行 Processing 程式**
- 開啟 Processing IDE
- 載入任一 `.pde` 檔案
- 點擊執行 ▶️

3️⃣ **上傳 ESP32 程式 + 開啟手機控制**
```powershell
# 上傳 Arduino 程式到 ESP32
# 然後啟動網頁伺服器
cd web
.\serve.ps1
```

手機開啟：`http://<你的電腦IP>:3000/art.html`

---

## 課程教學建議

### 初階（1 小時）
1. `IoT_Art_Visualizer.pde` - 色彩視覺化
2. 配合 `art.html` 手機控制
3. ESP32 RGB LED 同步

### 進階（1.5 小時）
1. `IoT_3D_Cube.pde` - 3D 互動
2. `IoT_Particle_System.pde` - 粒子系統
3. ESP32 感測器整合

### 挑戰（2 小時）
1. `IoT_Video_Art.pde` - 影片藝術
2. `IoT_Sound_Visualizer.pde` - 音樂視覺化
3. `IoT_Generative_Art.pde` - 生成藝術
4. 學員自由創作組合

---

## 開發提示

### Processing 基本語法
```java
void setup() {
  size(800, 600);     // 設定視窗大小
  colorMode(HSB);     // HSB 色彩模式
}

void draw() {
  background(0);      // 清空畫面
  // 每幀執行的繪圖程式碼
}
```

### MQTT 回調函式
```java
void messageReceived(String topic, byte[] payload) {
  String msg = new String(payload);
  println("收到訊息: " + msg);
  // 處理 MQTT 訊息
}
```

### 除錯技巧
- `println()` - 印出除錯訊息
- `Ctrl+T` - 自動排版程式碼
- 查看 Console 視窗的錯誤訊息

---

## 故障排除

### ❌ 問題：MQTT 連線失敗

**解決方式**：
1. 確認 MQTT broker 正在運行
2. 檢查 `MQTT_HOST` 設定（預設 `tcp://localhost:1883`）
3. 測試連線：
   ```powershell
   cd server
   .\pubsub.ps1 -BrokerHost localhost -Topic test/topic -Message hello
   ```

---

### ❌ 問題：沒有視覺反應

**解決方式**：
1. 確認已訂閱正確的 Topic
2. 檢查 Processing Console 是否有 "MQTT: ..." 訊息
3. 使用 PowerShell 手動發送測試：
   ```powershell
   cd server
   .\pubsub.ps1 -BrokerHost localhost -Topic "iot/lab/esp32-01/cmd/color" -Message '{"cmd":"color","value":"#ff0000"}'
   ```

---

### ❌ 問題：影片無法播放

**解決方式**：
1. 確認已安裝 Video Library
2. 檢查影片檔案路徑：`data/sample.mp4`
3. 支援格式：MP4, MOV, AVI
4. 建議解析度：1280x720 以下

---

### ❌ 問題：效能不佳/卡頓

**解決方式**：
1. 減少粒子數量
2. 降低視窗解析度：`size(640, 480)`
3. 減少繪圖複雜度
4. 關閉不必要的視覺效果

---

## 延伸學習

- **Processing 官方教學**：https://processing.org/tutorials
- **MQTT 協定**：https://mqtt.org/
- **生成藝術**：https://www.generativehut.com/
- **OpenProcessing 範例**：https://openprocessing.org/

---

## 創意挑戰

💡 **挑戰 1**：結合多個感測器創作互動藝術作品  
💡 **挑戰 2**：將作品輸出為影片或 GIF  
💡 **挑戰 3**：使用 Processing 控制真實世界的燈光裝置  
💡 **挑戰 4**：開發自己的視覺效果演算法
