#define RL 10
float MQ_Sensor = A0;
float VRL = 0;
float Rs = 0;
float analog = 0;
float Ro = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(MQ_Sensor, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  analog = analogRead(MQ_Sensor);
  VRL = analog*(3.3/1023.0);
  Rs = (3.3/VRL -1) * RL;
  Ro = Rs/9.6;
  Serial.print("Ro = ");
  Serial.println(Ro);
  delay(1000);
}
