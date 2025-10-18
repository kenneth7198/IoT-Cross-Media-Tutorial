/**
 * IoT Generative Art - Processing MQTT 生成藝術
 * 
 * 功能：接收 MQTT 感測器資料，產生演算藝術（Generative Art）
 * Topic: iot/lab/+/sensor/+ (所有感測器)
 * 
 * 藝術風格：
 * - 根據光敏感測器改變顏色主題
 * - 根據觸控感測器產生圖案
 * - 根據按鈕觸發藝術變換
 * - 自動儲存為 PNG 檔案
 * 
 * 使用方式：
 * 按下 's' 儲存當前畫面
 * 按下 'c' 清空畫布
 * 按下空白鍵切換繪圖模式
 */

import mqtt.*;

MQTTClient client;
PGraphics canvas;
int drawMode = 0; // 0=lines, 1=circles, 2=triangles, 3=bezier
color currentColor;
float lightValue = 2000;
int touchCount = 0;
ArrayList<DrawPoint> points = new ArrayList<DrawPoint>();

String MQTT_HOST = "tcp://localhost:1883";
String CLIENT_ID = "processing-genart";
String TOPIC = "iot/lab/+/sensor/+";

void setup() {
  size(800, 600);
  colorMode(HSB, 360, 100, 100);
  
  // 建立離屏畫布（保留繪圖歷史）
  canvas = createGraphics(width, height);
  canvas.beginDraw();
  canvas.background(0, 0, 95);
  canvas.endDraw();
  
  currentColor = color(180, 70, 80);
  
  // 連線 MQTT
  client = new MQTTClient(this);
  client.connect(MQTT_HOST, CLIENT_ID);
  client.subscribe(TOPIC);
  
  println("Processing Generative Art");
  println("Commands: [s]ave, [c]lear, [space] change mode");
}

void draw() {
  background(0, 0, 95);
  image(canvas, 0, 0);
  
  // 自動繪圖
  if (frameCount % 3 == 0 && points.size() >= 2) {
    drawOnCanvas();
  }
  
  // 顯示資訊
  fill(0, 0, 20);
  textAlign(LEFT, TOP);
  textSize(16);
  text("Mode: " + getModeName(), 10, 10);
  text("Light: " + nf(lightValue, 1, 0), 10, 30);
  text("Touch: " + touchCount, 10, 50);
  text("Points: " + points.size(), 10, 70);
}

String getModeName() {
  switch(drawMode) {
    case 0: return "Lines";
    case 1: return "Circles";
    case 2: return "Triangles";
    case 3: return "Bezier Curves";
    default: return "Unknown";
  }
}

void drawOnCanvas() {
  canvas.beginDraw();
  canvas.strokeWeight(2);
  canvas.stroke(currentColor, 150);
  canvas.fill(currentColor, 50);
  
  if (drawMode == 0) {
    // 連線
    DrawPoint p1 = points.get(int(random(points.size())));
    DrawPoint p2 = points.get(int(random(points.size())));
    canvas.line(p1.x, p1.y, p2.x, p2.y);
  } else if (drawMode == 1) {
    // 圓形
    DrawPoint p = points.get(int(random(points.size())));
    float size = random(10, 50);
    canvas.circle(p.x, p.y, size);
  } else if (drawMode == 2) {
    // 三角形
    if (points.size() >= 3) {
      DrawPoint p1 = points.get(int(random(points.size())));
      DrawPoint p2 = points.get(int(random(points.size())));
      DrawPoint p3 = points.get(int(random(points.size())));
      canvas.triangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
    }
  } else if (drawMode == 3) {
    // 貝茲曲線
    if (points.size() >= 4) {
      DrawPoint p1 = points.get(int(random(points.size())));
      DrawPoint p2 = points.get(int(random(points.size())));
      DrawPoint p3 = points.get(int(random(points.size())));
      DrawPoint p4 = points.get(int(random(points.size())));
      canvas.noFill();
      canvas.bezier(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y);
    }
  }
  
  canvas.endDraw();
}

void messageReceived(String topic, byte[] payload) {
  String msg = new String(payload);
  println("MQTT: " + topic);
  
  int valIdx = msg.indexOf("\"value\":");
  if (valIdx < 0) return;
  
  String val = extractNumber(msg, valIdx+8);
  float value = float(val);
  
  // 光敏感測器 → 顏色主題
  if (topic.indexOf("/sensor/light") >= 0) {
    lightValue = value;
    float hue = map(value, 0, 4095, 0, 360);
    currentColor = color(hue, 70, 80);
  }
  
  // 觸控感測器 → 新增繪圖點
  if (topic.indexOf("/sensor/touch") >= 0) {
    if (value > 0) {
      touchCount++;
      float x = random(width * 0.1, width * 0.9);
      float y = random(height * 0.1, height * 0.9);
      points.add(new DrawPoint(x, y));
      if (points.size() > 100) {
        points.remove(0);
      }
    }
  }
  
  // 按鈕 → 切換模式
  if (topic.indexOf("/cmd/button") >= 0) {
    if (msg.indexOf("\"pressed\"") >= 0) {
      drawMode = (drawMode + 1) % 4;
    }
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
  if (key == 's' || key == 'S') {
    // 儲存畫面
    String filename = "genart_" + year() + nf(month(), 2) + nf(day(), 2) + "_" + 
                      nf(hour(), 2) + nf(minute(), 2) + nf(second(), 2) + ".png";
    canvas.save(filename);
    println("Saved: " + filename);
  } else if (key == 'c' || key == 'C') {
    // 清空畫布
    canvas.beginDraw();
    canvas.background(0, 0, 95);
    canvas.endDraw();
    points.clear();
    println("Canvas cleared");
  } else if (key == ' ') {
    // 切換模式
    drawMode = (drawMode + 1) % 4;
  }
}

void mousePressed() {
  points.add(new DrawPoint(mouseX, mouseY));
  if (points.size() > 100) points.remove(0);
}

class DrawPoint {
  float x, y;
  DrawPoint(float x, float y) {
    this.x = x;
    this.y = y;
  }
}
