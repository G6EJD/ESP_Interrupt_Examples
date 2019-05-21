// toggles LED when interrupt pin changes state

int pin = 13;
volatile int state = LOW;

void ICACHE_RAM_ATTR blink()
{
  state = !state;
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
