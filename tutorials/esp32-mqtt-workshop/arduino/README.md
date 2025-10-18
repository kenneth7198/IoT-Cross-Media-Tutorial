# ESP32 Arduino 範例（4 份）

- 01_button_led.ino：按鈕控制 LED（數位 IO）
- 02_lightsensor_adc.ino：光敏電阻 ADC 量測並序列印出
- 03_touch_or_ttp223.ino：電容觸控/外接 TTP223 感測
- 04_mqtt_basic.ino：連線到 MQTT，發布/訂閱

## custom/ 目錄（你的原始程式）

- custom_button_led.ino：微動開關控制 LED
	- LED = GPIO23，按鈕 = GPIO22
	- 測試：按下按鈕 LED 亮，序列監控顯示 1/0
- custom_light_touch.ino：光敏電阻 ADC + 電容感測
	- 光敏 = GPIO4，電容 = GPIO12，LED = GPIO23
	- 測試：序列監控顯示光敏 ADC 值，電容感測可用註解段落控制 LED

這兩份程式可直接燒錄測試，若要升級為 MQTT 版本，請參考 config.example.h 與 TEMPLATE_mqtt_device.ino。

接線圖與 Topic 設計詳見根目錄 `topics.md`。