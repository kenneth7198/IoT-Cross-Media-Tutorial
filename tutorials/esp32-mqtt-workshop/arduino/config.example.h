#pragma once

// 將此檔案複製為 config.h 並填入你的 Wi-Fi 與 Broker 設定

// WiFi 設定
#define WIFI_SSID      "NetArt"
#define WIFI_PASS      "1qaz2wsx"

// 固定 IP 設定（手動設定 IP）
#define USE_STATIC_IP  true              // true=使用固定IP, false=使用DHCP
#define STATIC_IP      "192.168.100.201"
#define SUBNET_MASK    "255.255.255.0"
#define GATEWAY        "192.168.100.1"
#define DNS_SERVER     "8.8.8.8"

// MQTT Broker 設定
#define MQTT_HOST      "192.168.100.1"   // 改成你的 Broker IP
#define MQTT_PORT      1883
#define DEVICE_ID      "esp32-01"        // 每個裝置要不同！
