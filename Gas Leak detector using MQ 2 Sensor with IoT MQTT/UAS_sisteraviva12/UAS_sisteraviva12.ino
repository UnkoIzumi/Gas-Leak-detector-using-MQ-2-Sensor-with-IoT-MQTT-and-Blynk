#include <ESP8266WiFi.h>
#include <Servo.h>
#include <PubSubClient.h>
#define MQTT_VERSION MQTT_VERSION_3_1_1

#define CCW   1000
#define STOP 1500
#define Ledgreen D3
#define Buzzer D4
#define Ledred D5

Servo myservo;
const int MQ_Sensor = A0;
int sensorThres = 400;
int indi = 0;

int buttonState;

// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "KelinciPercobaan";
//char pass[] = "qwerty12";

char ssid[] = "AKUR";
char pass[] = "AYUDIFA0705";

#define mqttserver "broker.hivemq.com" //mqtt server di android
#define mqttport 1883 // dengan port 1883

WiFiClient espClient2; // client
PubSubClient client (espClient2); //client

//pembacaan data masuk dri mqtt
void callback(char* topic, byte* payload, unsigned int length){
  for(int i = 0; 1 < length; i++){
    Serial.print((char)payload[i]);
  }

  Serial.println();
  if((char)payload[0] == '1'){
    buttonState = 1; // tombol on
  }else if((char)payload[0] == '0'){
     buttonState = 0; // tombol off
  }
  Serial.println(buttonState);
}

void reconnected(){
  while(!client.connected()){
    Serial.println("Menghubungkan kembali ke MQTT..");
    if(client.connect("arduinoClient", "fahmi", "12345678")){
      Serial.println("Sukses terhubung Kembali..");
    }else{
      Serial.print("Gagal,..");
      Serial.print(client.state());
      Serial.println("tunggu 3 detik..");
      delay(3000);
    }
  }
  client.subscribe("kontroler/data");
}

void mqq2(){
  char mq[4];
  char leddd[2];
  int ppm = analogRead(MQ_Sensor);

  Serial.print("Nilai Output : ");
  Serial.println(ppm);
  if(ppm > sensorThres){
    indi = 1;
    digitalWrite(Ledred, HIGH);
    digitalWrite(Buzzer, HIGH);
    digitalWrite(Ledgreen, LOW);
    if(buttonState == HIGH){
    myservo.writeMicroseconds(CCW);
    delay(1400);
    myservo.writeMicroseconds(STOP);
    delay(1000);
    digitalWrite(Ledred, LOW);
    digitalWrite(Buzzer, LOW);
    digitalWrite(Ledgreen, HIGH);
    indi = 0; 
    }
  }else{
    digitalWrite(Ledgreen, HIGH);
    digitalWrite(Ledred, LOW);
    digitalWrite(Buzzer, LOW);
    indi = 0;
   }
  dtostrf(ppm, 2, 1, mq);
  dtostrf(indi, 1, 0, leddd);
  client.publish("kontroler/led", leddd);
  client.publish("kontroler/mq", mq);
}

void setup(){
//Set serial monitor pada baut 9600
  pinMode(MQ_Sensor, INPUT);
  pinMode(Ledgreen, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Ledred, OUTPUT);
  myservo.attach(4);
  myservo.writeMicroseconds(STOP);
  Serial.begin(9600);
  
  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connect Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    yield();
  }
  client.setServer(mqttserver, mqttport);
  client.connect("arduinoClient", "fahmi", "12345678");
  client.setCallback(callback);
  Serial.println("Wifi Connected");
  Serial.print("IP address : ");
  Serial.println(WiFi.localIP());
}

void loop(){
  mqq2();
  
 if(!client.connected()){
  reconnected();
 }
 client.loop();
 delay(1000);
}
