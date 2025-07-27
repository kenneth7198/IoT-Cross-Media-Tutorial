## 🆘 常見問題解決

### Q: 系統無法啟動？

**A:** 檢查以下項目：
1. Node.js 版本是否 16+ (`node --version`)
2. 是否執行過安裝 (`npm run setup-education`)
3. 端口 3000, 3001 是否被佔用
4. 防火牆是否阻擋了 Node.js

### Q: 瀏覽器顯示錯誤？

**A:** 常見解決方案：
1. 清除瀏覽器快取 (Ctrl+F5)
2. 檢查後端是否正常運行
3. 查看瀏覽器開發者工具的錯誤訊息
4. 重啟整個系統

### Q: 資料不見了？

**A:** 資料恢復方法：
1. 執行 `npm run reset-db` 重置為初始狀態
2. 檢查 `backend-simple/database/` 資料庫檔案
3. 重新執行安裝載入示範資料

### Q: 想要修改程式碼？

**A:** 開發建議：
1. 使用 `npm run dev` 啟動開發模式
2. 修改前端檔案會自動重新載入
3. 修改後端檔案需要重啟後端服務
4. 建議使用 VS Code 編輯器

## 🎖️ 進階學習資源

### 📚 延伸學習

1. **升級到完整版**: `../gcp/` 目錄的雲端版本
2. **真實 IoT 硬體**: Arduino、Raspberry Pi 整合
3. **進階前端**: 學習更複雜的 React 模式
4. **後端深度**: 微服務架構和部署

### 🔗 相關技術文檔

- [React 官方文檔](https://reactjs.org/docs/)
- [Express.js 指南](https://expressjs.com/zh-tw/)
- [SQLite 教學](https://www.sqlite.org/docs.html)
- [Material-UI 組件庫](https://mui.com/)

### 👥 社群支援

- 課程論壇：與同學討論技術問題
- 教師 Office Hour：面對面解決疑難
- GitHub Issues：回報系統問題
- 技術部落格：分享學習心得

---

🎓 **開始您的 IoT 學習之旅吧！**

記住：學習程式設計最重要的是動手實作。不要害怕犯錯，每個錯誤都是學習的機會。

如果遇到任何問題，隨時查看這份指南或尋求協助。祝您學習愉快！ 🚀
