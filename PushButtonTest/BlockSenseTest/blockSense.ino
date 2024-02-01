#define MAX_RANG (520)//the max measurement value of the module is 520cm(a little bit longer
#define ADC_SOLUTION (1023.0)//ADC accuracy of Arduino UNO is 10bit
int sensityPin = A1; // select the input pin
int led1 = 8;
float initDistance = 20;
float distanceToGrab = 15;
float distanceChange;
void setup() {
 // Serial init
 Serial.begin(9600);
 pinMode(led1,OUTPUT);
}
float dist_t, sensity_t;
void loop() {
sensity_t = analogRead(sensityPin);
dist_t = sensity_t * MAX_RANG / ADC_SOLUTION;//
distanceChange = initDistance - dist_t;
if (distanceChange <= distanceToGrab){
  digitalWrite(led1,HIGH);
}
else{
  digitalWrite(led1,LOW);
}

Serial.print(dist_t,0);
Serial.println("cm");
delay(500);
}
