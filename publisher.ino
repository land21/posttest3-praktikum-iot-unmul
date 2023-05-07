#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Mengatur SSID dan Password Jaringan yang di pakai
const char* ssid = "Universitas Mulawarman";
const char* password = "";

// server Mqtt broker
const char* mqtt_server = "broker.hivemq.com";

// Sensor Api
#define FS_PIN D1

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int FS = 0;
int FSold = 0;
int value = 0;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


// fungsi untuk menghubungkan ke broker
void reconnect() {
  while (!client.connected()) {
    Serial.print("Mencoba untuk menghubungkan dengan MQTT\n");
    String clientId = "ESP8266Client- ";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("Terhubung");
      client.subscribe("iotunmulunik");
    } else {
      Serial.print("Gagal Terhubung, rc=");
      Serial.print(client.state());
      Serial.println("Mencoba dalam 5 detik");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(FS_PIN,INPUT);
  pinMode(BUILTIN_LED, OUTPUT); 
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  int FS = digitalRead(FS_PIN);
  if (FS == FSold){
    FS = FSold;
  } else {
    FSold = FS;
    delay(2000);
    snprintf (msg, MSG_BUFFER_SIZE, "%s", itoa(FS,msg,10));
    Serial.print("Pesan : ");
    Serial.println(msg);
    client.publish("iot_unmul/iot_a_2", msg); }
}
