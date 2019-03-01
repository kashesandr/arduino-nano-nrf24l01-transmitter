#include "nRF24L01.h" //NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include "RF24.h"
#include "SPI.h"
#include <stdlib.h>

RF24 radio(9, 10); // NRF24L01 used SPI pins + Pin 9 and 10 on the NANO
const uint64_t pipe = 0xE6E6E6E6E6E6; // Needs to be the same for communicating between 2 NRF24L01
unsigned int data;

/**
 * Define class for button
 */
class Button {
  public:
    int pin;
    int state;
    bool pressed;
};

/**
 * Create array of buttons
 */
int buttonsCount = 2;
Button* buttons = new Button[buttonsCount];

void setup(void) {

//  Serial.begin(9600);

  radio.begin(); 
  radio.setPALevel( RF24_PA_MAX ) ;
  radio.setDataRate( RF24_250KBPS ) ;
  radio.setCRCLength( RF24_CRC_8 ) ;
  radio.setRetries(15, 15);
  radio.setAutoAck(false);
  radio.openWritingPipe(pipe); // Get NRF24L01 ready to transmit

  // buttons setup
  buttons[0].pin = 2;
  buttons[1].pin = 3;
  for (int i = 0; i < buttonsCount; ++i) {
    buttons[i].pressed = false;
    pinMode(buttons[i].pin, INPUT);
  }

}

void loop(void) {

  for (int n = 0; n < buttonsCount; n++) {
    buttons[n].state = digitalRead(buttons[n].pin);
    if (buttons[n].state == LOW) {
      if (!buttons[n].pressed) {
        data = buttons[n].pin;
//        Serial.println(data);
        radio.write(&data, sizeof(unsigned int));
        buttons[n].pressed = true;
      }
    } else {
      buttons[n].pressed = false;
    }

  }

}
