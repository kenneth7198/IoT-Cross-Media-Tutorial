#!/bin/bash

# ESP32 LED 控制器 - 快速啟動腳本 (macOS/Linux)
# 用途: 一鍵啟動 MQTT Broker 和 Web Server

set -e

# 顏色設定
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 印出標題
print_banner() {
    echo -e "${BLUE}"
    echo "╔════════════════════════════════════════╗"
    echo "║   ESP32 LED 控制器 - 快速啟動工具    ║"
    echo "╚════════════════════════════════════════╝"
    echo -e "${NC}"
}

# 取得本機 IP
get_local_ip() {
    if [[ "$OSTYPE" == "darwin"* ]]; then
        # macOS
        LOCAL_IP=$(ifconfig | grep "inet " | grep -v 127.0.0.1 | awk '{print $2}' | head -n 1)
    else
        # Linux
        LOCAL_IP=$(hostname -I | awk '{print $1}')
    fi
    echo "$LOCAL_IP"
}

# 檢查 Node.js
check_nodejs() {
    if ! command -v node &> /dev/null; then
        echo -e "${RED}❌ 未安裝 Node.js${NC}"
        echo "請先安裝 Node.js: https://nodejs.org/"
        exit 1
    fi
    echo -e "${GREEN}✅ Node.js 已安裝: $(node -v)${NC}"
}

# 檢查並安裝依賴
check_dependencies() {
    cd "$(dirname "$0")/../server"
    
    if [ ! -d "node_modules" ]; then
        echo -e "${YELLOW}📦 正在安裝 MQTT Broker 依賴...${NC}"
        npm install
        echo -e "${GREEN}✅ 依賴安裝完成${NC}"
    else
        echo -e "${GREEN}✅ 依賴已存在${NC}"
    fi
}

# 啟動 MQTT Broker
start_mqtt_broker() {
    echo -e "${YELLOW}🚀 正在啟動 MQTT Broker...${NC}"
    cd "$(dirname "$0")/../server"
    
    # 在背景執行
    node broker.js &
    MQTT_PID=$!
    echo $MQTT_PID > /tmp/mqtt_broker.pid
    
    sleep 2
    
    if ps -p $MQTT_PID > /dev/null; then
        echo -e "${GREEN}✅ MQTT Broker 已啟動 (PID: $MQTT_PID)${NC}"
        echo -e "   TCP: localhost:1883"
        echo -e "   WebSocket: ws://localhost:9001"
        return 0
    else
        echo -e "${RED}❌ MQTT Broker 啟動失敗${NC}"
        return 1
    fi
}

# 啟動 Web Server
start_web_server() {
    echo -e "${YELLOW}🌐 正在啟動 Web Server...${NC}"
    cd "$(dirname "$0")"
    
    # 使用 Python 3 內建的 HTTP server
    python3 -m http.server 3000 &
    WEB_PID=$!
    echo $WEB_PID > /tmp/web_server.pid
    
    sleep 2
    
    if ps -p $WEB_PID > /dev/null; then
        echo -e "${GREEN}✅ Web Server 已啟動 (PID: $WEB_PID)${NC}"
        echo -e "   Port: 3000"
        return 0
    else
        echo -e "${RED}❌ Web Server 啟動失敗${NC}"
        return 1
    fi
}

# 顯示使用資訊
show_info() {
    LOCAL_IP=$(get_local_ip)
    
    echo ""
    echo -e "${BLUE}════════════════════════════════════════${NC}"
    echo -e "${GREEN}🎉 所有服務已啟動！${NC}"
    echo -e "${BLUE}════════════════════════════════════════${NC}"
    echo ""
    echo -e "${YELLOW}📱 手機訪問網址:${NC}"
    echo -e "   ${GREEN}http://${LOCAL_IP}:3000/esp32-led-control.html${NC}"
    echo ""
    echo -e "${YELLOW}🖥️  電腦訪問網址:${NC}"
    echo -e "   ${GREEN}http://localhost:3000/esp32-led-control.html${NC}"
    echo ""
    echo -e "${YELLOW}⚙️  MQTT Broker:${NC}"
    echo -e "   TCP: localhost:1883"
    echo -e "   WebSocket: ${GREEN}ws://${LOCAL_IP}:9001${NC}"
    echo ""
    echo -e "${BLUE}════════════════════════════════════════${NC}"
    echo -e "${YELLOW}📝 下一步:${NC}"
    echo "   1. 確保 ESP32 已上傳程式並連接 WiFi"
    echo "   2. 在手機瀏覽器開啟上方網址"
    echo "   3. 設定 MQTT URL: ws://${LOCAL_IP}:9001"
    echo "   4. 點擊「連線」按鈕"
    echo "   5. 開始控制 LED！"
    echo ""
    echo -e "${RED}⚠️  停止服務: 按 Ctrl+C 或執行 ./stop-services.sh${NC}"
    echo -e "${BLUE}════════════════════════════════════════${NC}"
}

# 清理函式（當腳本結束時）
cleanup() {
    echo ""
    echo -e "${YELLOW}🛑 正在停止服務...${NC}"
    
    if [ -f /tmp/mqtt_broker.pid ]; then
        MQTT_PID=$(cat /tmp/mqtt_broker.pid)
        kill $MQTT_PID 2>/dev/null || true
        rm /tmp/mqtt_broker.pid
        echo -e "${GREEN}✅ MQTT Broker 已停止${NC}"
    fi
    
    if [ -f /tmp/web_server.pid ]; then
        WEB_PID=$(cat /tmp/web_server.pid)
        kill $WEB_PID 2>/dev/null || true
        rm /tmp/web_server.pid
        echo -e "${GREEN}✅ Web Server 已停止${NC}"
    fi
    
    echo -e "${GREEN}👋 再見！${NC}"
}

# 註冊清理函式
trap cleanup EXIT INT TERM

# 主程式
main() {
    print_banner
    check_nodejs
    check_dependencies
    
    if start_mqtt_broker && start_web_server; then
        show_info
        
        # 保持腳本運行
        echo ""
        echo -e "${YELLOW}⏳ 服務正在運行中... (按 Ctrl+C 停止)${NC}"
        
        # 無限等待直到用戶按 Ctrl+C
        while true; do
            sleep 1
        done
    else
        echo -e "${RED}❌ 服務啟動失敗${NC}"
        exit 1
    fi
}

# 執行主程式
main
