#include <Arduino.h>

// RGB LED pin (works on any pin for built-in RGB on ESP32-S3)
#define RGB_LED_PIN 48

const int MAIN_BTN = 5;
bool changeMainBtnCurrent = false;
bool mainBtnPrev = LOW;
bool mainBtnCurrent = LOW;
const int DATA_LED = 7;

typedef void (*ButtonPressHandler) (void);

void setup() {
 
  Serial.begin(115200);
  Serial.println("🕺 ESP32-S3 DISCO MODE ACTIVATED! 🪩");
  Serial.println("=================================");
  
  randomSeed(analogRead(0)); // Seed random number generator
  setRGB(0, 0, 0); // Start with LED off
  
  Serial.println("Get ready to party! 🎉");
   // the debounce wasn't working properly because
   // I had MAIN_BTN set as INPUT but not INPUT_PULLDOWN
   // INPUT_PULLDOWN makes debounce work perfectly 
   pinMode(MAIN_BTN, INPUT_PULLDOWN);
   pinMode(DATA_LED, OUTPUT);
  delay(1000);

}
int counter = 0;
bool flashIsOn = true;
void loop() {
  
//  if (flashIsOn){
//    policeFlash();
//  }
//  if (counter > 5){ flashIsOn = false;}
//  counter++;

  checkButton(MAIN_BTN,mainBtnPrev,mainBtnCurrent,checkChangeMainButton);
  
}

void checkChangeMainButton(void){
  if (mainBtnPrev == LOW && mainBtnCurrent == HIGH){
    flashIsOn = !flashIsOn;
  }

  mainBtnPrev = mainBtnCurrent;
  if (flashIsOn){
    analogWrite(DATA_LED, 255);
    //policeFlash();    
    
    
  }
  else{
    analogWrite(DATA_LED, 0);
    
  }
  
  
}

void setRGB(uint8_t red, uint8_t green, uint8_t blue) {
  neopixelWrite(RGB_LED_PIN, red, green, blue);
}

void policeFlash() {
  // Red/Blue alternating
  Serial.println("RED is On.");
  for(int i = 0; i < 1; i++) {
  setRGB(255, 0, 0); // Red
  delay(100);
  setRGB(0, 0, 0);
  delay(50);
  }
  Serial.println("BLUE is On.");
  for(int i = 0; i < 1; i++) {
  setRGB(0, 0, 255); // Blue
  delay(100);
  setRGB(0, 0, 0);
  delay(50);
  }
}

void checkButton(const int BUTTON,  bool &last, bool &current, ButtonPressHandler handler ){
  
  current = debounce(last, BUTTON);              // Read debounced state
  
  if (last == LOW && current == HIGH)    // If it was pressed…
  {
    // This runs the code that we want to happen 
    // when the user presses the button.  The function ptr 
    // allows us to do different types of work
     handler();
  }
  last = current;                        // Reset button value
}

boolean debounce(boolean last, int button)
{
 boolean current = digitalRead(button);    // Read the button state
 if (last != current)                      // If it's different…
 {
  delay(5);                                // Wait 5ms -- thought delay mattered but it didn't (see INPUT_PULLDOWN note)
  current = digitalRead(button);           // Read it again
 }
 return current;                           // Return the current value
}
