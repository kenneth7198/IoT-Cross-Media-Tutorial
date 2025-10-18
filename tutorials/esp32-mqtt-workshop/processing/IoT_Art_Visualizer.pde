/**
 * IoT Art Visualizer - Processing MQTT 視覺藝術
 * 
 * 功能：訂閱 MQTT color 指令，即時改變畫面顏色與視覺效果
 * Topic: iot/lab/esp32-01/cmd/color
 * Payload: {"cmd":"color","value":"#e74c3c"}
 * 
 * 安裝需求：
 * 1. Processing 4.x: https://processing.org/download
 * 2. MQTT Client for Processing: 
 *    Sketch > Import Library > Add Library > 搜尋 "MQTT" > 安裝 "mqtt" by Joel Gaehwiler
 * 
 * 使用方式：
 * 1. 修改下方 MQTT_HOST 為你的 Broker IP
 * 2. 確保 Mosquitto/Aedes 已啟動（port 1883）
 * 3. 執行此程式
 * 4. 用手機開啟 web/art.html，點選色塊
 * 5. Processing 畫面即時變色並產生視覺效果
 */

import mqtt.*;

MQTTClient client;
color currentColor = color(100, 100, 100);
color targetColor = color(100, 100, 100);
ArrayList<Particle> particles = new ArrayList<Particle>();

// ---- 設定區 ----
String MQTT_HOST = "tcp://localhost:1883";  // 改成你的 Broker IP
String CLIENT_ID = "processing-art";
String TOPIC = "iot/lab/+/cmd/color";  // 訂閱所有裝置的 color 指令

void setup() {
  size(800, 600);
  colorMode(RGB, 255);
  
  // 連線 MQTT Broker
  client = new MQTTClient(this);
  client.connect(MQTT_HOST, CLIENT_ID);
  
  // 訂閱 topic
  client.subscribe(TOPIC);
  
  println("Processing MQTT Art Visualizer");
  println("Subscribed to: " + TOPIC);
}

void draw() {
  // 漸變到目標顏色
  currentColor = lerpColor(currentColor, targetColor, 0.05);
  background(currentColor);
  
  // 標題
  fill(255);
  textAlign(CENTER, TOP);
  textSize(24);
  text("IoT Art Visualizer", width/2, 20);
  textSize(16);
  text("Color: " + hex(currentColor, 6), width/2, 60);
  
  // 繪製粒子
  for (int i = particles.size()-1; i >= 0; i--) {
    Particle p = particles.get(i);
    p.update();
    p.display();
    if (p.isDead()) particles.remove(i);
  }
  
  // 中央顏色圓
  noStroke();
  fill(currentColor);
  circle(width/2, height/2, 200);
  
  // 動態圓環
  noFill();
  strokeWeight(3);
  stroke(currentColor, 150);
  float r = 120 + 30 * sin(frameCount * 0.05);
  circle(width/2, height/2, r);
}

// MQTT 訊息接收
void messageReceived(String topic, byte[] payload) {
  String msg = new String(payload);
  println("MQTT: " + topic + " -> " + msg);
  
  // 解析顏色
  int idx = msg.indexOf("#");
  if (idx >= 0 && msg.length() >= idx+7) {
    String hexColor = msg.substring(idx, idx+7);
    targetColor = unhexColor(hexColor);
    
    // 產生粒子效果
    for (int i = 0; i < 20; i++) {
      particles.add(new Particle(width/2, height/2, targetColor));
    }
  }
}

void clientConnected() {
  println("MQTT connected!");
}

void connectionLost() {
  println("MQTT connection lost!");
}

// 手動點擊也能產生效果
void mousePressed() {
  targetColor = color(random(255), random(255), random(255));
  for (int i = 0; i < 30; i++) {
    particles.add(new Particle(mouseX, mouseY, targetColor));
  }
}

// Hex 轉 Color
color unhexColor(String hex) {
  if (hex.length() != 7 || hex.charAt(0) != '#') return color(100);
  return color(
    unhex(hex.substring(1,3)),
    unhex(hex.substring(3,5)),
    unhex(hex.substring(5,7))
  );
}

// 粒子類別
class Particle {
  float x, y;
  float vx, vy;
  color col;
  float life = 255;
  
  Particle(float x, float y, color c) {
    this.x = x;
    this.y = y;
    this.col = c;
    float angle = random(TWO_PI);
    float speed = random(2, 6);
    this.vx = cos(angle) * speed;
    this.vy = sin(angle) * speed;
  }
  
  void update() {
    x += vx;
    y += vy;
    life -= 3;
  }
  
  void display() {
    noStroke();
    fill(col, life);
    circle(x, y, 8);
  }
  
  boolean isDead() {
    return life <= 0;
  }
}
