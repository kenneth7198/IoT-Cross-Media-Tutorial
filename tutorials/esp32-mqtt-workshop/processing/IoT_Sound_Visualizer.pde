/**
 * IoT Sound Visualizer - Processing MQTT 音樂視覺化
 * 
 * 功能：接收 MQTT 聲音強度資料，產生音樂視覺化效果
 * Topic: iot/lab/+/sensor/sound
 * Payload: {"deviceId":"esp32-01","type":"sound","value":512,"unit":"analog"}
 * 
 * 安裝需求：
 * Processing Sound library (Tools > Add Tool > Libraries > Sound)
 * 
 * 使用方式：
 * 1. ESP32 連接麥克風或聲音感測器
 * 2. 透過 MQTT 發送聲音強度值
 * 3. Processing 即時產生視覺效果
 * 
 * 也可以載入音樂檔案測試（放在 data/ 資料夾）
 */

import mqtt.*;
import processing.sound.*;

MQTTClient client;
SoundFile soundFile; // 可選：載入音樂檔案測試
Amplitude amp;

float currentVolume = 0;
float targetVolume = 0;
ArrayList<Circle> circles = new ArrayList<Circle>();
int barCount = 64;
float[] barHeights = new float[barCount];

String MQTT_HOST = "tcp://localhost:1883";
String CLIENT_ID = "processing-sound";
String TOPIC = "iot/lab/+/sensor/sound";

// 可選：音樂檔案（放在 data/ 資料夾）
String SOUND_FILE = ""; // 例如 "music.mp3"

void setup() {
  size(800, 600);
  colorMode(HSB, 360, 100, 100);
  
  // 載入音樂檔案（可選）
  if (SOUND_FILE.length() > 0) {
    soundFile = new SoundFile(this, SOUND_FILE);
    soundFile.loop();
    amp = new Amplitude(this);
    amp.input(soundFile);
  }
  
  // 連線 MQTT
  client = new MQTTClient(this);
  client.connect(MQTT_HOST, CLIENT_ID);
  client.subscribe(TOPIC);
  
  println("Processing Sound Visualizer");
}

void draw() {
  background(0, 0, 5);
  
  // 從音樂檔案或 MQTT 取得音量
  if (soundFile != null) {
    targetVolume = amp.analyze() * 500;
  }
  currentVolume = lerp(currentVolume, targetVolume, 0.2);
  
  // 頻譜條
  drawBars();
  
  // 中央圓形波動
  drawCenterWave();
  
  // 粒子效果
  if (currentVolume > 50) {
    circles.add(new Circle(width/2, height/2, currentVolume));
  }
  for (int i = circles.size()-1; i >= 0; i--) {
    Circle c = circles.get(i);
    c.update();
    c.display();
    if (c.isDead()) circles.remove(i);
  }
  
  // 顯示資訊
  fill(0, 0, 100);
  textAlign(LEFT, TOP);
  textSize(16);
  text("Volume: " + nf(currentVolume, 1, 1), 10, 10);
  text("Particles: " + circles.size(), 10, 30);
}

void drawBars() {
  float barWidth = width / float(barCount);
  
  for (int i = 0; i < barCount; i++) {
    // 更新高度
    barHeights[i] = lerp(barHeights[i], currentVolume + random(-20, 20), 0.3);
    
    float h = constrain(barHeights[i], 0, height/2);
    float hue = map(i, 0, barCount, 200, 300);
    
    fill(hue, 80, 90);
    noStroke();
    rect(i * barWidth, height - h, barWidth-2, h);
  }
}

void drawCenterWave() {
  float radius = 100 + currentVolume;
  float points = 50;
  
  noFill();
  strokeWeight(3);
  
  for (int layer = 0; layer < 3; layer++) {
    float offset = layer * 20;
    float hue = (frameCount + layer * 60) % 360;
    stroke(hue, 80, 90);
    
    beginShape();
    for (int i = 0; i <= points; i++) {
      float angle = map(i, 0, points, 0, TWO_PI);
      float r = radius + sin(angle * 4 + frameCount * 0.05) * 20 + offset;
      float x = width/2 + cos(angle) * r;
      float y = height/2 + sin(angle) * r;
      vertex(x, y);
    }
    endShape(CLOSE);
  }
}

void messageReceived(String topic, byte[] payload) {
  String msg = new String(payload);
  
  // 解析聲音強度
  int valIdx = msg.indexOf("\"value\":");
  if (valIdx >= 0) {
    String val = extractNumber(msg, valIdx+8);
    float soundVal = float(val);
    // ESP32 ADC: 0-4095 轉換為視覺強度 0-300
    targetVolume = map(soundVal, 0, 4095, 0, 300);
  }
}

String extractNumber(String str, int startIdx) {
  String num = "";
  for (int i = startIdx; i < str.length(); i++) {
    char c = str.charAt(i);
    if (Character.isDigit(c) || c == '.' || c == '-') {
      num += c;
    } else if (num.length() > 0) {
      break;
    }
  }
  return num;
}

void keyPressed() {
  if (key == ' ' && soundFile != null) {
    if (soundFile.isPlaying()) soundFile.pause();
    else soundFile.play();
  }
}

class Circle {
  float x, y, size;
  float alpha = 100;
  float hue;
  
  Circle(float x, float y, float s) {
    this.x = x;
    this.y = y;
    this.size = s;
    this.hue = random(360);
  }
  
  void update() {
    size += 3;
    alpha -= 2;
  }
  
  void display() {
    noFill();
    strokeWeight(2);
    stroke(hue, 80, 90, alpha);
    circle(x, y, size);
  }
  
  boolean isDead() {
    return alpha <= 0;
  }
}
