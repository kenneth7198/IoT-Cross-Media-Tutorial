// custom_light_touch.ino
// 功能：光敏電阻 ADC 量測 + 電容感測（TTP223），序列監控顯示光敏值
// 腳位：光敏 = GPIO4，電容 = GPIO12，LED = GPIO23
// 測試：序列監控顯示光敏 ADC 值，電容感測可用註解段落控制 LED

const int touchPin = 12;  // 電容感測用
const int adcPin = 4;     // 光敏電阻用
const int ledPin = 23;

void setup() {
  pinMode(touchPin, INPUT);
  pinMode(adcPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);
}

void loop() {
  int touchVal = digitalRead(touchPin);
  int adcVal = analogRead(adcPin);
  Serial.println(adcVal);
  // 若要用電容感測控制 LED，取消下方註解
  // if(touchVal == HIGH){
  //   digitalWrite(ledPin, HIGH);
  // }else{
  //   digitalWrite(ledPin, LOW);
  // }
  delay(100);
}
