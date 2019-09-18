#define FASTLED_INTERNAL                                      // this needs to come before #include <FastLED.h> to suppress pragma messages during compile time in the Arduino IDE.
#include "FastLED.h"                                          // FastLED library.


//LED FIXED GLOBAL
#define DATA_PIN 12                                           // Data pin to connect to the strip.
#define COLOR_ORDER GRB                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE WS2812                                       //
#define NUM_LEDS 43                                           // Number of LED's.

// LED changeable global variables.
uint8_t max_bright = 64;                                      // Overall brightness definition. It can be changed on the fly.
struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.
TBlendType    currentBlending = LINEARBLEND;                  // NOBLEND or LINEARBLEND



void setup() {

  //LED SETUP
  LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2812
  set_max_power_in_volts_and_milliamps(5, 2000);                  // FastLED Power management set at 5V, 2000mA, aka USB Port
  FastLED.setBrightness(max_bright);
  FastLED.setMaxRefreshRate(400);

}

void loop() {
  colorDrops();
  FastLED.show();
}

void colorDrops() {

  //Variables control the speed and shape of the drops
  uint16_t speed = 90; // lower is faster
  uint8_t dropWidth = 20; //lenght in pixel of each drop. Depending on fade, not all pixels will  be lit.
  uint8_t fade = 16; //length of the tail. Lower = longer tail.

  //Variables control the color/gradient the drops
  bool dropColorRepeats = true; //each drop is same color, or does each drop differ?
  uint8_t stripDelta = 7; // solid color or gradient across each drop?
  uint8_t timeDelta = 0;  // time change color. if dropColorRepeats=true, set to 0 to keep drop static as it moves. if dropColorRepeats = false, set = to stripDelta to keep static
  uint8_t baseHue = 196;   // if selecting static color above, use this to choose color
  static CRGBPalette16 currentPalette = RainbowColors_p ;  // change palette for underlying color. can bring in custom.

  //render next frame:
  uint8_t time = (millis() / speed);  //
  for (int i = 0; i < NUM_LEDS; i++) {

    //set color of each pixel
    if (dropColorRepeats) {
      leds[i] = ColorFromPalette(currentPalette, (((i + time) % dropWidth) * stripDelta) + (time * timeDelta) + baseHue , 255, currentBlending); //dropColorRepeats true and static color while moving
    }
    else {
      leds[i] = ColorFromPalette(currentPalette, (i * stripDelta) + (time * timeDelta) + baseHue , 255, currentBlending); //((i % dropWidth)*stripDelta)+(time*timeDelta)
    }

    //do some math to create the fading trail
    uint8_t fadeIndex = (i + time) % dropWidth;
    uint16_t thisFade = fadeIndex * fade;  //this can result in a number that is larger than 8 bit, so make 16 to avoid rollover
    thisFade = constrain(thisFade, 0, 255); //constrain result to 8 bit w/o rolling over. clamp at 255 on high end
    leds[i].fadeToBlackBy(thisFade);
  }

}// colorDrops
