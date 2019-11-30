#include <Arduino.h>
#include <TimerOne.h>

// const int BUTTON_DOWN = HIGH;
// const int BUTTON_UP = LOW;
// const int LED_ON = 0;
// const int LED_OFF = 1;
// const int RELAY_ON = 0;
// const int RELAY_OFF = 1;
// const int buttonPin = 5;     // the number of the pushbutton pin
// const int ledPin =  13;      // the number of the LED pin

// // prod board
const int ledBlue = 8;
const int ledGreen = 7;
const int ledRed = 9;

const int relay = 11;

const int floatSwitch = 2;

const int RELAY_OFF = LOW;
const int RELAY_ON = HIGH;
// const int FAN_MED = 2;
// const int FAN_HI = 3;

// const int LED_NONE = FAN_OFF;
// const int LED_RED = FAN_LOW ;
// const int LED_GREEN = FAN_MED;
// const int LED_BLUE = FAN_HI;

// int countWhitePercent = 0;

// int buttonState = 0;         // variable for reading the pushbutton status
// bool previousStateDown = false;
unsigned long timeCount = 0;
// int fanState = FAN_OFF;
// int backLightState = LED_OFF;
// int red = LED_OFF;
// int green = LED_OFF;
// int blue = LED_OFF;
// bool needToSetFan = false;
// bool toggledLight = false;

// int whiteLEDstate = LED_NONE;

// Prototypes
void timerIsr();
void showWhite();
void displayLedColor(int color);
void fanControl(int level);
void setFan();
void displayLedColor(int color);
void sequenceLED();
void cycleFan();

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(relay, OUTPUT);

  digitalWrite(relay, RELAY_OFF);

  // pinMode(buttonPin, INPUT);
  Timer1.initialize(1000);          // set a timer of length 1,000,000 microseconds (or 1S)
  Timer1.attachInterrupt(timerIsr); // attach the service routine here

  //sequenceLED();
}

// the loop function runs over and over again forever
void loop()
{
  // wait for a second

  if (timeCount > 2000)
  {
    Timer1.stop();
    Timer1.restart(); //set our stopwatch to 0
    timeCount = 0;
    Timer1.start(); //and start it up
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    digitalWrite(ledBlue, !digitalRead(ledBlue));
    if (digitalRead(floatSwitch))
    {
      digitalWrite(ledGreen, !digitalRead(ledGreen));
      digitalWrite(ledRed, !digitalRead(ledRed));
      digitalWrite(relay, !digitalRead(relay));
    }
  }
}

void loop1()
{

  // if (digitalRead(buttonPin) == BUTTON_UP) {
  //   digitalWrite( ledPin, LED_OFF );

  //   showWhite();

  //   if (previousStateDown) {
  //     delay(10); // debounce
  //     // button was down, so we can act
  //     if (toggledLight) {
  //       toggledLight = false;
  //     } else {
  //       cycleFan();
  //       needToSetFan = true;
  //     }
  //     previousStateDown = false;
  //   }
  //   if (needToSetFan) {
  //     if (timeCount > 1000) {
  //       needToSetFan = false;
  //       setFan();
  //       Timer1.stop();
  //       timeCount = 0;
  //     }
  //   }
  // } else {
  //   digitalWrite( ledPin, LED_ON );

  //   // button must be down
  //   if (!previousStateDown) {
  //     delay(10); // debounce
  //     //Just been pressed
  //     Timer1.restart();   //set our stopwatch to 0
  //     timeCount = 0;
  //     Timer1.start();     //and start it up
  //     previousStateDown = true;
  //   } else {
  //     if (timeCount > 500) {
  //       //500mS so toggle the light relay
  //       digitalWrite( relayLight, RELAY_ON );
  //       delay(300);
  //       digitalWrite( relayLight, RELAY_OFF );
  //       Timer1.stop();     //and stop it up
  //       timeCount = 0;
  //       toggledLight = true;
  //     }
  //   }
  // }
}

// int backLightControl(int currentState) {
//   int led = LED_NONE;
//   switch (currentState) {
//     case 0:
//       led =LED_RED;
//       break;
//     case 1:
//       led = LED_GREEN;
//       break;
//     case 2:
//       led = LED_BLUE;
//       break;
//     case 3:
//       led = LED_RED;
//       break;
//     case 4:
//       led = LED_GREEN;
//       break;
//     case 5:
//       led = LED_BLUE;
//       break;
//     case 6:
//       led = LED_GREEN;
//       break;
//     case 7:
//       led = LED_BLUE;
//       break;
//     case 8:
//      led = LED_GREEN;
//       break;
//     case 9:
//     //  led = LED_GREEN;
//       break;
//     default:
//       led = LED_NONE;
//   }
//  // led = LED_BLUE;
//   return led;
// }

/// --------------------------
/// Custom ISR Timer Routine
/// --------------------------
void timerIsr()
{
  timeCount++;
}
