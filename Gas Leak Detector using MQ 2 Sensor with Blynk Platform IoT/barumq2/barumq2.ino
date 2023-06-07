#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

double RL = 10000;
#define MQ_Sensor A0
#define Led D3
#define Buzzer D4
#define indi_servo D5
#define VPIN_button V0

float VRL;
float Rs = 0;
float Ro = 0;
float ratio = 0;
float ppm = 0;

int buttonState = LOW;
int pushbutton = HIGH;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "HP9_wHTwCdF3rls9mo4Nm-qGO2hO-vis";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "iPhone";
char pass[] = "12345678";

BLYNK_CONNECTED() {

  // Request the latest state from the server

  Blynk.syncVirtual(VPIN_button);

  // Alternatively, you could override server state using:
 // Blynk.virtualWrite(VPIN_button, pushbutton);

}

BLYNK_WRITE(VPIN_button) {
  buttonState = param.asInt();
  digitalWrite(indi_servo, buttonState);
}

float kalibrasiADC(int adc){
  float VRL=(4.98*adc)/1023;
  return (VRL);
}

void kalibrasiMQ2(){
  int i;
  float value=0;
  float res = 0;
  for(i=0;i<50;i++){
    value += kalibrasiADC(analogRead(MQ_Sensor));
    delay(100);
  }
  value = value/50;
  res = 4.98-value;
  res = (res/value)*RL;
}
void kadargas(){
  int value = kalibrasiADC(analogRead(MQ_Sensor));
  Rs = 4.98-value;
  Rs = (Rs/value)*RL;
  Ro = Rs;
  ratio = Rs/Ro;
  if(ratio>1){
    ratio = 1;
  }
  delay(100);
  ppm = ratio/18.446;
  ppm = pow(ppm, -2.375);
  ppm = ppm-1015.12;
  Serial.print("Rs/Ro = ");
  Serial.println(ratio);
  Serial.print("Nilai LPG(ppm) : ");
  Serial.println(ppm);
  
  if(ppm > 500){
    digitalWrite(Led, HIGH);
    digitalWrite(Buzzer, HIGH);
    Blynk.virtualWrite(V1, 255);
    Blynk.notify("GAS BOCOR !! ");
    // Update Button Widget
        if (digitalRead(indi_servo) == LOW) {
    // pushButton2State is used to avoid sequential toggles
          if (pushbutton != LOW) {
      
            // Toggle Relay state
            buttonState = !buttonState;
            digitalWrite(indi_servo, buttonState);
            digitalWrite(Led, LOW);
            digitalWrite(Buzzer, LOW); 
      
            // Update Button Widget
            Blynk.virtualWrite(VPIN_button, buttonState);  
          }
          pushbutton = LOW; 
        }else{
          pushbutton = HIGH;
        }
        delay(1000);
        Blynk.virtualWrite(V1, 0);
  }else{
    digitalWrite(Led, LOW);
    digitalWrite(Buzzer, LOW);
    Blynk.virtualWrite(V2, 255);
    delay(1000);
    Blynk.virtualWrite(V2, 0);  
  }
  
}

void setup(){
//Set serial monitor pada baut 9600
  Blynk.begin(auth, ssid, pass);
  Blynk.config(auth);
  pinMode(Led, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  
  pinMode(indi_servo, OUTPUT);
  pinMode(pushbutton, INPUT_PULLUP);
  digitalWrite(indi_servo, buttonState);
  Serial.begin(9600);
}



void loop(){
  kalibrasiMQ2();
  kadargas();
  Blynk.run();
  delay(500);
}
