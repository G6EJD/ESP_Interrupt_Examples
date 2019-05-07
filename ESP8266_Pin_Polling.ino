const byte inputPin = D8; // Or other pins that support an input
#define    LED_pin D1

void setup() {
  Serial.begin(115200);
  pinMode(inputPin, INPUT);   // InputPin set to be input-mode
  pinMode(LED_pin, OUTPUT);   // LED pin set to output-mode
  digitalWrite(LED_pin, LOW); // Turn off LED (Low = Off, High = On)
}
 
void loop() {
  if (digitalRead(inputPin) == HIGH) {   // test to see in the inputPin is HIGH?
    Serial.println("An event occurred"); // Input pin went HIGH
    digitalWrite(LED_pin, HIGH);         // Switch on LED to signify event occurred
    delay(500);
    digitalWrite(LED_pin, LOW);          // Switch off LED to signify event is complete
  }
  delay(1000); // Do nothing to simulate doing other work, perhaps updating a screen
}
