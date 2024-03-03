/**
 * PROGRAM TO OPERATE ESTIM DEVICE
 * @author: Andrew Scott-George
 * Created 03/03/24
 * Changelog
 * - 
 */

#include <Time.h>

#define IN1 0 // Pin for enable 1 on L298
#define IN2 1 // Pin for enable 2 on L298
#define PWM 2 // Pin for modifying PWM on L298
#define BUTTON 3 // Pin for multifunction button

int buttonState = 0; // track button state; default set to 0.
int clicks = 0; // system state; default set to 0.
int buttonHoldTime = 0; // track how long button was held for
int turnOffTime = 2000; // How long button must be held for it to turn off


void setup() {
  Serial.begin(115200);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(BUTTON, OUTPUT);
}

void estim_on(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void estim_off(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void loop() {
  buttonState = digitalRead(BUTTON); // Get button state

  if (buttonState == 1){ // If button pressed
    if(buttonHoldTime == 0){ // if button was previously off...
      clicks += 1; // Increment clicks by 1
    }
    buttonHoldTime += millis(); // Add to button hold time for as long as it is held
  } else {
    buttonHoldTime = 0; // Reset button hold time to 0 when it is not pressed
  }

  if (buttonHoldTime > turnOffTime){
    estim_off(); // turn system off
    clicks = 0; // reset clicks to 0
    delay(1000); // delay, potentialy prevent system switching back on when this triggers but user still holding
  }

  switch (clicks){
    case 0: // system is off.
      estim_off();
      break;
    case 1: // system half power.
      estim_on();
      analogWrite(PWM, 127); // half intensity
      break; 
    case 2: // system full power.
      estim_on();
      analogWrite(PWM, 255); // Maximum intensity
      break;
    case 3: // turn off; reset clicks back to 0.
      estim_off();
      clicks = 0;
      break;
  }
}
