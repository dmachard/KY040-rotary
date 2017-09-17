/*
  The KY-040 rotary encoder with software debouncing and interrupts support

  MIT License

  Copyright (c) 2017 Denis MACHARD

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include "KY040rotary.h"

// init the button
KY040 Rotary(2, 3, 4);


void OnButtonClicked(void) {
  Serial.println("Button 1: clicked");
}
void OnButtonLeft(void) {
  Serial.println("Button 1: rotating left");
}
void OnButtonRight(void) {
  Serial.println("Button 1: rotating right");
}

/* Handlers for interrupts mode
  void RSW1_SwitchInterruptHandler(void) {
    RSW1.HandleSwitchInterrupt();
  }
  void RSW1_RotateInterruptHandler(void) {
    RSW1.HandleRotateInterrupt();
  }
*/

void setup() {
  // open the serial port
  Serial.begin(115200);
  Serial.println("Starting...");

  /* Init in interrupts mode
    if ( !RSW1.Begin(RSW1_SwitchInterruptHandler, RSW1_RotateInterruptHandler) ) {
  */
  if ( !Rotary.Begin() ) {
    Serial.println("unable to init rotate button");
    while (1);
  }

  // init callbacks
  Rotary.OnButtonClicked(OnButtonClicked);
  Rotary.OnButtonLeft(OnButtonLeft);
  Rotary.OnButtonRight(OnButtonRight);

  Serial.println("KY-040 rotary encoder OK");
}


void loop() {
  Rotary.Process( millis() );
}

