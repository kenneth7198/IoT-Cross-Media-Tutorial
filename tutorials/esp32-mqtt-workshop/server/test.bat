@echo off
setlocal

set HOST=localhost
set PORT=1883

REM 訂閱一個測試主題（新視窗）
start cmd /k mosquitto_sub -h %HOST% -p %PORT% -t test/hello -v

REM 發布幾則訊息
mosquitto_pub -h %HOST% -p %PORT% -t test/hello -m "hi 1"
mosquitto_pub -h %HOST% -p %PORT% -t test/hello -m "hi 2"
mosquitto_pub -h %HOST% -p %PORT% -t test/hello -m "hi 3"

echo 測試完成。按任意鍵結束。
pause >nul
