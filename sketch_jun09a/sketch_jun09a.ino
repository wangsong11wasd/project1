#include <BH1750.h>
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi连接参数
const char* ssid = "Mryang";
const char* password = "123456789";

// MQTT服务器参数
const char* mqtt_server = "112.124.53.200";
const char* mqtt_username = "wangsong";
const char* mqtt_password = "123456";
const int mqtt_port = 1883;
const char* mqtt_topic = "test1";

// 创建BH1750对象
BH1750 lightMeter;

// 创建WiFi和MQTT客户端对象
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void setup() {
  // 初始化串口和BH1750
  Serial.begin(115200);
  Wire.begin();
  lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE);

  Serial.println(F("BH1750 One-Time Test"));
  //连接WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  //设置MQTT服务器并连接
  mqttClient.setServer(mqtt_server, mqtt_port);
  reconnect();
}

void loop() {
  // 读取BH1750传感器数据
  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  lightMeter.configure(BH1750::ONE_TIME_HIGH_RES_MODE);

  // 将传感器数据格式化为JSON
  String msg =  String(lux) ;

  // 连接MQTT服务器并发送数据
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.publish(mqtt_topic, msg.c_str());

  // 等待2s
  delay(2000);
}
//MQTT服务器重新连接
void reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Connecting to MQTT...");
    if (mqttClient.connect("ESP32-C3-client", mqtt_username, mqtt_password)) {
      Serial.println("connected");
      mqttClient.subscribe(mqtt_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}
