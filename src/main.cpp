#include <Arduino.h>
#include <TimerOne.h>


const int BUTTON_DOWN = HIGH;
const int BUTTON_UP = LOW;
const int LED_ON = 0;
const int LED_OFF = 1;
const int RELAY_ON = 0;
const int RELAY_OFF = 1;
const int buttonPin = 5;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

// prod board
const int ledBlue =  4;
const int ledGreen =  3;
const int ledRed =  2;

const int relayOff =  6;
const int relayLow =  7;
const int relayMed =  8;
const int relayHi =  9;
const int relayLight =  10;


const int FAN_OFF = 0;
const int FAN_LOW = 1;
const int FAN_MED = 2;
const int FAN_HI = 3;

const int LED_NONE = FAN_OFF;
const int LED_RED = FAN_LOW ;
const int LED_GREEN = FAN_MED;
const int LED_BLUE = FAN_HI;


int countWhitePercent = 0;

int buttonState = 0;         // variable for reading the pushbutton status
bool previousStateDown = false;
unsigned long timeCount = 0;
int fanState = FAN_OFF;
int backLightState = LED_OFF;
int red = LED_OFF;
int green = LED_OFF;
int blue = LED_OFF;
bool needToSetFan = false;
bool toggledLight = false;

int whiteLEDstate = LED_NONE;


void setup()
{

  pinMode(ledPin, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(relayOff, OUTPUT);
  pinMode(relayLow, OUTPUT);
  pinMode(relayMed, OUTPUT);
  pinMode(relayHi, OUTPUT);
  pinMode(relayLight, OUTPUT);

  digitalWrite( relayOff, RELAY_OFF);
  digitalWrite( relayLow, RELAY_OFF);
  digitalWrite( relayMed, RELAY_OFF);
  digitalWrite( relayHi, RELAY_OFF);
  digitalWrite( relayLight, RELAY_OFF);

  pinMode(buttonPin, INPUT);
  Timer1.initialize(1000); // set a timer of length 1,000 microseconds (or 1mS)
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here

  sequenceLED();

}

void loop()
{

  if (digitalRead(buttonPin) == BUTTON_UP) {
    digitalWrite( ledPin, LED_OFF );

    showWhite();

    if (previousStateDown) {
      delay(10); // debounce
      // button was down, so we can act
      if (toggledLight) {
        toggledLight = false;
      } else {
        cycleFan();
        needToSetFan = true;
      }
      previousStateDown = false;
    }
    if (needToSetFan) {
      if (timeCount > 1000) {
        needToSetFan = false;
        setFan();
        Timer1.stop();
        timeCount = 0;
      }
    }

  } else {
    digitalWrite( ledPin, LED_ON );

    // button must be down
    if (!previousStateDown) {
      delay(10); // debounce
      //Just been pressed
      Timer1.restart();   //set our stopwatch to 0
      timeCount = 0;
      Timer1.start();     //and start it up
      previousStateDown = true;
    } else {
      if (timeCount > 500) {
        //500mS so toggle the light relay
        digitalWrite( relayLight, RELAY_ON );
        delay(300);
        digitalWrite( relayLight, RELAY_OFF );
        Timer1.stop();     //and stop it up
        timeCount = 0;
        toggledLight = true;
      }
    }
  }
}

int backLightControl(int currentState) {
  int led = LED_NONE;
  switch (currentState) {
    case 0:
      led =LED_RED;
      break;
    case 1:
      led = LED_GREEN;
      break;
    case 2:
      led = LED_BLUE;
      break;
    case 3:
      led = LED_RED;
      break;
    case 4:
      led = LED_GREEN;
      break;
    case 5:
      led = LED_BLUE;
      break;
    case 6:
      led = LED_GREEN;
      break;
    case 7:
      led = LED_BLUE;
      break;
    case 8:
     led = LED_GREEN;
      break;
    case 9:
    //  led = LED_GREEN;
      break;
    default:
      led = LED_NONE;
  }
 // led = LED_BLUE;
  return led;
}


void showWhite() {
  if (++backLightState > 50) {
    backLightState = 0;
  }
  if (  fanState == FAN_OFF) {
    // only show backlight if fan is off
    displayLedColor(backLightControl(backLightState));
  }
}




void displayLedColor(int color) {
  digitalWrite( ledBlue, LED_OFF);
  digitalWrite( ledGreen, LED_OFF);
  digitalWrite( ledRed, LED_OFF);
  switch (color) {
    case LED_RED:
      digitalWrite( ledRed, LED_ON);
      break;
    case LED_GREEN:
      digitalWrite( ledGreen, LED_ON);
      break;
    case LED_BLUE:
      digitalWrite( ledBlue, LED_ON);
      break;
    case LED_NONE:
    default:
      break;
  }
}


void fanControl(int level) {
  digitalWrite( level, RELAY_ON );
  delay(300);
  digitalWrite( level, RELAY_OFF );
}

void setFan() {
  switch (fanState) {
    case FAN_OFF:
      fanControl(relayOff);
      break;
    case FAN_LOW:
      fanControl(relayLow);
      break;
    case FAN_MED:
      fanControl(relayMed);
      break;
    case FAN_HI:
      fanControl(relayHi);
      break;
  }
}

void cycleFan() {
  if (++fanState > FAN_HI) {
    fanState = FAN_OFF;
  }
  displayLedColor(fanState) ;
}
/// --------------------------
/// Custom ISR Timer Routine
/// --------------------------
void timerIsr()
{
  timeCount++;
}


void sequenceLED() {
  digitalWrite( ledBlue, blue);
  digitalWrite( ledGreen, green);
  digitalWrite( ledRed, red);
  for (int i = 0 ; i < 12; i++) {
    delay(100);
    cycleFan();
  }

  delay(200);
  digitalWrite( ledBlue, LED_OFF);
  digitalWrite( ledGreen, LED_OFF);
  digitalWrite( ledRed, LED_OFF);
}

