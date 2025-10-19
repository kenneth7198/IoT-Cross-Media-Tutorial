# ESP32 + MQTT + Processing 工作坊完整教材

**🎯 4 小時從零開始建立 IoT 視覺互動系統**

這是一個完整的 ESP32 + MQTT + Processing 視覺互動工作坊教材，涵蓋硬體、軟體、網頁、視覺藝術四個面向。

---

## 📖 教材文件

### 👨‍🏫 講師用
- **[TEACHING_PLAN.md](./TEACHING_PLAN.md)** - 完整教學計畫（240 分鐘詳細流程）⭐
- **[TEACHING_QUICKREF.md](./TEACHING_QUICKREF.md)** - 快速參考卡（隨時查閱）
- **[CHECKLIST.md](./CHECKLIST.md)** - 課前準備清單
- **[SUMMARY.md](./SUMMARY.md)** - 教材總覽

### 👨‍🎓 學員用
- **[STUDENT_GUIDE.md](./STUDENT_GUIDE.md)** - 學員操作手冊（完整步驟）⭐
- **[QUICKSTART.md](./QUICKSTART.md)** - 15 分鐘快速開始
- **[install.md](./install.md)** - 軟體安裝指南
- **[QA.md](./QA.md)** - 常見問題

### 📚 技術文件
- **[materials.md](./materials.md)** - 課程大綱與時間規劃
- **[topics.md](./topics.md)** - MQTT Topic 設計說明
- **[assets/wiring.md](./assets/wiring.md)** - 接線圖與電路說明
- **[assets/architecture.md](./assets/architecture.md)** - 系統架構圖

---

## 🎯 學習目標

完成工作坊後，學員將能夠：
- ✅ 理解 IoT 架構與 MQTT 協定
- ✅ 掌握 ESP32 開發（GPIO、ADC、WiFi、MQTT）
- ✅ 實作手機網頁控制（含搖晃偵測）
- ✅ 整合 Processing 視覺藝術
- ✅ 創作互動藝術作品

---

## 📋 課程大綱（4 小時）

| 時段 | 時長 | 主題 | 檢查點 |
|------|------|------|--------|
| Part 1 | 60min | IoT 基礎 + 環境設定 | LED 閃爍 ✅ |
| Part 2 | 60min | ESP32 基礎互動 | 感測器正常 ✅ |
| 休息 | 15min | 中場休息 ☕ | - |
| Part 3 | 60min | MQTT 整合 + 手機控制 | 手機控制 LED ✅ |
| Part 4 | 45min | Processing 視覺藝術 | 視覺效果出現 ✅ |

**詳細教學流程**：[TEACHING_PLAN.md](./TEACHING_PLAN.md)

---

## 🚀 快速開始

### 講師（課前準備）

1. **啟動 MQTT Broker**
   ```powershell
   cd server
   npm install
   node broker.js
   ```

2. **啟動 Web Server**
   ```powershell
   cd web
   .\serve.ps1
   ```

3. **參考教學計畫**
   - 詳細流程：[TEACHING_PLAN.md](./TEACHING_PLAN.md)
   - 快速查閱：[TEACHING_QUICKREF.md](./TEACHING_QUICKREF.md)

### 學員（跟著操作）

1. **安裝軟體** - 參考：[install.md](./install.md)
2. **硬體測試** - 參考：[STUDENT_GUIDE.md](./STUDENT_GUIDE.md)
3. **開始實驗** - 參考：[QUICKSTART.md](./QUICKSTART.md)

---

## 🎨 教材內容
- 講義與大綱：`materials.md`
- Topic 與訊息格式：`topics.md`
- ESP32 程式碼：`arduino/`
- 手機網頁前端：`web/`
- Processing 視覺藝術：`processing/` 🎨
- 本地測試工具（Node + MQTT）：`server/`
- 投影片：`slides/iot-mqtt-workshop.marp.md`

---

## 🎨 藝術互動挑戰（IoT Art）

### 完整互動鏈

手機 → MQTT → ESP32 → Processing 視覺藝術

### 操作流程

1. **手機端**：開啟 `web/art.html`，點選畫作色塊
2. **ESP32 端**：燒錄 `arduino/custom/art_color_led_mqtt.ino`，RGB LED 變色
3. **Processing 端**：執行 `processing/IoT_Art_Visualizer.pde`，畫面即時變色並產生粒子效果

### Topic 與格式

- Topic: `iot/lab/esp32-01/cmd/color`
- Payload: `{"cmd":"color","value":"#e74c3c"}`

### 進階玩法

- 多人協作：同時控制不同視覺元素
- 顏色接力：每人依序撈取顏色完成拼圖
- 自訂畫布：學生設計自己的互動藝術作品

## 先決條件
- Windows 10/11
- 安裝 Arduino IDE 2.x 與 ESP32 Board
- 有網路（同一個 Wi-Fi）
- 1~4 台 ESP32 開發板 + LED、按鈕、光敏電阻、TTP223（或可用板載觸控/ADC 替代）

更多背景說明可參考倉庫根目錄的 `MQTT整合說明.md`。
