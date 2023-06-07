#include <ESP8266WiFi.h>
#include <PubSubClient.h> // esp sebagai publish dan client sebaga subscribe
 
const char* ssid = "Vector3";      // Nama AP/Hotspot
const char* password = "Ecourmc2";    // Password AP/Hotspot

#define mqttserver "broker.hivemq.com" //mqtt server di android
#define mqttport 1883 // dengan port 1883

int t;
int led;

WiFiClient espClient; //membuaat wifi client
PubSubClient client(espClient); // publish client dengan wifi client esp


void setup() {
  t = 10;
  led = 1;

  Serial.begin(9600);
  delay(10);
 // Mengatur WiFi ----------------------------------------------------------
  Serial.println();
  Serial.println("Configuring Wifi...");
  //WiFi.mode(WIFI_STA);
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
 // Start the server -------------------------------------------------------
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Sukses Wifi Terkoneksi ke jaringan");
  client.setServer(mqttserver, mqttport);
 // Print the IP address ---------------------------------------------------
  Serial.println(WiFi.localIP());
 }
 
void reconnected(){
  while(!client.connected()){
    Serial.println("Menghubungkan kembali ke MQTT..");
    if(client.connect("client_sister)")){
      Serial.println("Sukses terhubung Kembali..");
    }else{
      Serial.print("Gagal,..");
      Serial.println("tunggu 3 detik..");
      delay(3000);
    }
  }
}
 
void loop() {
  char data[4];
  char lele[2];
  t = t+5;
  led = 1;
  if (t == 100){
    t = 10;
    led = 0;
  }
  Serial.println(t);
  Serial.println(led);
  dtostrf(t, 2, 0, data);// fungsi konversi int ke char
  dtostrf(led, 1, 0, lele);
  client.publish("kontroller1/date", data);// memberikan alamat topic
  client.publish("kontroller1/lede", lele);
  if(!client.connected()){ // jika client tidak konek
    reconnected(); // maka reconnect
  }
  delay(500);
}
