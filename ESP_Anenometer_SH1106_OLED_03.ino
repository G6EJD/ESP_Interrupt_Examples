/* Using an ESP8266 to constuct an anemometer using a low cost anemometer head unit. It measures the time of one rotation, from
   which it determins wind speed. The actual reading is determined empirically and adjusted for aerodynamic drag effects. The
   values displayed by the unit match those of a Davis Instruments 6152 Weather Station.
   See: https://www.amazon.co.uk/MISOL-Spare-weather-station-speed/dp/B00QDMBQGG
   
   This software, the ideas and concepts is Copyright (c) David Bird 2019 and beyond. All rights to this software are reserved.
   It is prohibited to redistribute or reproduce of any part or all of the software contents in any form other than the following:
     1. You may print or download to a local hard disk extracts for your personal and non-commercial use only. 
     2. You may copy the content to individual third parties for their personal use, but only if you acknowledge the author David Bird as the source of the material. 
     3. You may not, except with my express written permission, distribute or commercially exploit the content.
     4. You may not transmit it or store it in any other website or other form of electronic retrieval system for commercial purposes. 
     5. You MUST include all of this copyright and permission notice ('as annotated') and this shall be included in all copies or
        substantial portions of the software and where the software use is visible to an end-user.
  THE SOFTWARE IS PROVIDED "AS IS" FOR PRIVATE USE ONLY, IT IS NOT FOR COMMERCIAL USE IN WHOLE OR PART OR CONCEPT.
  FOR PERSONAL USE IT IS SUPPLIED WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  
  IN NO EVENT SHALL THE AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#define SDA D3 // On ESP8266 D1 Mini
#define SCL D4 // On ESP8266 D1 Mini

#include "SH1106.h"               // See https://github.com/squix78/esp8266-oled-ssd1306 or via Sketch/Include Library/Manage Libraries
SH1106 display(0x3c, SDA, SCL);   // OLED display object definition (address, SDA, SCL)

const byte   interruptPin = D8; // Or other pins that support an interrupt
unsigned int Debounce_Timer, Current_Event_Time, Last_Event_Time, Event_Counter;
float        WindSpeed;

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA, SCL);                          // Start the Wire service for the OLED display using assigned pins for SDA and SCL
  display.init();                                // Initialise the display
  display.flipScreenVertically();                // Flip the screen around by 180°
  display.setContrast(128);                      // Set display contrast, 255 is maximum and 0 minimum. 128 is an average value
  display.setFont(ArialMT_Plain_16);             // Set the Font size
  pinMode(interruptPin, INPUT_PULLUP);
  noInterrupts();                                // Disable interrupts during set-up
  attachInterrupt(digitalPinToInterrupt(interruptPin), WSpeed_ISR, RISING); //Respond to a LOW on the interrupt pin and goto WSpeed_ISR
  timer0_isr_init();                             // Initialise Timer-0
  timer0_attachInterrupt(Timer_ISR);             // Goto the Timer_ISR function when an interrupt occurs
  timer0_write(ESP.getCycleCount() + 80000000L); // Pre-load Timer-0 with a time value of 1-second
  interrupts();                                  // Enable interrupts
}

void loop() {
  display.clear();
  display.drawString(50, 30, String(WindSpeed, 1) + " MPH");
  display.display();
  delay(500);
}

//#########################################################################################
// Interrupt service routine
void WSpeed_ISR (void) {
  if (!(millis() - Debounce_Timer) < 5) {
    Debounce_Timer = millis();                                        // Set debouncer to prevent false triggering
    Event_Counter++;
  }
}

void Timer_ISR (void) {                                                       // Timer reached zero, now re-load it to repeat
  timer0_write(ESP.getCycleCount() + 80000000L);                              // Reset the timer, do this first for timing accuracy
  WindSpeed = Event_Counter*2.5/2;
  Serial.println(Event_Counter);
  Event_Counter =0;
}
