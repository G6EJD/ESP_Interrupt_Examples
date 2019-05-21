// toggles LED when interrupt pin changes state

int pin = 13;
DRAM_ATTR int state = LOW;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR blink()
{
  portENTER_CRITICAL(&mux);
  state = !state;
  portEXIT_CRITICAL(&mux);
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
