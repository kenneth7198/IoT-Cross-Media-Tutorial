/**
 * IoT Particle System - Processing MQTT 粒子系統
 * 
 * 功能：接收 MQTT 指令產生不同類型的粒子效果
 * Topic: iot/lab/+/cmd/particle
 * Payload: {"cmd":"particle","type":"explosion|rain|wave|spiral","count":100}
 * 
 * 使用方式：
 * ESP32 感測器資料或手機操作可觸發不同粒子效果
 */

import mqtt.*;

MQTTClient client;
ArrayList<Particle> particles = new ArrayList<Particle>();
String particleType = "explosion";
color particleColor = color(255, 100, 50);

String MQTT_HOST = "tcp://localhost:1883";
String CLIENT_ID = "processing-particle";
String TOPIC = "iot/lab/+/cmd/particle";
String TOPIC_SENSOR = "iot/lab/+/sensor/+";

void setup() {
  size(800, 600);
  colorMode(HSB, 360, 100, 100);
  
  client = new MQTTClient(this);
  client.connect(MQTT_HOST, CLIENT_ID);
  client.subscribe(TOPIC);
  client.subscribe(TOPIC_SENSOR);
  
  println("Processing Particle System");
}

void draw() {
  background(0, 0, 10);
  
  // 更新並繪製粒子
  for (int i = particles.size()-1; i >= 0; i--) {
    Particle p = particles.get(i);
    p.update();
    p.display();
    if (p.isDead()) particles.remove(i);
  }
  
  // 顯示資訊
  fill(0, 0, 100);
  textAlign(LEFT, TOP);
  textSize(16);
  text("Particles: " + particles.size(), 10, 10);
  text("Type: " + particleType, 10, 30);
}

void messageReceived(String topic, byte[] payload) {
  String msg = new String(payload);
  println("MQTT: " + msg);
  
  // 粒子類型
  if (msg.indexOf("explosion") >= 0) {
    particleType = "explosion";
    createExplosion(width/2, height/2, 50);
  } else if (msg.indexOf("rain") >= 0) {
    particleType = "rain";
    createRain(30);
  } else if (msg.indexOf("wave") >= 0) {
    particleType = "wave";
    createWave(40);
  } else if (msg.indexOf("spiral") >= 0) {
    particleType = "spiral";
    createSpiral(60);
  }
  
  // 粒子數量
  int countIdx = msg.indexOf("\"count\":");
  if (countIdx >= 0) {
    String countVal = extractNumber(msg, countIdx+8);
    int count = int(float(countVal));
    if (particleType.equals("explosion")) {
      createExplosion(width/2, height/2, count);
    }
  }
  
  // 顏色
  int idx = msg.indexOf("#");
  if (idx >= 0 && msg.length() >= idx+7) {
    String hexColor = msg.substring(idx, idx+7);
    particleColor = unhexColor(hexColor);
  }
  
  // 感測器觸發
  if (topic.indexOf("/sensor/light") >= 0) {
    int valIdx = msg.indexOf("\"value\":");
    if (valIdx >= 0) {
      String val = extractNumber(msg, valIdx+8);
      float lightVal = float(val);
      if (lightVal < 500) { // 遮光時觸發
        createExplosion(random(width), random(height), 30);
      }
    }
  }
}

void createExplosion(float x, float y, int count) {
  for (int i = 0; i < count; i++) {
    float angle = random(TWO_PI);
    float speed = random(2, 8);
    particles.add(new Particle(x, y, cos(angle)*speed, sin(angle)*speed, particleColor));
  }
}

void createRain(int count) {
  for (int i = 0; i < count; i++) {
    particles.add(new Particle(random(width), -10, 0, random(2, 5), particleColor));
  }
}

void createWave(int count) {
  for (int i = 0; i < count; i++) {
    float x = map(i, 0, count, 0, width);
    particles.add(new Particle(x, height/2, random(-2, 2), random(-5, -2), particleColor));
  }
}

void createSpiral(int count) {
  for (int i = 0; i < count; i++) {
    float angle = map(i, 0, count, 0, TWO_PI * 3);
    float radius = map(i, 0, count, 0, 100);
    float x = width/2 + cos(angle) * radius;
    float y = height/2 + sin(angle) * radius;
    particles.add(new Particle(x, y, cos(angle)*2, sin(angle)*2, particleColor));
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

color unhexColor(String hex) {
  if (hex.length() != 7 || hex.charAt(0) != '#') return color(0, 0, 100);
  return color(
    (unhex(hex.substring(1,3)) / 255.0) * 360,
    (unhex(hex.substring(3,5)) / 255.0) * 100,
    (unhex(hex.substring(5,7)) / 255.0) * 100
  );
}

void mousePressed() {
  createExplosion(mouseX, mouseY, 50);
}

class Particle {
  float x, y, vx, vy;
  color col;
  float life = 100;
  float size = 8;
  
  Particle(float x, float y, float vx, float vy, color c) {
    this.x = x;
    this.y = y;
    this.vx = vx;
    this.vy = vy;
    this.col = c;
  }
  
  void update() {
    x += vx;
    y += vy;
    vy += 0.2; // 重力
    life -= 1;
  }
  
  void display() {
    noStroke();
    fill(col, life);
    circle(x, y, size);
  }
  
  boolean isDead() {
    return life <= 0 || y > height + 50;
  }
}
