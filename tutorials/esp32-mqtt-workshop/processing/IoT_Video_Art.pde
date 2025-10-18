/**
 * IoT Video Art - Processing MQTT 影片藝術
 * 
 * 功能：接收 MQTT 指令控制影片播放、濾鏡效果
 * Topic: iot/lab/+/cmd/video
 * Payload: {"cmd":"video","action":"play|pause|reverse","filter":"tint|blur|pixelate"}
 * 
 * 安裝需求：
 * 1. Processing 4.x + Video Library
 * 2. MQTT Client for Processing
 * 3. 準備一段影片檔案（MP4/MOV）放在 data/ 資料夾
 * 
 * 使用方式：
 * 1. 在 sketch 資料夾建立 data/ 目錄
 * 2. 放入影片檔案（如 sample.mp4）
 * 3. 修改 VIDEO_FILE 變數
 * 4. 手機開啟 web/art.html 控制影片
 */

import mqtt.*;
import processing.video.*;

MQTTClient client;
Movie video;
color tintColor = color(255);
int filterMode = 0; // 0=none, 1=tint, 2=blur, 3=pixelate
boolean isReverse = false;
float playSpeed = 1.0;

// ---- 設定區 ----
String MQTT_HOST = "tcp://localhost:1883";
String CLIENT_ID = "processing-video";
String TOPIC = "iot/lab/+/cmd/video";
String VIDEO_FILE = "sample.mp4"; // 改成你的影片檔名

void setup() {
  size(800, 600);
  
  // 載入影片
  video = new Movie(this, VIDEO_FILE);
  video.loop();
  
  // 連線 MQTT
  client = new MQTTClient(this);
  client.connect(MQTT_HOST, CLIENT_ID);
  client.subscribe(TOPIC);
  
  println("Processing Video Art");
  println("Video: " + VIDEO_FILE);
}

void draw() {
  background(0);
  
  if (video.available()) {
    video.read();
  }
  
  // 套用濾鏡
  pushMatrix();
  translate(width/2, height/2);
  imageMode(CENTER);
  
  if (filterMode == 1) {
    tint(tintColor);
  } else if (filterMode == 2) {
    filter(BLUR, 2);
  } else if (filterMode == 3) {
    // Pixelate 效果
    int pixelSize = 10;
    video.loadPixels();
    for (int y = 0; y < video.height; y += pixelSize) {
      for (int x = 0; x < video.width; x += pixelSize) {
        int index = x + y * video.width;
        if (index < video.pixels.length) {
          fill(video.pixels[index]);
          noStroke();
          rect(x - video.width/2, y - video.height/2, pixelSize, pixelSize);
        }
      }
    }
    popMatrix();
    return;
  }
  
  image(video, 0, 0);
  popMatrix();
  noTint();
  
  // 顯示資訊
  fill(255);
  textAlign(LEFT, TOP);
  textSize(16);
  text("Filter: " + getFilterName(), 10, 10);
  text("Speed: " + nf(playSpeed, 1, 1) + "x", 10, 30);
  text("Time: " + nf(video.time(), 1, 1) + "s / " + nf(video.duration(), 1, 1) + "s", 10, 50);
}

String getFilterName() {
  switch(filterMode) {
    case 0: return "None";
    case 1: return "Tint";
    case 2: return "Blur";
    case 3: return "Pixelate";
    default: return "Unknown";
  }
}

void messageReceived(String topic, byte[] payload) {
  String msg = new String(payload);
  println("MQTT: " + msg);
  
  // 解析 action
  if (msg.indexOf("play") >= 0) {
    video.play();
  } else if (msg.indexOf("pause") >= 0) {
    video.pause();
  } else if (msg.indexOf("stop") >= 0) {
    video.stop();
  } else if (msg.indexOf("reverse") >= 0) {
    isReverse = !isReverse;
    playSpeed = isReverse ? -1.0 : 1.0;
    video.speed(playSpeed);
  } else if (msg.indexOf("fast") >= 0) {
    playSpeed = 2.0;
    video.speed(playSpeed);
  } else if (msg.indexOf("slow") >= 0) {
    playSpeed = 0.5;
    video.speed(playSpeed);
  }
  
  // 解析濾鏡
  if (msg.indexOf("\"filter\":\"tint\"") >= 0) {
    filterMode = 1;
  } else if (msg.indexOf("\"filter\":\"blur\"") >= 0) {
    filterMode = 2;
  } else if (msg.indexOf("\"filter\":\"pixelate\"") >= 0) {
    filterMode = 3;
  } else if (msg.indexOf("\"filter\":\"none\"") >= 0) {
    filterMode = 0;
  }
  
  // 解析顏色
  int idx = msg.indexOf("#");
  if (idx >= 0 && msg.length() >= idx+7) {
    String hexColor = msg.substring(idx, idx+7);
    tintColor = unhexColor(hexColor);
  }
}

color unhexColor(String hex) {
  if (hex.length() != 7 || hex.charAt(0) != '#') return color(255);
  return color(
    unhex(hex.substring(1,3)),
    unhex(hex.substring(3,5)),
    unhex(hex.substring(5,7))
  );
}

void keyPressed() {
  if (key == ' ') {
    if (video.isPlaying()) video.pause();
    else video.play();
  } else if (key == 'f') {
    filterMode = (filterMode + 1) % 4;
  }
}
