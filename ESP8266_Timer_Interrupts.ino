volatile boolean LED_state = false;
#define LED_pin   D1

void setup() {
  Serial.begin(115200);
  pinMode(D1, OUTPUT); // LED pin
  noInterrupts();                                // Switch off interrupts whilst they are set up
  timer0_isr_init();                             // Initialise Timer0
  timer0_attachInterrupt(ISR);                   // Goto the ISR function below when an interrupt occurs
  timer0_write(ESP.getCycleCount() + 80000000L); // Pre-load timer with a time value (1-second)
  interrupts();                                  // Switch interrupts back on
}

void loop() { 
  delay(100000);                                 // Do nothing, but the LED keeps flashing!
}

ICACHE_RAM_ATTR void ISR (void) {                                // Timer reached zero, now re-load it to repeat
  timer0_write(ESP.getCycleCount() + 80000000L); // Reset the timer, do this first for timing accuracy
  LED_state = !LED_state;                        // Toggle the current LED_state
  digitalWrite(LED_pin, LED_state);              // Flash the LED on or off
}
