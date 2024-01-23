int leftlinesensorPin = 12;
int rightlinesensorPin = 11; // Connect sensor to input pin 3
void setup() {
 Serial.begin(9600); // Init the serial port
 pinMode(leftlinesensorPin, INPUT); // declare LED as output
 pinMode(rightlinesensorPin, INPUT); // declare Micro switch as input
}
void loop(){
 int valLeft = digitalRead(leftlinesensorPin); // read left input value
 Serial.print(valLeft);
 int valRight = digitalRead(rightlinesensorPin); // read right input value
 Serial.println(valRight);
 delay(100);
}
