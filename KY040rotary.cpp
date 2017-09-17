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

#include <Arduino.h>
#include "KY040rotary.h"

#define KY040_SW_DEBOUNCE	50
#define KY040_DT_DEBOUNCE	160

KY040::KY040(uint8_t pinClk, uint8_t pinDt, uint8_t pinSw)
{
  this->pinClk = pinClk;
  this->pinDt = pinDt;
  this->pinSw = pinSw;

  this->basicMode = true;

  this->swState = 0;
  this->swLastTime = 0;
  this->swDebounce = false;

  this->dtState = 0;
  this->dtLastTime = 0;
  this->dtPreviousPos = 0;
  this->dtDebounce = false;

  this->signalAB = 0;
}

bool KY040::Begin(isr isr1, isr isr2)
{
  pinMode(this->pinClk, INPUT);
  pinMode(this->pinDt, INPUT);
  pinMode(this->pinSw, INPUT_PULLUP);

  if ( isr1 != NULL ) attachInterrupt(digitalPinToInterrupt(this->pinSw), isr1, CHANGE);
  if ( isr2 != NULL ) {
    attachInterrupt(digitalPinToInterrupt(this->pinClk), isr2, CHANGE);
    attachInterrupt(digitalPinToInterrupt(this->pinDt), isr2, CHANGE);
  }

  if ( isr1 != NULL || isr2 != NULL ) {
    this->basicMode = false;
  }

  delay(300);
  return true;
}

void KY040::Process(unsigned long t)
{
  if ( this->basicMode ) {
    this->DecodeSignals();

    if (digitalRead(this->pinSw) == LOW) {
      this->swState = 1;
    }
  }

  // checking button with debouncing
  if ( this->swState && this->swDebounce == false ) {
    this->swLastTime = t % 1000;
    this->swDebounce = true;
  }
  if ( this->swDebounce && ((t % 1000 - this->swLastTime) > KY040_SW_DEBOUNCE) ) {
    if (digitalRead(this->pinSw) == HIGH) {
      if (this->_OnCbClick) this->_OnCbClick();
    }
    this->swState = 0;
    this->swDebounce = false;
  }

  // checking button direction with debouncing
  if (this->dtState  && this->dtDebounce == false) {
    this->dtLastTime = (t % 1000);
    this->dtDebounce = true;
  }
  if ( this->dtDebounce && ((t % 1000) - this->dtLastTime) > KY040_DT_DEBOUNCE ) {
    if (this->dtState == 1) {
      if (this->_OnCbRight) this->_OnCbRight();
    }
    if (this->dtState == 2) {
      if (this->_OnCbLeft) this->_OnCbLeft();
    }
    this->dtState = 0;
    this->dtDebounce = false;
  }

}

void KY040::HandleSwitchInterrupt(void)
{
  this->swState = 1;
}

void KY040::HandleRotateInterrupt(void)
{
  this->DecodeSignals();
}

void KY040::DecodeSignals(void)
{
  int signalA = digitalRead(this->pinClk);
  int signalB = digitalRead(this->pinDt);

  int encoded = (signalB << 1) | signalA;  // converting the 2 pin value to single number
  int sum  = (this->signalAB << 2) | encoded; // adding it to the previous encoded value

  if (sum == 0b0001 || sum == 0b0111 || sum == 0b1110 || sum == 0b1000) {
    this->dtState = 1;
  }
  if (sum == 0b0010 || sum == 0b1011 || sum == 0b1101 || sum == 0b0100) {
    this->dtState = 2;
  }

  this->signalAB = encoded; // store this value for next time
}

void KY040::OnButtonClicked( callback cb )
{
  this->_OnCbClick = cb;
}

void KY040::OnButtonLeft( callback cb )
{
  this->_OnCbLeft = cb;
}

void KY040::OnButtonRight( callback cb )
{
  this->_OnCbRight = cb;
}
