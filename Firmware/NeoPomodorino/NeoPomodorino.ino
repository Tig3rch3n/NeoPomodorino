#include <Adafruit_NeoPixel.h>

//Times (Defaults in Minutes)
#define POMODORO_LIMIT 4            //4
#define POMODORO_TIME  25           //25
#define SHORT_PAUSE    5            //5
#define LONG_PAUSE     25           //25

//Brightness
#define LUMINANCE      20

// Timer Colors //Default Colors
#define POMODORO_color CYAN                   //RED
#define PAUSE_SHORT_color  ORANGE             //YELLOW
#define PAUSE_LONG_color  WHITE               //GREEN
#define TIMER_off OFF

// Timer Break Color
#define POMODORO_start_animation WHITE_b      //RED_b
#define POMODORO_s_pause_animation ORANGE_b   //YELLOW_b
#define POMODORO_l_pause_animation RED_b      //GREEN_b

// Timer Color Chart       LED Count, RED, GREEN, BLUE
#define OFF     0, 0, 0
#define WHITE   intensity, intensity, intensity
#define RED     intensity, 0, 0
#define ORANGE  intensity, intensity /3, 0
#define YELLOW  intensity, intensity, 0
#define BLUE    0, 0, intensity
#define GREEN   0, intensity, intensity /5
#define CYAN    0, intensity, intensity /8
#define GREEN_P   intensity /2, intensity, intensity /2

// Break Color Chart       LED Count, RED, GREEN, BLUE
#define WHITE_b   LUMINANCE, LUMINANCE, LUMINANCE
#define RED_b     LUMINANCE, 0, 0
#define ORANGE_b  LUMINANCE, LUMINANCE /5, 0
#define YELLOW_b  LUMINANCE, LUMINANCE, 0
#define BLUE_b    0, 0, LUMINANCE
#define GREEN_b   0, LUMINANCE, 0
#define CYAN_b    0, LUMINANCE /10, LUMINANCE /10

//IO Stuff
#define BUTTON_PIN     2
#define PIXEL_PIN      6
#define PIXEL_COUNT    24

enum State
{
  Off = 0,
  Pomodoro = 1,
  ShortPause = 2,
  LongPause = 3
};

enum ButtonClick
{
  None,
  ShortClick,
  LongClick
};

Adafruit_NeoPixel ring = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

State currentState;
int pomodoro;
unsigned long starttime;
unsigned long duration;
bool buttonState = HIGH;

void setup() 
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  ring.begin();
  ring.show();
  
  turnOff();
}

void loop()
{
  unsigned long now = millis();
  ButtonClick button = getButtonClick();

  // Time can be extended by pressing the button. 1 short click => +1 minute.
  if (currentState != Off && button == ShortClick)
    starttime = min(starttime + 60000, now);

  // Long clicks can be used to turn the device on and off
  else if (button == LongClick)
  {
    if (currentState == Off)
      startPomodoro();
    else
      turnOff();
  }

  // If time runs out, go to next phase.
  else
  {
    unsigned long elapsedtime = now - starttime;
    if (elapsedtime > duration)
    {
      if (currentState == Pomodoro)
        startPause();
      else if (currentState == ShortPause || currentState == LongPause)
        startPomodoro();
    }
  }
  
  // Update ring
  updateDisplay();
  delay(20);

}

void turnOff()
{
  pomodoro = 0;
  currentState = Off;
}

void startPomodoro()
{
  pomodoro = (pomodoro % 4) + 1;
  blink((PIXEL_COUNT * pomodoro) / 4);
  starttime = millis();
  duration = POMODORO_TIME * 60000;
  currentState = Pomodoro;
  initLeds();
}

void startPause()
{
  starttime = millis();
  if (pomodoro < 4)
  {
    duration = SHORT_PAUSE * 60000;
    currentState = ShortPause;
  }
  else
  {
    duration = LONG_PAUSE * 60000;
    currentState = LongPause;
  }
  initLeds();
}

ButtonClick getButtonClick()
{
  ButtonClick result = None;
  bool newState = digitalRead(BUTTON_PIN);
  if (newState == LOW && buttonState == HIGH)
  {
    delay(20);
    buttonState = digitalRead(BUTTON_PIN);
    if (buttonState == LOW)
    {
      for(int i = 0; i < 25; i++)
      {
        delay(20);
        buttonState = digitalRead(BUTTON_PIN);
        if (buttonState == HIGH)
          return ShortClick;
      }
      return LongClick;
    }
    else
      return None;
  }
  else
  {
    buttonState = newState;
    return None;
  }
}

void updateDisplay()
{
  float progress = ((float)(millis() - starttime) / duration) * PIXEL_COUNT;
  int done = floor(progress);
  float fraction = progress - done;
  for(int i = 0; i < PIXEL_COUNT; i++)
  {
    uint8_t intensity = 0;
    if (i == done)
      intensity = LUMINANCE * (1 - fraction);
    else if (i > done)
      intensity = LUMINANCE;

    if (currentState == Pomodoro)
      ring.setPixelColor(i, POMODORO_color);
    else if (currentState == ShortPause)
      ring.setPixelColor(i, PAUSE_SHORT_color);
    else if (currentState == LongPause)
      ring.setPixelColor(i, PAUSE_LONG_color);
    else
      ring.setPixelColor(i, TIMER_off);
  }
  ring.show();
}

void blink(int ledcount)
{
  for(int i = 0; i < 3; i++)
  {
    for(int j = 0; j < PIXEL_COUNT; j++)
      ring.setPixelColor(j, 0, 0, j < ledcount ? LUMINANCE : 0); //Blink round counter BLUE
    ring.show();
    delay(200);
    for(int j = 0; j < PIXEL_COUNT; j++)
      ring.setPixelColor(j, 0, 0, 0);
    ring.show();
    delay(200);
  }
}

void initLeds()
{
  uint32_t color;
  if (currentState == Pomodoro)
    color = ring.Color(POMODORO_start_animation);
  else if (currentState == ShortPause)
    color = ring.Color(POMODORO_s_pause_animation);
  else if (currentState == LongPause)
    color = ring.Color(POMODORO_l_pause_animation);

  for(int i = PIXEL_COUNT; i > 0; i--)
  {
    ring.setPixelColor(i - 1, color);
    ring.show();
    delay(20);
  }
}