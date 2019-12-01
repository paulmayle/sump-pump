#include <Arduino.h>
#include <TimerOne.h>

const int ledBlue = 8;
const int ledGreen = 7;
const int ledRed = 9;

const int relay = 11;

const int floatSwitch = 2;
const int floatSwitchLow = 3;

const int RELAY_OFF = LOW;
const int RELAY_ON = HIGH;

volatile unsigned long timeCount = 0;
volatile unsigned long relayDelay = 101;

enum color
{
  BLACK,
  RED,
  GREEN,
  BLUE,
  YELLOW,
  PURPLE,
  CYAN,
  WHITE
};

boolean lowDetector = true;

// Prototypes
void timerIsr();
void showWhite();
void displayLedColor(int color);
void fanControl(int level);
void setFan();
void displayLedColor(int color);
void sequenceLED();
void cycleFan();
void flashLed(unsigned int color);
void ledColor(unsigned int color);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(floatSwitch, INPUT_PULLUP);
  pinMode(floatSwitchLow, INPUT_PULLUP);

  digitalWrite(relay, RELAY_OFF);

  // pinMode(buttonPin, INPUT);
  Timer1.initialize(100000);        // set a timer of length 100,000 microseconds (or 100mS)
  Timer1.attachInterrupt(timerIsr); // attach the service routine here

  //sequenceLED();
}

// the loop function runs over and over again forever
void loop()
{
  // wait for a second
  if (digitalRead(floatSwitch))
  {
    // water level is high
    ledColor(BLUE);
    digitalWrite(relay, HIGH);
    relayDelay = 0;
    if (digitalRead(floatSwitchLow))
    {
      // should be low to indicate water
      //lowDetector = false;
    }
  }
  else
  {
    if (relayDelay > 100)
    {
      // we can turn off the pump
      relayDelay = 101; // don't let it wrap ..

      digitalWrite(relay, LOW);

      if (lowDetector && digitalRead(floatSwitchLow))
      {
        flashLed(RED);
      }
      else
      {
        flashLed(PURPLE);
      }
    }
    else
    {
      // pump should be running
      ledColor(GREEN);
      if (lowDetector && digitalRead(floatSwitchLow))
      {
        // don't need to wait for the timer, the level has dropped
        relayDelay = 101; // don't let it wrap ..
      }
    }
  }
}
bool ledState;
void flashLed(unsigned int color)
{
  if (ledState)
  {
    // led is on
    if (timeCount > 1)
    {
      timeCount = 0;
      ledColor(BLACK);
      ledState=false;
    }
  }
  else
  {
    // led is off
    if (timeCount > 7)
    {
      timeCount = 0;
      ledColor(color);
      ledState=true;
    }
  }
}

void ledColor(unsigned int color = BLACK)
{

  digitalWrite(ledGreen, LOW);
  digitalWrite(ledRed, LOW);
  digitalWrite(ledBlue, LOW);

  switch (color)
  {
  case BLACK:
    break;
  case RED:
    digitalWrite(ledRed, HIGH);
    break;
  case GREEN:
    digitalWrite(ledGreen, HIGH);
    break;
  case BLUE:
    digitalWrite(ledBlue, HIGH);
    break;
  case YELLOW:
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledRed, HIGH);
    break;
  case CYAN:
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledBlue, HIGH);
    break;
  case PURPLE:
    digitalWrite(ledBlue, HIGH);
    digitalWrite(ledRed, HIGH);
    break;
  case WHITE:
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledBlue, HIGH);
    break;
  default:
    break;
  }
}

/// --------------------------
/// Custom ISR Timer Routine
/// --------------------------
void timerIsr()
{
  timeCount++;
  relayDelay++;
}
