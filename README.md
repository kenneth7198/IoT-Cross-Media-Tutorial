# 🎓 IoT Cross-Media 教學版 - 快速開始指南

## 一鍵安裝和啟動

### 1. 系統需求檢查

確認您的電腦符合以下需求：
- **Node.js**: 16.0 以上版本
- **記憶體**: 最少 2GB 可用 RAM
- **硬碟**: 500MB 可用空間
- **瀏覽器**: Chrome, Firefox, Safari, Edge
- **軟體**: Node.js 16.x 以上版本 https://nodejs.org/en/download

### 2. 快速安裝

```bash
# 進入教學版目錄，並安裝基本環境
cd IoT-Cross-Media-Tutorial
npm install fs-extra chalk ora inquirer sqlite3

# 一鍵安裝（包含所有依賴和範例）
npm run setup-education

# 一鍵啟動
npm run start-education
```

### 3. 系統將自動：

✅ 檢查系統需求  
✅ 建立專案結構  
✅ 安裝前後端依賴  
✅ 初始化 SQLite 資料庫  
✅ 載入示範資料  
✅ 啟動前後端服務  
✅ 開啟瀏覽器到 http://localhost:3000  

## 🌟 系統架構概覽

```
您的電腦
├── 🌐 前端 (React) - localhost:3000
│   ├── 儀表板 - 監控 IoT 設備狀態
│   ├── 設備管理 - 新增/編輯虛擬設備
│   ├── 資料視覺化 - 感測器資料圖表
│   ├── 媒體庫 - 圖片上傳和管理
│   └── 教學模式 - 互動式學習介面
│
├── ⚡ 後端 (Node.js + Express) - localhost:3001
│   ├── RESTful API - 資料 CRUD 操作
│   ├── WebSocket - 即時資料推送
│   ├── 檔案上傳 - 媒體檔案處理
│   ├── IoT 模擬器 - 虛擬感測器資料
│   └── 簡易認證 - 基礎用戶管理
│
└── 🗄️ SQLite 資料庫 - 本機檔案
    ├── users - 用戶資料
    ├── devices - 設備資訊  
    ├── sensor_data - 感測器歷史資料
    └── media_files - 媒體檔案記錄
```

## 📚 第一次使用導覽

### 步驟 1: 系統啟動確認

當您看到以下畫面時，表示系統啟動成功：

```
🎯 系統狀態儀表板
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
📱 前端應用: http://localhost:3000
⚡ 後端 API: http://localhost:3001  
📊 API 文檔: http://localhost:3001/api-docs
🗄️ 資料庫:   SQLite (backend-simple/database/)
```

### 步驟 2: 探索系統功能

1. **儀表板頁面**: 查看所有虛擬 IoT 設備的即時狀態
2. **設備管理**: 新增溫度、濕度、光線感測器
3. **資料圖表**: 觀察感測器資料的變化趨勢
4. **媒體上傳**: 嘗試上傳圖片到系統中

### 步驟 3: 開始學習

點選頁面右上角的 "🎓 教學模式" 按鈕，系統將引導您：

- 📖 **基礎概念**: IoT 系統運作原理
- 🔧 **動手實作**: 修改程式碼看效果
- 🧪 **實驗挑戰**: 完成指定任務
- 📊 **進度追蹤**: 學習成就解鎖

## 🛠️ 常用操作指令

```bash
# 啟動系統
npm run start-education

# 重新安裝依賴
npm run install-all

# 重置資料庫（清除所有資料）
npm run reset-db

# 執行測試
npm run test

# 停止系統
Ctrl + C
```
