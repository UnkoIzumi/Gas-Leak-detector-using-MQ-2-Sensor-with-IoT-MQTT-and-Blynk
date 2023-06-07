#include <ESP8266WiFi.h>
#include <Servo.h>
#include <PubSubClient.h>

#define CCW   1000
#define STOP 1500
#define RL 10
#define m -0.44953
#define b 1.23257
#define Ledgreen D3
#define Buzzer D4
#define Ledred D5

Servo myservo;
float MQ_Sensor = A0;
float VRL = 0;
float Rs = 0;
float Ro = 27.0;
float bersih = 9.6;
float Ratio = 0;
float ppm = 0;
int indi = 0;

int buttonState = LOW;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "KelinciPercobaan";
char pass[] = "qwerty12";

//char ssid[] = "Vector3";
//char pass[] = "Ecourmc2";

#define mqttserver "broker.hivemq.com" //mqtt server di android
#define mqttport 1883 // dengan port 1883

WiFiClient espClient2;
PubSubClient client (espClient2);

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
  Serial.println();
}

void reconnected(){
  while(!client.connected()){
    Serial.println("Menghubungkan kembali ke MQTT..");
    if(client.connect("clientid)")){
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
  VRL = analogRead(MQ_Sensor)*(3.3/1023.0);
  Rs = ((3.3*RL)/VRL)-RL;
  Ratio = Rs/Ro;
  ppm = pow(10, ((log10(Ratio)-b)/m));

  Serial.print("Nilai LPG(ppm) : ");
  Serial.println(ppm);
  
  if(ppm >= 300){
    while(buttonState == 0){
            digitalWrite(Ledred, HIGH);
            digitalWrite(Buzzer, HIGH);
            digitalWrite(Ledgreen, LOW);
            indi = 1;
            delay(1000);
        }
    myservo.writeMicroseconds(CCW);
    delay(1400);
    myservo.writeMicroseconds(STOP);
delay(1000);
   digitalWrite(Ledred, LOW);
   digitalWrite(Buzzer, LOW);
   digitalWrite(Ledgreen, HIGH);
   indi = 0; 
  }else{
    digitalWrite(Ledgreen, HIGH);
    digitalWrite(Ledred, LOW);
    digitalWrite(Buzzer, LOW);
    indi = 0;
   }
  dtostrf(ppm, 2, 0, mq);
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
