#include <ESP8266WiFi.h>
#include <PubSubClient.h> // esp sebagai publish dan client sebaga subscribe
 
const char* ssid = "Vector3";      // Nama AP/Hotspot
const char* password = "Ecourmc2";    // Password AP/Hotspot

#define mqttserver "broker.hivemq.com" //mqtt server di android
#define mqttport 1883 // dengan port 1883
int c = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void callback1(char* topic, byte* payload, unsigned int length){
  Serial.print("Message received in topic");
  Serial.print(topic);
  Serial.println(length);

  Serial.println("Message received from Broker : ");
  for(int i = 0; 1 < length; i++){
    Serial.print((char)payload[i]);
  }

  Serial.println();
  Serial.println("not state");
  if(((char)payload[0] == '1') && (c == 1)){
    Serial.println("LED 2 Nyala");
  }else if(((char)payload[0] == '0') && (c == 0)){
    Serial.println("LED 1 Nyala");
  }else if((char)payload[0] == '5'){
    c = 1;
  }else if((char)payload[0] == '6'){
    c =0;
  }
  Serial.println();
}

void reconnected(){
  while(!client.connected()){
    Serial.println("Menghubungkan kembali ke MQTT..");
    if(client.connect("client_sister)")){
      Serial.println("Sukses terhubung Kembali..");
    }else{
      Serial.print("Gagal,..");
      Serial.print(client.state());
      Serial.println("tunggu 3 detik..");
      delay(3000);
    }
  }
  client.subscribe("kontroller1/data");
  c =1;
}

void setup() {
 Serial.begin(9600);
  delay(10);
 // Mengatur WiFi ----------------------------------------------------------
  Serial.println();
  Serial.println("Connecting to Wifi...");
  WiFi.mode(WIFI_STA);
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
 // Start the server -------------------------------------------------------
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
    yield();
  }
  Serial.println("Connected");
  client.setServer(mqttserver, mqttport);
  client.setCallback(callback1);
 // Print the IP address ---------------------------------------------------
  Serial.println(WiFi.localIP());  
}

void loop() {
 //Serial.println("Esp as Subcribe");
 //Serial.println("print topic :");
 if(!client.connected()){
  reconnected();
 }
 client.loop();
 delay(1000);
}
