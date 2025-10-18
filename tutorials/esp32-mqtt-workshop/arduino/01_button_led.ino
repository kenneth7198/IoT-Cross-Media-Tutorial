// 01_button_led.ino - 按鈕控制 LED（ESP32）
const int LED_PIN = 23;   // 依實際接腳修改，或用內建 LED 2
const int BTN_PIN = 22;

void setup(){
  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT);
  Serial.begin(115200);
}

void loop(){
  int v = digitalRead(BTN_PIN);
  digitalWrite(LED_PIN, v ? HIGH : LOW);
  Serial.println(v);
  delay(50);
}
