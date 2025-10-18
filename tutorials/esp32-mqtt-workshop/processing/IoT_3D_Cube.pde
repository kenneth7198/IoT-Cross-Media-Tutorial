/**
 * IoT 3D Cube - Processing MQTT 3D 互動
 * 
 * 功能：接收 MQTT 指令控制 3D 方塊旋轉、顏色、大小
 * Topic: iot/lab/+/cmd/3d
 * Payload: {"cmd":"rotate","x":1.5,"y":2.0,"z":0.5} 或 {"cmd":"color","value":"#ff0000"}
 * 
 * 使用方式：
 * 手機搖晃或 ESP32 感測器資料可控制 3D 方塊
 */

import mqtt.*;

MQTTClient client;
float rotX = 0, rotY = 0, rotZ = 0;
float targetRotX = 0, targetRotY = 0, targetRotZ = 0;
color cubeColor = color(100, 150, 255);
float cubeSize = 200;

String MQTT_HOST = "tcp://localhost:1883";
String CLIENT_ID = "processing-3d";
String TOPIC = "iot/lab/+/cmd/3d";
String TOPIC_SENSOR = "iot/lab/+/sensor/+"; // 訂閱感測器資料

void setup() {
  size(800, 600, P3D);
  
  client = new MQTTClient(this);
  client.connect(MQTT_HOST, CLIENT_ID);
  client.subscribe(TOPIC);
  client.subscribe(TOPIC_SENSOR);
  
  println("Processing 3D Cube");
}

void draw() {
  background(20);
  lights();
  
  // 漸變到目標旋轉角度
  rotX = lerp(rotX, targetRotX, 0.1);
  rotY = lerp(rotY, targetRotY, 0.1);
  rotZ = lerp(rotZ, targetRotZ, 0.1);
  
  // 3D 方塊
  pushMatrix();
  translate(width/2, height/2, 0);
  rotateX(rotX);
  rotateY(rotY);
  rotateZ(rotZ);
  
  fill(cubeColor);
  stroke(255);
  strokeWeight(2);
  box(cubeSize);
  popMatrix();
  
  // 顯示資訊
  hint(DISABLE_DEPTH_TEST);
  fill(255);
  textAlign(LEFT, TOP);
  textSize(16);
  text("Rotation: X=" + nf(degrees(rotX), 1, 1) + "° Y=" + nf(degrees(rotY), 1, 1) + "° Z=" + nf(degrees(rotZ), 1, 1) + "°", 10, 10);
  text("Size: " + nf(cubeSize, 1, 1), 10, 30);
  hint(ENABLE_DEPTH_TEST);
}

void messageReceived(String topic, byte[] payload) {
  String msg = new String(payload);
  println("MQTT: " + topic + " -> " + msg);
  
  // 3D 旋轉指令
  if (topic.indexOf("/cmd/3d") >= 0) {
    int xIdx = msg.indexOf("\"x\":");
    int yIdx = msg.indexOf("\"y\":");
    int zIdx = msg.indexOf("\"z\":");
    
    if (xIdx >= 0) {
      String xVal = extractNumber(msg, xIdx+4);
      targetRotX = float(xVal);
    }
    if (yIdx >= 0) {
      String yVal = extractNumber(msg, yIdx+4);
      targetRotY = float(yVal);
    }
    if (zIdx >= 0) {
      String zVal = extractNumber(msg, zIdx+4);
      targetRotZ = float(zVal);
    }
  }
  
  // 感測器資料（用光敏/觸控控制旋轉）
  if (topic.indexOf("/sensor/light") >= 0) {
    int valIdx = msg.indexOf("\"value\":");
    if (valIdx >= 0) {
      String val = extractNumber(msg, valIdx+8);
      float lightVal = float(val);
      targetRotY = map(lightVal, 0, 4095, 0, TWO_PI);
    }
  } else if (topic.indexOf("/sensor/touch") >= 0) {
    int valIdx = msg.indexOf("\"value\":");
    if (valIdx >= 0) {
      String val = extractNumber(msg, valIdx+8);
      if (float(val) > 0) {
        targetRotX += 0.5;
      }
    }
  }
  
  // 顏色指令
  int idx = msg.indexOf("#");
  if (idx >= 0 && msg.length() >= idx+7) {
    String hexColor = msg.substring(idx, idx+7);
    cubeColor = unhexColor(hexColor);
  }
  
  // 大小指令
  int sizeIdx = msg.indexOf("\"size\":");
  if (sizeIdx >= 0) {
    String sizeVal = extractNumber(msg, sizeIdx+7);
    cubeSize = float(sizeVal);
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
  if (hex.length() != 7 || hex.charAt(0) != '#') return color(100);
  return color(
    unhex(hex.substring(1,3)),
    unhex(hex.substring(3,5)),
    unhex(hex.substring(5,7))
  );
}

void mouseDragged() {
  targetRotY += (mouseX - pmouseX) * 0.01;
  targetRotX += (mouseY - pmouseY) * 0.01;
}
