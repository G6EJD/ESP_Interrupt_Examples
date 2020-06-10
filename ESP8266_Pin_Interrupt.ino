const byte        interruptPin = D8; // Or other pins that support an interrupt
volatile boolean  interrupt_occurred = false; 
#define           LED_pin D1

void setup() {
  Serial.begin(115200);
  pinMode(LED_pin, OUTPUT);   // LED pin to Output mode
  digitalWrite(LED_pin, LOW); // Turn LED off (High = On, Low = Off)
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), ISR, FALLING); //Respond to falling edges on the pin
}
 
void loop() {
  if (interrupt_occurred == true) { // If an interrupt has occurred report it and turn off LED
    Serial.println("An interrupt occurred");
    delay(500);
    digitalWrite(LED_pin, LOW);     // Now turn Off LED, ready for the next interrupt
    interrupt_occurred = false;
  }
  delay(1000);
}
 
ICACHE_RAM_ATTR void ISR () {                  // Interrupt Service Routine, come here when an interrupt occurs
  digitalWrite(LED_pin, HIGH); // Interrupt occurred, so turn On LED
  interrupt_occurred = true;   // Record that an interrupt occurred
}
