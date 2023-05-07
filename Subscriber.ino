#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Mengatur SSID dan Password Jaringan yang di pakai
const char* ssid = "Universitas Mulawarman";
const char* password = ""; 

// server Mqtt broker
const char* mqtt_server = "broker.hivemq.com";

// Buzzer & LED pin
#define BUZZER_PIN D5
#define LED_PIN D1

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

// Fungsi koneksi wifi
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

// Fungsi untuk menerima data
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Pesan diterima [");
  Serial.print(topic);
  Serial.print("] ");
  String data = ""; 
  for (int i = 0; i < length; i++) {
    data += (char)payload[i];
  }
  int Api = data.toInt(); 
  if ( Api == 1) { 
    Serial.println("APIII LARI!!!");
  } else {
    Serial.println("Tidak Ada Api");
  }
  if ( Api == 1) {
    tone(BUZZER_PIN,1000,2000);
    digitalWrite(LED_PIN, HIGH); 
  } else {
    noTone(BUZZER_PIN);
    digitalWrite(LED_PIN, LOW); 
  }
}

// fungsi untuk mengubungkan ke broker
void reconnect() {
  while (!client.connected()) {
    Serial.println("Mencoba untuk menghubungkan dengan MQTT...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("Terhubung");
      client.subscribe("iot_unmul/iot_a_2");
    } else {
      Serial.println("Gagal Terhubung, rc=");
      Serial.print(client.state());
      Serial.println(" Mencoba dalam 5 detik");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);  // Inisialisasi pin BUZZER
  pinMode(LED_PIN, OUTPUT);     // Inisialisasi pin LED
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback); 
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}