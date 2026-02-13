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

//included only once
#pragma once

typedef void(*callback)();
typedef void(*isr)();

class KY040
{
  public:
    uint8_t pinClk;
    uint8_t pinDt;
    int pinSw;

    bool basicMode;

    volatile int swState;
    unsigned long swLastTime;
    bool swDebounce;

    volatile int dtState;
    unsigned long dtLastTime;
    int dtPreviousPos;
    volatile int signalAB;
    bool dtDebounce;

  public:
    KY040(uint8_t pinClk, uint8_t pinDt, uint8_t pinSw);
    bool Begin(isr isr1 = NULL, isr isr2 = NULL);
    void Process(unsigned long t);
    void DecodeSignals(void);
    void HandleSwitchInterrupt( void );
    void HandleRotateInterrupt( void );
    void OnButtonClicked( callback );
    void OnButtonLeft( callback );
    void OnButtonRight( callback );

  protected:
    callback _OnCbClick = NULL;
    callback _OnCbLeft = NULL;
    callback _OnCbRight = NULL;
};
