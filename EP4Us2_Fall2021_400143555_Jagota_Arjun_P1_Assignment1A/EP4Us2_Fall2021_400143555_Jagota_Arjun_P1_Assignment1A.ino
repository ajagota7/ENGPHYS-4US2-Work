

int t = 2000;
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(14, OUTPUT);
  pinMode(12,OUTPUT);

 for (int tt =0; tt<3; tt++){
    digitalWrite(12, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(t);                       // wait for a second
    digitalWrite(12, LOW);    // turn the LED off by making the voltage LOW
    delay(t);

    digitalWrite(14, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(t);                       // wait for a second
    digitalWrite(14, LOW);    // turn the LED off by making the voltage LOW
    delay(t);

    digitalWrite(12, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(14, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(t);                       // wait for a second
    digitalWrite(12, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(14, LOW);    // turn the LED off by making the voltage LOW
    delay(t);

    t = t/2;
  } 
}

// the loop function runs over and over again forever
void loop() {

}
