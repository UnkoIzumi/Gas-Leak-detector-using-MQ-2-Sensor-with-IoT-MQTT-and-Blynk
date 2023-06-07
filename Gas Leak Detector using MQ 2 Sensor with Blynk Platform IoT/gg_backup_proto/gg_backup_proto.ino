#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

int TRIGGER = 7;           // membuat varibel trig yang di set ke-pin 3
int ECHO = 6;           // membuat variabel echo yang di set ke-pin 2
long durasi, distance;     // membuat variabel durasi dan jarak

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "as8qY_JLfNuDO0fuLSnWUlcTxUv_dbo4";
//"as8qY_JLfNuDO0fuLSnWUlcTxUv_dbo4";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Andromax";
char pass[] = "passwordapa";
void setup() {
  
  Serial.begin (9600);
  Blynk.begin(auth, ssid, pass);
  Blynk.config(auth);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);
}

void loop() {

  // program dibawah ini agar trigger memancarakan suara ultrasonic
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(8);
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(8);
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(8);

  durasi = pulseIn(ECHO, HIGH); // menerima suara ultrasonic
  distance = (durasi / 2) / 29.1;  // mengubah durasi menjadi jarak (cm)
  int satuan = (distance * 100) / 15;
 
   if (satuan <= 33) {
    Blynk.virtualWrite(V2, 255);
    delay(1000);
    Blynk.virtualWrite(V2, 0);
}

   else if (satuan <= 66) {
    Blynk.virtualWrite(V1, 255);
    delay(1000);
    Blynk.virtualWrite(V1, 0);
}
   
  else if (satuan >= 80) {
    Blynk.virtualWrite(V0, 255);
    delay(1000);
    Blynk.virtualWrite(V0, 0);
}
  
  Serial.print("Centimeter: ");
  Serial.println(distance);
  Blynk.virtualWrite(V9, satuan);
  delay(200);
  Blynk.run();
}
