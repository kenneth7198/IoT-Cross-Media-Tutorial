// custom_button_led.ino
// 功能：微動開關控制 LED，序列監控顯示按鈕狀態
// 腳位：LED = GPIO23，按鈕 = GPIO22
// 測試：按下按鈕 LED 亮，放開 LED 滅，序列監控顯示 1/0

const int ledPin = 23;
const int inputButton = 22;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(inputButton, INPUT);
  Serial.begin(9600);
}

void loop() {
  int val = digitalRead(inputButton);
  digitalWrite(ledPin, val ? HIGH : LOW);
  Serial.println(val);
  delay(100);
}
