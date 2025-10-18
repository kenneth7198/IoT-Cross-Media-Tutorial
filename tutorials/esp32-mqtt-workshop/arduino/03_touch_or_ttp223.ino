// 03_touch_or_ttp223.ino - 觸控/外接 TTP223
// 觸控可用 ESP32 觸控腳（如 T0=GPIO4、T5=GPIO12），或接 TTP223 到一般數位腳
const int TOUCH_PIN = 12; // T5
const int LED_PIN = 23;

void setup(){
  pinMode(LED_PIN, OUTPUT);
  pinMode(TOUCH_PIN, INPUT);
  Serial.begin(115200);
}

void loop(){
  int touchVal = digitalRead(TOUCH_PIN);
  digitalWrite(LED_PIN, touchVal ? HIGH : LOW);
  Serial.println(touchVal);
  delay(50);
}
