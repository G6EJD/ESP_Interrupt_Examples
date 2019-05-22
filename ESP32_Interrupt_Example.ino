// toggles LED when interrupt pin changes state

int pin = 13;
volatile int state = LOW;

portMUX_TYPE synch = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR blink()
{
  portENTER_CRITICAL(&synch);
  state = !state;
  portEXIT_CRITICAL(&synch);
}

void setup()
{
  pinMode(pin, OUTPUT);
  attachInterrupt(0, blink, CHANGE);
}

void loop()
{
  digitalWrite(pin, state);
}
