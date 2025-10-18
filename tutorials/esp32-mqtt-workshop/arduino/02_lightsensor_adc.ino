// 02_lightsensor_adc.ino - 光敏電阻 ADC（ESP32）
// 將光敏電阻與 10k 電阻做分壓接到 ADC 腳（例如 GPIO 34）
const int ADC_PIN = 34;

void setup(){
  Serial.begin(115200);
}

void loop(){
  int v = analogRead(ADC_PIN); // 0..4095
  Serial.println(v);
  delay(100);
}
